// Local Headers
#include "game.hpp"
#include "shader.hpp"
#include "spaceShip.hpp"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <iostream>

// Prototypes
void framebuffer_size_callback(GLFWwindow*, int, int);
void processInput(GLFWwindow*);

// Space Ship
// ------------------------------------
SpaceShip *spaceShip;


int main() {

    // Load GLFW and Create a Window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    // GLFW_OPENGL_FORWARD_COMPAT Hint needs to be set on Darwin systems "Mac OS"
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    //glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    GLFWwindow* window = glfwCreateWindow(mWidth, mHeight, "Asteroids", nullptr, nullptr);

    // Check for Valid Context
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Create Context and Load OpenGL Functions
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    // Show OpenGL version
    std::cerr << "OpenGL " << glGetString(GL_VERSION) << std::endl;

    // build and compile the Space Ship
    // ------------------------------------
    spaceShip = new SpaceShip("../Asteroids/Shaders/space_ship.vs", "../Asteroids/Shaders/space_ship.fs");

    // Rendering Loop
    while (!glfwWindowShouldClose(window)) {
        // Input Control
        processInput(window);

        // { Begin of Render Logic        
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        spaceShip->render();

        // } End of Render Logic

        // Flip Buffers and Draw
        glfwSwapBuffers(window);
        // Check and Call Events
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return EXIT_SUCCESS;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    } 
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {   
        std::cout << "UP" << std::endl;
        spaceShip->moveForward();
    }
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {   
        std::cout << "DOWN" << std::endl;
        spaceShip->moveBackward();
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {   
        std::cout << "LEFT" << std::endl;
        spaceShip->rotateLeft();
    }
    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {   
        std::cout << "RIGHT" << std::endl;
        spaceShip->rotateRight();
    }

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
}
