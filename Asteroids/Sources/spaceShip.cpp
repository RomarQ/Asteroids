#include "spaceShip.hpp"
#include "shader.hpp"

#include <iostream>

// Constructor
SpaceShip::SpaceShip(const char* vertexShaser, const char* fragmentShader) : spaceShip(Shader(vertexShaser, fragmentShader))
{
    spaceShip.setFloat("yOffSet", 1.0f);
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
    spaceShip.setFloat("yOffset", yOffSet);
}

void SpaceShip::use() 
{ 
    spaceShip.use();
}