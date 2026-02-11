# Turtle-OS AI Coding Guidelines

## Project Overview
Turtle-OS is an OpenGL ES 2.0 graphics application for rendering and displaying image galleries with interactive zoom effects. It uses C++17, GLFW for windowing, and stb_image for texture loading.

Turtle-OS is to become a modular, extensible codebase for learning graphics programming concepts. The architecture emphasizes clear separation of concerns, with a core Engine class managing the main loop and rendering, and nested canvas classes organizing shader uniforms, geometry data, and texture management.

A Touchdesigner light if you may, based on Shaders chained together. In and outputs, effects and so on. The main goal is to create a flexible system for rendering images with GPU-accelerated effects, while maintaining clean code organization and extensibility for future features.

## Architecture

### Core Hierarchy
```
Engine (top-level class with nested canvases)
├── MainCanvas (nested class, inherits Image::TextureArray)
├── FrontCanvas (nested class, inherits Image::Texture)
├── FrontCanvas2 (nested class, inherits Image::Texture)
├── LowerCanvas (nested class, inherits Image::Texture)
├── GLSL (static shader/render backend)
└── Window (GLFW wrapper)

Base Classes (standalone, not under TurtleBase)
├── Uniform (shader value wrapper)
│   └── ScaleVertices (easing animations with timing)
├── Geometry
│   ├── Vertex (position, rotation, naming)
│   └── Shape (quad mesh, scaling, indices)
└── Image
    ├── Texture (single image loading & GPU upload)
    └── TextureArray (collection manager, image switching)
```

### Data Flow
1. **Initialization** (`main.cpp` → `Engine::Engine()`)
   - Creates Engine (top-level class)
   - Initializes multiple canvases: MainCanvas (gallery), FrontCanvas/FrontCanvas2/LowerCanvas (overlays)
   - ImageFinder recursively scans directory (default: "data") and populates TextureArray::_images array in MainCanvas
   - Shaders compiled and bound via `create_shaders()`
   - Window key/mouse callbacks registered via `glfwSetWindowUserPointer()` + static handlers

2. **Render Loop** (`Engine::run()`)
   - While `!Window::should_close()`:
     * Poll events (GLFW input)
     * GLSL::render_loop() for MainCanvas (gallery with zoom)
     * ScaleVertices::run() - updates zoom animation (easing calculations)
     * GLSL::render_loop() for overlay canvases (FrontCanvas, etc.)
     * Window::swap_buffers() + Window::poll_events()

3. **Image Discovery & Switching**
   - ImageFinder::find_all_images() recursively loads paths into TextureArray::_images array
   - TextureArray::select_texture(index) switches current image + logs debug info
   - Arrow keys trigger texture switch via GLFW callback, reset zoom animation
   - Support for up to 1000 images via TEXTURE_ARRAY_SIZE constant
   - Mouse handling: dragging/resizing canvases (added feature, not in original)

## Key Conventions

### Class Organization
- **Nested classes**: Engine uses nested canvas classes for UI layers. Base classes (Uniform, Geometry, Image) are standalone.
- **Inheritance pattern**: Child classes inherit vertex/shape data via public/protected inheritance (e.g., Shape : Vertex, TextureArray : Texture : Geometry::Shape)
- **Static backends**: GLSL class uses only static methods - all rendering is stateless
- **Callback handling**: GLFW callbacks use `glfwSetWindowUserPointer()` to store Engine*, then cast in static handler (see `Engine::key_call_back()`)

### Naming Conventions
- Member variables: `_snake_case` with leading underscore (e.g., `_zoom`, `_image_index`, `_vertices`)
- Methods: `snake_case` lowercase
- Configuration methods: `print_config()` and `getSettings()` for debugging output
- Texture paths: stored as `std::string` in `_path` member
- Array constants: all caps (e.g., `TEXTURE_ARRAY_SIZE`)

### Resource Management
- TextureArray uses fixed-size array `std::array<std::string, TEXTURE_ARRAY_SIZE>` (1000 slots for paths)
- GLuint texture handles stored directly in objects (`_image` in Texture class)
- ImageFinder::get_filled_array_count() must be called to get actual populated count (not all 1000 slots may be filled)
- Images loaded on-demand via Texture::load_texture() → GLSL::texture_to_gpu()

## Build & Development

### Build Commands
```bash
make -j              # Compile (parallel jobs)
make clean && make -j # Clean and recompile
./bin/Turtle-OS      # Run executable
```

### Makefile Details
- Uses C++17 standard with strict warnings (-Wall)
- Links against GLESv2 and GLFW libraries
- Object files in `obj/`, binary in `bin/`
- Shader includes via `-I./src/shaders` (loads from files, not hardcoded)

### Dependencies
- **Platform-specific**: Install libglfw3-dev + GLES2 dev libraries (varies by OS)
- **Shader files**: Loaded from `src/shaders/vertex.glsl` and `src/shaders/fragment.glsl`
- **Assets**: Place images in `data/` directory (nested subdirectories OK)

## Common Patterns

### GLFW Window Integration
- Window is managed by dedicated Window class wrapping GLFWwindow*
- Key callback pattern: store Engine* via `glfwSetWindowUserPointer(window, this)` in constructor
- Retrieve via `glfwGetWindowUserPointer(window)` and cast to Engine* in static callback function
- Check `action == GLFW_PRESS` to filter key events before processing

### Adding New Uniforms
1. Create class inheriting from `Uniform` in base_shader_access.h (see ScaleVertices for example)
2. Constructor: `Uniform(shader_location, initial_value)`
3. Initialize `_shader_location` from GLSL program after shader compilation
4. Call `set_uniform_value()` before each render if value changes
5. ScaleVertices adds timing fields (`_zoom_speed`, `_ease_factor`) and `run()` for frame updates

### Loading & Switching Textures
- Single texture: Create Texture instance → constructor calls `load_texture(path)` → GLSL::texture_to_gpu()
- Texture arrays: TextureArray::select_texture(index) updates `_image_index` and loads image at that path
- Image discovery: ImageFinder::find_all_images() fills std::array with paths recursively (no filtering - includes all supported formats)
- Check array bounds with ImageFinder::get_filled_array_count()

### Debug Output
- Call `print_config()` methods in constructors for initialization logs
- Check DEBUG_LEVEL enum (OFF, BASIC, DETAILED) set in Engine constructor before logging
- TextureArray::select_texture() logs current index and path
- Texture::load_texture() logs dimensions and stbi_load status
- Example: `if (DEBUG_LEVEL >= DebugLevel::BASIC) { std::cout << ... }`

## Critical Notes

- **Shader binding**: Uniforms need explicit shader location setting - location ID comes from GLSL program after compilation
- **Vertex data**: Hardcoded in Shape::_vertices (4 vertices for quad, with texture coordinates)
- **Texture coordinates**: Vertices include position (x,y) + texture coords (u,v) in format: position.x, position.y, tex_u, tex_v
- **Index buffer**: Shape::_indices = {0,1,2,2,3,0} - two triangles forming quad
- **Image format**: stb_image loaded with RGBA 4-channel requirement in load_texture()
- **GLFW pointer pattern**: CRITICAL - must use glfwSetWindowUserPointer() before GLFW can access Engine members in callbacks
- **MainCanvas folder**: Defaults to "data/" - pass alternative path in Engine constructor if needed
- **Exception handling**: main.cpp wraps engine.run() in try-catch, logs std::exception::what()
- **Debug level**: Set DEBUG_LEVEL = DebugLevel::BASIC|DETAILED in Engine constructor to control output verbosity
- **Multi-canvas rendering**: Added overlays (FrontCanvas, etc.) for UI layers; render in sequence after main gallery
- **Mouse interactions**: Dragging/resizing canvases via mouse callbacks (extension beyond original gallery focus)
