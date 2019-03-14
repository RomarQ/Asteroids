#ifndef SPACE_SHIP_H
#define SPACE_SHIP_H

#include "shader.hpp";

class SpaceShip
{
    private:
        Shader spaceShip;
        float yOffSet, xOffSet = yOffSet = 0.0f;
        float angle = 0.0f;
        float speed = 0.005f;

    public:
        SpaceShip(const char* vertexShaser, const char* fragmentShader);

        void moveForward();
        void use();
};

#endif