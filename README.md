# Turtle-OS

A slideshow learning tool that helps kids explore topics through images and information!

## What is Turtle-OS?

Turtle-OS is a simple slideshow system designed for kids to learn about different topics. You organize your learning materials into folders by subject (like animals, space, dinosaurs, etc.), and Turtle-OS displays them as an interactive slideshow.

## Important: Data is NOT Included!

**This repository contains only the Turtle-OS program itself.** You need to add your own images and learning materials to the `data/` folder. The data folder is empty by default and ignored by git.

## How to Organize Your Learning Materials

Turtle-OS uses a simple folder structure to organize topics. Create folders inside the `data/` directory following this pattern:

```
data/
├── biology/
│   ├── animals/
│   │   ├── mammals/
│   │   │   ├── lion/
│   │   │   │   ├── name.txt         # Name of the topic
│   │   │   │   └── images/          # Folder with pictures
│   │   │   │       ├── lion1.jpg
│   │   │   │       ├── lion2.jpg
│   │   │   │       └── lion3.png
│   │   │   ├── elephant/
│   │   │   │   ├── name.txt
│   │   │   │   └── images/
│   │   │   └── whale/
│   │   │       ├── name.txt
│   │   │       └── images/
│   │   └── birds/
│   │       └── eagle/
│   │           ├── name.txt
│   │           └── images/
│   └── plants/
│       └── trees/
│           └── oak/
│               ├── name.txt
│               └── images/
├── space/
│   ├── planets/
│   │   ├── mars/
│   │   │   ├── name.txt
│   │   │   └── images/
│   │   └── jupiter/
│   │       ├── name.txt
│   │       └── images/
│   └── stars/
└── history/
    └── dinosaurs/
        ├── t-rex/
        │   ├── name.txt
        │   └── images/
        └── triceratops/
            ├── name.txt
            └── images/
```

### Structure Explained:

- **Main category** (biology, space, history) → **Subcategories** (animals, planets) → **Types** (mammals, birds) → **Specific topic** (lion, elephant)
- Each topic folder contains:
  - `name.txt` - A simple text file with the name or description of the topic
  - `images/` - A folder with all the pictures for this topic (jpg, png, etc.)

This way, you can organize lots of different learning topics and Turtle-OS will find all the images to show you!

## Setup & Build Instructions

### Install Required Packages

**For PoP!-OS or Ubuntu:**
```bash
sudo apt install libglfw3-dev
sudo apt install libgles2-mesa-dev libegl1-mesa-dev mesa-common-dev
```

**For Pinebook Pro (Manjaro):**
```bash
sudo pacman -S --needed --noconfirm gcc g++ make pkgconf glfw-wayland libglvnd mesa
```
Note: Use glfw-wayland for Wayland sessions. Do not mix with glfw-x11.

### Build the Program

1. Compile the project:
```bash
make
```

2. To rebuild from scratch:
```bash
make clean && make
```

### Run Turtle-OS

After building, run the slideshow:
```bash
./bin/Turtle-OS
```

The program will look in the `data/` folder for your images and display them!

## Project Structure

```
├── src/                          # Source code
│   ├── main.cpp                  # Entry point
│   ├── turtle_baseclass.h/.cpp   # Core functionality
│   └── shaders/                  # Graphics shaders
│       ├── vertex.glsl
│       └── fragment.glsl
├── bin/                          # Compiled program (created when you build)
├── data/                         # YOUR learning materials go here!
│   └── (empty - add your organized folders here)
├── log/                          # Log files
└── Makefile                      # Build instructions
```

## Technical Notes

- Turtle-OS uses OpenGL ES 2.0 for graphics rendering
- Ensure your system supports OpenGL ES 2.0 (Panfrost drivers for Pinebook Pro)
- The program automatically finds and loads images from the data folder structure
