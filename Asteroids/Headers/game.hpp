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
    GAME_OVER,
    GAME_TEST
};

const int GAME_MAX_DIFFFICULTY = 2;
const int GAME_MAX_LIVES = 3;
const float MENU_KEY_PRESS_COOLDOWN = 0.2;

namespace Games {
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    int PositiveAngle(int angle);

    class Game {
        public:
            // Game state
            GameState State;
            int difficulty = 1;

            // Lives counter
            int lives = GAME_MAX_LIVES;
            // Score Counter
            int score = 0;

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
            void checkShipCollisions();
            void updateGameState(GameState state);
    };
}

#endif