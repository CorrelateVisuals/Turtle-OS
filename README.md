# Turtle-OS OpenGL Setup

## Overview

This project demonstrates a minimal OpenGL ES 2.0 application using GLFW on a Pinebook Pro (Manjaro Linux). It renders a simple triangle with modern C++17 code and RAII-safe window management.

## Dependencies (Manjaro)

Install the required packages:

sudo pacman -S --needed --noconfirm gcc g++ make pkgconf glfw-wayland libglvnd mesa

Note: Use glfw-wayland for Wayland sessions. Do not mix with glfw-x11.

## Build & Run

1. Compile the project:

make

2. Run the application:

./bin/app

## Project Structure

.
├── src/
│   ├── main.cpp       # Entry point
│   ├── window.h/cpp   # Window class (RAII)
│   ├── shader.h/cpp   # Shader utilities
│   └── shaders/
│       ├── vertex.glsl
│       └── fragment.glsl
├── bin/               # Compiled binaries
├── data/              # Assets (optional)
├── log/               # Log files (optional)
└── Makefile           # Build instructions

## Notes

- Shader sources are loaded from files (src/shaders/vertex.glsl and src/shaders/fragment.glsl).
- Window class handles GLFW initialization and cleanup automatically.
- Triangle vertices are currently hardcoded in main.cpp but can be moved to a file in the future.
- Ensure your Pinebook Pro supports OpenGL ES 2.0 via Panfrost drivers.
