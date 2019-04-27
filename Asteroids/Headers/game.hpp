#ifndef GAME_H
#define GAME_H

#include "camera.hpp"

#include "textRenderer.hpp"
#include "spaceShip.hpp"
#include "projectile.hpp"
#include "asteroid.hpp"
#include "textRenderer.hpp"

using namespace TextRenders;
using namespace Asteroids;
using namespace Projectiles;
using namespace SpaceShips;

// Represents the current state of the game
enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

namespace Games {

    void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    int PositiveAngle(int angle);

    class Game {
        public:
            // Game state
            GameState State;

            // Score Counter
            int score = 0;
            TextRenderer scoreDisplay;

            Shader *textShader;

            // Space Ship
            // ------------------------------------
            SpaceShips::SpaceShip *spaceShip;

            // Constructor/Destructor
            Game(float width, float height, GLFWwindow* window);
            ~Game();
            // Initialize game state (load all shaders/textures/levels)
            void Init();

            void ProcessInput(GLFWwindow *window);
            void Render();

            void checkProjectileCollisions();
            void checkAsteroidCollisions();
            bool checkShipCollisions();
    };
}

#endif