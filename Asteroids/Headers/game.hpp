#ifndef GAME_H
#define GAME_H

#include <GLFW/glfw3.h>
#include "camera.hpp"

#include "asteroid.hpp"

#include "spaceShip.hpp"
using namespace SpaceShips;

// Represents the current state of the game
enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

namespace Asteroids {

    void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

    class Game {
        public:
            // Game state
            GameState State;

            // Space Ship
            // ------------------------------------
            SpaceShips::SpaceShip *spaceShip;

            // Constructor/Destructor
            Game(float width, float height, GLFWwindow* window);
            ~Game();
            // Initialize game state (load all shaders/textures/levels)
            void Init();

            void ProcessInput(GLFWwindow *window);
            void Update(float dt);
            void Render();
    };
}

#endif