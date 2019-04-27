#ifndef ASTEROID_H
#define ASTEROID_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <GLFW/glfw3.h>

#include "shader.hpp"
#include "model.hpp"
#include "camera.hpp"
#include "collision.hpp"
using namespace Collision;

const float ASTEROID_SPAWN_COOLDOWN = 1;
const float ASTEROID_MOVEMENT_COOLDOWN = 0.01;
const float ASTEROID_ROTATION_COOLDOWN = 0.01;
const float ASTEROID_MOVEMENT_SPEED = 0.01;
const float ASTEROID_ROTATION_SPEED = 0.1;
const int ASTEROID_MAX = 1;
const float ASTEROID_SCALE = 0.00035;

const int asteroidTypes = 1;

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
    AABB asteroidHitbox();

}

#endif