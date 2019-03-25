#ifndef ASTEROID_H
#define ASTEROID_H

#include "shader.hpp"
#include "model.hpp"
#include "camera.hpp"

const float asteroidSpeed = 0.03;
const int maxAsteroids = 10;
const float asteroidCoolDown = 0.05;

//float totalAsteroids = 0;

class Asteroid {
    private:
        Shader shader;
        Model objectModel;

        float angle = 0;
        float xOffSet = 0, yOffSet = 0;

        glm::mat4* modelMatrix = new glm::mat4;

        unsigned int texture;

    public:
        Asteroid();

        void render(float width, float height, Camera camera);
        void dislocateAsteroid();

        static void renderAsteroids(vector<Asteroid*> *asteroids, float width, float height, Camera camera);
};

#endif