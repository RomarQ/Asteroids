#ifndef SPACE_SHIP_H
#define SPACE_SHIP_H

#include "shader.hpp"

#include <glm/glm.hpp>

class SpaceShip
{
    private:
        float yOffSet, xOffSet = yOffSet = 0.0f;
        float angle = glm::radians(90.0f); // radians
        float speed = 0.015f;
        float rotation_speed = 0.07; // radians

        unsigned int VBO, VAO, EBO;
        unsigned int texture1, texture2;

    public:
        Shader shader;

        SpaceShip(const char* vertexShaser, const char* fragmentShader);
        ~SpaceShip();

        void render();

        void moveForward();
        void moveBackward();
        void rotateLeft();
        void rotateRight();
        void adjustOffSets();
        void use() const;
};

#endif