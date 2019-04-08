#include "spaceShip.hpp"
#include "shader.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb_image.h>

#include <iostream>

namespace SpaceShips {
    /*
        * Ship Model
        */
    Model *model;

    //
    float max_X = 4.5;
    float max_Y = 2.5;

    // Constructor
    SpaceShip::SpaceShip(const char* vertexShader, const char* fragmentShader) : shader(vertexShader, fragmentShader)
    {
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.006, 0.006, 0.006));

        /*
                * Buffer configuration
                */
        unsigned int buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4), &modelMatrix, GL_STATIC_DRAW);

        for (unsigned int i = 0; i < model->meshes.size(); i++)
        {
            unsigned int VAO = model->meshes[i].VAO;
            glBindVertexArray(VAO);
            // set attribute pointers for matrix (4 times vec4)
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
            glEnableVertexAttribArray(5);
            glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
            glEnableVertexAttribArray(6);
            glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glVertexAttribDivisor(3, 1);
            glVertexAttribDivisor(4, 1);
            glVertexAttribDivisor(5, 1);
            glVertexAttribDivisor(6, 1);

            glBindVertexArray(0);
        }
    }

    SpaceShip::~SpaceShip()
    {
        delete model;
        delete modelMatrix;
    }

    void SpaceShip::render(float width, float height, Camera camera)
    {
        shader.use();

        // configure transformation matrices
        glm::mat4 projection = glm::perspective(glm::radians(45.f), width / height, 0.1f, 1000.0f);
        projection = glm::translate(projection, glm::vec3(xOffSet, yOffSet, 0.0f));
        projection = glm::rotate(projection, angle+glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
        projection = glm::translate(projection, glm::vec3(-xOffSet, -yOffSet, 0.0f));
        projection = glm::translate(projection, glm::vec3(xOffSet, yOffSet, 0.0f));
        glm::mat4 view = glm::lookAt(
            glm::vec3(0.0f, 6.0f, 0.0f), // Looking from top
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 1.0f)
        );
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        // draw asteroid
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, model->textures_loaded[0].id);
        for (unsigned int i = 0; i < model->meshes.size(); i++)
        {
            glBindVertexArray(model->meshes[i].VAO);
            glDrawElements(GL_TRIANGLES, model->meshes[i].indices.size(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }
    }

    void SpaceShip::moveForward()
    { 
        xOffSet += cos(angle) * speed;
        yOffSet += sin(angle) * speed;

        adjustOffSets();
        
        shader.setFloat("xOffset", xOffSet);
        shader.setFloat("yOffset", yOffSet);
    }

    void SpaceShip::moveBackward()
    {
        xOffSet -= cos(angle) * speed;
        yOffSet -= sin(angle) * speed;
    
        adjustOffSets();
        
        shader.setFloat("xOffset", xOffSet);
        shader.setFloat("yOffset", yOffSet);
    }

    void SpaceShip::adjustOffSets()
    {
        if(xOffSet > max_X || xOffSet < -max_X)
        {
            xOffSet *= -1.0f;
        }

        if(yOffSet > max_Y || yOffSet < -max_Y)
        {
            yOffSet *= -1.0f;
        }
    }

    void SpaceShip::rotateLeft()
    {
        angle += rotation_speed;
    }

    void SpaceShip::rotateRight()
    {
        angle -= rotation_speed;
    }

    void SpaceShip::use() const
    { 
        shader.use();
    }

    void loadModel(string modelPath) {
        model = new Model(modelPath);
    }
}