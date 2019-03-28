#ifndef ASTEROID_H
#define ASTEROID_H

#include "shader.hpp"
#include "model.hpp"
#include "camera.hpp"

const float ASTEROID_SPEED = 0.005;
const int ASTEROID_MAX = 30;
const float ASTEROID_COOLDOWN = 1;

namespace Asteroids {
    class Asteroid {
        private:
            Shader shader;

        public:
            float angle = 0;
            float rotation = 0;
            float rotationSpeed = 0.5;
            double xOffSet = 0;
            double yOffSet = 0;

            Asteroid(float width, float height, Camera camera);
            ~Asteroid();

            bool render(float width, float height, Camera camera);
            void dislocateAsteroid();
            void generateCoordinates();
    };

    void loadModel(string modelPath);
    void renderAsteroids(float width, float height, Camera camera);
    bool readyToSpawn();
    void CheckAsteroidCollisions();

}

#endif