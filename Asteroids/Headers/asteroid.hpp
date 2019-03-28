#ifndef ASTEROID_H
#define ASTEROID_H

#include "shader.hpp"
#include "model.hpp"
#include "camera.hpp"

const float ASTEROID_SPEED = 0.005;
const int ASTEROID_MAX = 10;
const float ASTEROID_COOLDOWN = 2;

namespace Asteroids {
    class Asteroid {
        private:
            Shader shader;
            float angle = 0;
            double xOffSet = 0;
            double zOffSet = 0;

        public:
            Asteroid(float width, float height, Camera camera);
            ~Asteroid();

            bool render(float width, float height, Camera camera);
            void dislocateAsteroid();
            void generateCoordinates();
    };

    void loadModel(string modelPath);
    void renderAsteroids(float width, float height, Camera camera);
    bool readyToSpawn();

}

#endif