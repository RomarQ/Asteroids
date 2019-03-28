// Preprocessor Directives
#ifndef ASTEROIDS
#define ASTEROIDS
#pragma once

// System Headers
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <math.h>

#include "shader.hpp"
// Reference: https://github.com/nothings/stb/blob/master/stb_image.h#L4
// To use stb_image, add this in *one* C++ source file.
//     #define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// Prototypes
void showFPS(GLFWwindow* window);

// Constants
const float mWidth = 1024;
const float mHeight = 578;
const char* APP_TITLE = "Asteroids";

#endif