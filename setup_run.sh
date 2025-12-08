#!/bin/bash
# Minimal GL template setup & run for Pinebook Pro

APP_NAME="app"   # << change this once
SRC_FILE="${APP_NAME}.c"
BIN_FILE="${APP_NAME}"

# 1. Install dependencies
sudo pacman -S --needed --noconfirm gcc make pkgconf glfw-x11 libglvnd mesa

# 2. Compile the program
gcc "$SRC_FILE" -o "$BIN_FILE" -lGLESv2 -lglfw

# 3. Run it
./"$BIN_FILE"
