#ifndef SPACE_SHIP_H
#define SPACE_SHIP_H

#include "shader.hpp"
#include "camera.hpp"
#include "model.hpp"

namespace SpaceShips {

    void loadModel(string modelPath);

    class SpaceShip {
        private:
            Shader shader;

            float yOffSet, xOffSet = yOffSet = 0.0f;
            float angle = glm::radians(90.0);
            float speed = 0.025f;
            float rotation_speed = 0.07; // radians

            glm::mat4* modelMatrix = new glm::mat4;

        public:
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