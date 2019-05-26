#ifndef SPACE_SHIP_H
#define SPACE_SHIP_H

#include "shader.hpp"
#include "camera.hpp"
#include "model.hpp"
#include <GLFW/glfw3.h>

const float SHIP_MOVEMENT_COOLDOWN = 0.01f;
const float SHIP_ROTATION_COOLDOWN = 0.01f;
const float SHIP_MOVEMENT_SPEED = 0.04f;
const float SHIP_ROTATION_SPEED = 0.1f;

namespace SpaceShips {

    void loadModel(string modelPath);

    class SpaceShip {
        private:
            Shader shader;

            float speed = SHIP_MOVEMENT_SPEED;
            float rotationSpeed = SHIP_ROTATION_SPEED;
            float lastMovementTimestamp = 0;
            float lastRotationTimestamp = 0;

            glm::mat4* modelMatrix = new glm::mat4;

        public:
            float yOffSet, xOffSet = yOffSet = 0.0f;
            float angle = glm::radians(90.0);

            SpaceShip(const char* vertexShaser, const char* fragmentShader);
            ~SpaceShip();

            void render(float width, float height, Camera camera);

            void moveForward();
            void moveBackward();
            void rotateLeft();
            void rotateRight();
            void adjustOffSets();
            void use() const;
    };
}
#endif