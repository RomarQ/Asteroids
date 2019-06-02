
# Asteroid

## Progress Sample

![Screenshot](progress.gif)

## TODO
* [X] Space Ship Movement/Rotation with User Input
* [X] Random Spawn of Asteroids Spawn
* [X] Asteroid vs Asteroid Collision and Asteroid Dispawn
* [X] FPS display
* [X] Space Ship Projectiles
* [X] Space Ship vs Asteroid Collision
* [X] Projectile vs Asteroid Collision
* [X] Difficulty System
* [X] Scoring System
* [X] Game Start/End Menu
* [ ] CLEAN ALL SPAGHETTI CODE!!!
  
## Summary
School Project [ Will fill this later ]

## Building with cmake

```bash
$ git clone --recursive https://github.com/RoMarQ/Asteroids
$ cd Asteroids
$ cd Asteroids/Vendor/glad/ && cmake . && make
$ cd ../../../Build/

# UNIX Makefile
$ cmake ..
$ make && ./Asteroids/Asteroids

# ! This project wasn't tested on Mac and Windows yet, but should work fine.

# Mac OSX
cmake -G "Xcode" ..

# Microsoft Windows
cmake -G "Visual Studio 15" ..
cmake -G "Visual Studio 15 Win64" ..

```

Functionality           | Library
----------------------- | ------------------------------------------
Mesh Loading            | [assimp](https://github.com/assimp/assimp)
OpenGL Function Loader  | [glad](https://github.com/Dav1dde/glad)
Windowing and Input     | [glfw](https://github.com/glfw/glfw)
OpenGL Mathematics      | [glm](https://github.com/g-truc/glm)
Texture Loading         | [stb](https://github.com/nothings/stb)
Text Rendering          | [freeType](https://www.freetype.org/)


## Boilerplate Used: [Glitter](https://github.com/Polytonic/Glitter)
