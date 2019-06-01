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

const float ASTEROID_SPAWN_COOLDOWN = 0.5;
const float ASTEROID_MOVEMENT_COOLDOWN = 0.01;
const float ASTEROID_ROTATION_COOLDOWN = 0.01;
const float ASTEROID_MOVEMENT_SPEED = 0.03;
const float ASTEROID_ROTATION_SPEED = 0.1;
const int ASTEROID_MAX = 30;
const float ASTEROID_SCALE = 0.00035;

/*
*   Removed multiple kinds of asteroids
*   Maybe will be added again in a future version
*/
const int asteroidTypes = 1;

namespace Asteroids {
    class Asteroid {
        private:
            Shader shader;

        public:
            float speed = ASTEROID_MOVEMENT_SPEED;
            float angle = 0;    // radians
            float rotation = 0; // radians
            float lastMovementTimestamp = 0;
            float lastRotationTimestamp = 0;
            double xOffSet = 0;
            double yOffSet = 0;

            Asteroid(float width, float height, int difficulty, Camera camera);
            // For Collision Testing
            Asteroid(float width, float height, int difficulty, Camera camera, float angle);

            ~Asteroid();

            void init();
            bool render(float width, float height, Camera camera);
            void dislocateAsteroid();
            void rotateAsteroid();
            void generateCoordinates();
    };

    void loadModel(string modelPath);

    vector<Asteroid*> * getAsteroids();
    void destroyAll();

    void renderAsteroids(float width, float height, int difficulty, Camera camera);
    void renderTest(float width, float height, int difficulty, Camera camera, float angle);
    bool readyToSpawn();
    AABB asteroidHitbox();
}

#endif