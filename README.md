# Turtle-OS OpenGL Setup

## Overview

### Install the required packages (PoP!-OS)

```bash
sudo apt install libglfw3-dev
sudo apt install libgles2-mesa-dev libegl1-mesa-dev mesa-common-dev
```

### Install the required packages Pinebook-pro (Manjaro)

```bash
sudo pacman -S --needed --noconfirm gcc g++ make pkgconf glfw-wayland libglvnd mesa
```

Note: Use glfw-wayland for Wayland sessions. Do not mix with glfw-x11.

## Build & Run

Go to the project root directory. Compile the project:

```bash
make -j
```

Compile and clean previous build:

```bash
make clean && make -j
```

Run the application:

```bash
./bin/Turtle-OS
```

## Project Structure

```txt
├── src/
│   ├── main.cpp                  # Entry point
│   ├── engine.h/.cpp             # Main engine
│   ├── turtle_baseclass.h/.cpp   # Turtle Base
│   ├── text_renderer.h/.cpp      # Text rendering on canvas
│   ├── stb_truetype.h            # Font loading library
│   └── shaders/
│       ├── vertex.glsl
│       └── fragment.glsl
├── bin/                          # Compiled binaries
├── obj/                          # Compiled .o files
├── data/                         # Assets, fill with images and fonts
│   └── font.ttf                  # TrueType font for text rendering
├── log/                          # Log files
└── Makefile                      # Build instructions
```

## Features

- **Image Viewer**: Browse through images in the data directory using arrow keys
- **Text Rendering**: Display text overlays on the canvas using TrueType fonts
- **Zoom Animation**: Smooth zoom transitions when switching between images

## Notes

- Shader sources are loaded from files (src/shaders/vertex.glsl and src/shaders/fragment.glsl).
- Window class handles GLFW initialization and cleanup automatically.
- Text rendering uses stb_truetype library for font loading and rendering.
- Place a TrueType font file at `data/font.ttf` for text rendering to work.
- Triangle vertices are currently hardcoded in main.cpp but can be moved to a file in the future.
- Ensure your Pinebook Pro supports OpenGL ES 2.0 via Panfrost drivers.
