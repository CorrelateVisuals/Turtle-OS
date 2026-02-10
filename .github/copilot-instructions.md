# Turtle-OS AI Coding Guidelines

## Project Overview
Turtle-OS is an OpenGL ES 2.0 graphics application for rendering and displaying image galleries with interactive zoom effects. It uses C++17, GLFW for windowing, and stb_image for texture loading.

## Architecture

### Core Hierarchy
```
Engine (inherits TurtleBase)
├── MainCanvas (inherits TextureArray)
├── GLSL (static shader/render backend)
└── Window (GLFW wrapper)

TurtleBase (base class with nested hierarchy)
├── Geometry::Shape::Vertex (base for 3D objects)
├── Image::TextureArray (inherits Shape, manages image collections)
│   └── Texture (single image loading)
└── ScaleVertices (inherits Uniform, easing animations)
```

### Data Flow
1. **Initialization** (`main.cpp` → `Engine::Engine()`)
   - Engine inherits from TurtleBase and initializes MainCanvas
   - MainCanvas loads all images from `data/` directory via ImageFinder

2. **Render Loop** (`Engine::run()`)
   - GLSL::render_loop() called continuously
   - Window::should_close() polls GLFW events
   - ScaleVertices (_zoom) updates vertex scale with easing animation
   - Shader uniforms updated and textures rendered

3. **Image Discovery** (ImageFinder class)
   - Recursively scans `data/` directory for supported image formats
   - Loads up to 1000 images (TEXTURE_ARRAY_SIZE constant)
   - Randomizes order and supports texture switching via index

## Key Conventions

### Class Organization
- **Nested classes**: TurtleBase uses deeply nested public classes (Geometry, Image) to organize related functionality. Prefer maintaining this structure when extending.
- **Inheritance pattern**: Classes use protected inheritance from geometry base classes to inherit vertex/shape data while hiding implementation.
- **Static backends**: GLSL class uses static methods - extend with additional static methods for new shader operations.

### Naming Conventions
- Member variables: `_snake_case` with leading underscore
- Methods: `snake_case` lowercase
- Configuration methods: `print_config()` and `getSettings()` for debugging output
- Texture paths stored as `std::string` in `_path` members

### Resource Management
- TextureArray uses fixed-size array `std::array<std::string, TEXTURE_ARRAY_SIZE>` (1000 slots)
- ImageFinder::get_filled_array_count() returns actual populated count for bounds checking
- GLuint texture handles stored directly in objects (`_image`, `_current_image`)

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

### Adding New Uniforms
1. Create class inheriting from `Uniform` (see ScaleVertices example)
2. Set `_shader_location` during initialization
3. Call `set_uniform_value()` in update loop before render

### Loading & Switching Textures
- Single texture: Use `Texture::load_texture()` which calls `GLSL::texture_to_gpu()`
- Texture arrays: Use `TextureArray::select_texture(index)` for switching
- Image discovery automatic via `ImageFinder::find_all_images()`

### Debugging Output
- Call `print_config()` methods in constructors for initialization logs
- TextureArray logs texture switches with index info
- Image loading shows dimensions and stbi status
- Uniform initialization logs shader locations and values

## Critical Notes

- **Shader binding**: Uniforms need explicit shader location setting - location ID comes from GLSL program
- **Vertex data**: Hardcoded in Shape::_vertices (4 vertices for quad with texture coords)
- **Z-buffer**: Ensure render order correct for overlapping geometry (not currently sorted)
- **Image format**: stb_image loaded with RGBA 4-channel requirement in load_texture()
