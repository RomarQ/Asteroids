#include "spaceShip.hpp"
#include "shader.hpp"

#include <iostream>

// Constructor
SpaceShip::SpaceShip(const char* vertexShaser, const char* fragmentShader) : shader(Shader(vertexShaser, fragmentShader))
{
    shader.setFloat("yOffSet", 1.0f);
}

void SpaceShip::moveForward()
{
    if(yOffSet > 1.0f)
    {
      yOffSet = -1.0f;
    } else if(yOffSet < -1.0f)
    {
      yOffSet = 1.0f;
    } else {
      yOffSet += speed;
    }
    
    std::cout << yOffSet << std::endl;
    shader.setFloat("yOffset", yOffSet);
}

void SpaceShip::moveBackward()
{
    if(yOffSet > 1.0f)
    {
      yOffSet = -1.0f;
    } else if(yOffSet < -1.0f)
    {
      yOffSet = 1.0f;
    } else {
      yOffSet -= speed;
    }
    
    std::cout << yOffSet << std::endl;
    shader.setFloat("yOffset", yOffSet);
}

void SpaceShip::use() const
{ 
    shader.use();
}