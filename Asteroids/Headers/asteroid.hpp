#ifndef ASTEROID_H
#define ASTEROID_H

#include "shader.hpp"
#include "model.hpp"
#include "camera.hpp"

const float ASTEROID_SPAWN_COOLDOWN = 1;
const float ASTEROID_MOVEMENT_COOLDOWN = 0.01;
const float ASTEROID_ROTATION_COOLDOWN = 0.01;
const float ASTEROID_MOVEMENT_SPEED = 0.005;
const float ASTEROID_ROTATION_SPEED = 0.1;
const int ASTEROID_MAX = 30;
const float ASTEROID_BASE_RADIUS = 0.14;

const glm::vec2 ASTEROID_QUADRANTS[4] = {
    glm::vec2(ASTEROID_BASE_RADIUS, ASTEROID_BASE_RADIUS),
    glm::vec2(-ASTEROID_BASE_RADIUS, ASTEROID_BASE_RADIUS),
    glm::vec2(-ASTEROID_BASE_RADIUS, -ASTEROID_BASE_RADIUS),
    glm::vec2(ASTEROID_BASE_RADIUS, -ASTEROID_BASE_RADIUS)
};

const int asteroidTypes = 2;
const float maxScore = 3.5;

namespace Asteroids {
    class Asteroid {
        private:
            Shader shader;

        public:
            int type = 1;
            float angle = 0;
            float rotation = 0;
            float lastMovementTimestamp = 0;
            float lastRotationTimestamp = 0;
            double xOffSet = 0;
            double yOffSet = 0;

            Asteroid(float width, float height, Camera camera);
            ~Asteroid();

            bool render(float width, float height, Camera camera);
            void dislocateAsteroid();
            void rotateAsteroid();
            void generateCoordinates();
    };

    void loadModel(string modelPath);

    vector<Asteroid*> * getAsteroids();

    void renderAsteroids(float width, float height, Camera camera);
    bool readyToSpawn();

}

#endif