#ifndef SPACE_SHIP_H
#define SPACE_SHIP_H

#include "shader.hpp"

class SpaceShip
{
    private:
        float yOffSet, xOffSet = yOffSet = 0.0f;
        float angle = 0.0f;
        float speed = 0.005f;

        unsigned int VBO, VAO, EBO;
        unsigned int texture1, texture2;

    public:
        Shader shader;

        SpaceShip(const char* vertexShaser, const char* fragmentShader);
        ~SpaceShip();

        void render();

        void moveForward();
        void moveBackward();
        void use() const;
};

#endif