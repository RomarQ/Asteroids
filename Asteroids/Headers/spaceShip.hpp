#ifndef SPACE_SHIP_H
#define SPACE_SHIP_H

#include "shader.hpp"

class SpaceShip
{
    private:

        float vertices[24] = {
            // positions            // colors           // texture coords
            0.0f, 0.03f,  0.0f,     0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
            0.0f, -0.03f, 0.0f,     0.0f, 0.0f, 1.0f,   0.0f, 0.3f,   // bottom left
            0.1f, 0.0f,   0.0f,     1.0f, 1.0f, 0.0f,   0.5f, 1.0f    // top
        };

        unsigned int indices[3] = {
            0, 1, 2
        };

        Shader shader;

        float yOffSet, xOffSet = yOffSet = 0.0f;
        float angle = glm::radians(90.0f); // radians
        float speed = 0.015f;
        float rotation_speed = 0.07; // radians

        unsigned int VBO, VAO, EBO;
        unsigned int texture1, texture2;

    public:
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