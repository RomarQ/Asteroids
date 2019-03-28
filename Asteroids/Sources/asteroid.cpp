#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <GLFW/glfw3.h>

#include "asteroid.hpp"

namespace Asteroids {
    /*
    * Where Asteroids will be stored
    */
    vector<Asteroid*> asteroids;

    /*
    * Global Model, that is used by all Asteroid Objects
    */
    Model *model;

    /*
    * Timestamp in seconds with 6 decimal units (0.000000)
    * This timestamp allows to know when to spawn a new asteroid
    */
    float lastAsteroidTimestamp = 0;

    /*
    * max x,z coordinates based on screen ration
    */
    float max_X = 4.5;
    float max_Y = 2.5;
    float radius = 0.12;

    Asteroid::Asteroid(float width, float height, Camera camera) : 
        shader("../Asteroids/Shaders/asteroid.vs", "../Asteroids/Shaders/asteroid.fs"),
        angle(((double) rand() / RAND_MAX) * 360.0f)
    {
        generateCoordinates();
        //
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1, 0.1, 0.1));

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

            shader.setInt("texture_diffuse1", 0);

            glBindVertexArray(0);
        }
    }

    Asteroid::~Asteroid()
    {
        delete model;
        for (Asteroid *a : asteroids) delete a;
    }

    bool Asteroid::render(float width, float height, Camera camera)
    {
        // activate shader
        shader.use();

        // Move asteroid according to the angle
        dislocateAsteroid();

        // configure transformation matrices
        glm::mat4 projection = glm::perspective(glm::radians(45.f), width / height, 0.1f, 1000.0f);
        projection = glm::translate(projection, glm::vec3(xOffSet, yOffSet, 0.0f));
        projection = glm::rotate(projection, angle+glm::radians((rotation += rotationSpeed)), glm::vec3(0.0, 0.0, 1.0));
        projection = glm::translate(projection, glm::vec3(-xOffSet, -yOffSet, 0.0f));
        projection = glm::translate(projection, glm::vec3(xOffSet, yOffSet, 0.0f));
        glm::mat4 view = glm::lookAt(
            glm::vec3(0.0f, 6.0f, 0.0f), // Looking from top
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 1.0f)
        );
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        if(!camera.isInsideFrustum(projection * view)) {
            return false;
        }

        //std::cout << "is inside: " << camera.isInsideFrustum(projection * view) << std::endl;

        // draw asteroid
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, model->textures_loaded[0].id);
        for (unsigned int i = 0; i < model->meshes.size(); i++)
        {
            glBindVertexArray(model->meshes[i].VAO);
            glDrawElements(GL_TRIANGLES, model->meshes[i].indices.size(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }

        return true;
    }

    void Asteroid::dislocateAsteroid()
    {
        float radians = glm::radians(angle);

        if (angle >= 0 && angle <= 90.f) {
            xOffSet -= cos(radians) * ASTEROID_SPEED;
            yOffSet -= sin(radians) * ASTEROID_SPEED;
        }
        else if (angle > 90.f && angle <= 180.f) {
            xOffSet += cos(radians) * -ASTEROID_SPEED;
            yOffSet -= sin(radians) * ASTEROID_SPEED;
        }
        else if (angle > 180.f && angle <= 270.f) {
            xOffSet += cos(radians) * -ASTEROID_SPEED;
            yOffSet += sin(radians) * -ASTEROID_SPEED;
        }
        else {
            xOffSet -= cos(radians) * ASTEROID_SPEED;
            yOffSet += sin(radians) * ASTEROID_SPEED;
        }
    }

    void loadModel(string modelPath) {
        Asteroids::model = new Model(modelPath);
    }

    void renderAsteroids(float width, float height, Camera camera)
    {
        if(Asteroids::asteroids.size() < ASTEROID_MAX && readyToSpawn())
        {
            lastAsteroidTimestamp = glfwGetTime();
            Asteroids::asteroids.push_back(new Asteroid(width, height, camera));
        }

        for(unsigned int i = 0; i < Asteroids::asteroids.size(); i++)
        {
            Asteroid *asteroid = Asteroids::asteroids.at(i);
            //std::cout << asteroid->xOffSet << "  " << asteroid->yOffSet << std::endl;
            if(!asteroid->render(width, height, camera)) {
                asteroids.erase(asteroids.begin()+i);
            }
            CheckAsteroidCollisions();
        }
    }

    bool readyToSpawn()
    {
        return (glfwGetTime() > lastAsteroidTimestamp+ASTEROID_COOLDOWN);
    }

    void Asteroid::generateCoordinates()
    {
        double random = ((double) rand() / RAND_MAX);
        std::cout << angle << std::endl;
        if (angle >= 0 && angle <= 90.f) {
            if (angle > 45.f) {
                yOffSet = max_Y;
                xOffSet = random * max_X;
            }
            else {
                yOffSet = random * max_Y;
                xOffSet = max_X;
            }
        }
        else if (angle > 90.f && angle <= 180.f) {
            if (angle > 135.f) {
                yOffSet = random * max_Y;
                xOffSet = -max_X;
            }
            else {
                yOffSet = max_Y;
                xOffSet = random * -max_X;
            }
        }
        else if (angle > 180.f && angle <= 270.f) {
            if (angle > 225.f) {
                yOffSet = -max_Y;
                xOffSet = random * -max_X;
            }
            else {
                yOffSet = random * -max_Y;
                xOffSet = -max_X;
            }
        }
        else {
            if (angle > 315.f) {
                yOffSet = random * -max_Y;
                xOffSet = max_X;
            }
            else {
                yOffSet = -max_Y;
                xOffSet = random * max_X;
            }
        }
    }

    void CheckAsteroidCollisions()
    {   
        /*
        *       y
        *   | 1 | 0 |
        *   ----+-----> x
        *   | 2 | 3 |
        */
        glm::vec2 quadrants[4] = {
            glm::vec2(radius, radius),
            glm::vec2(-radius, radius),
            glm::vec2(-radius, -radius),
            glm::vec2(radius, -radius)
        };

        vector<Asteroid*> collisions;

        for (int i = 0; i<asteroids.size(); i++) {
            for (int j = 0; j<asteroids.size(); j++) {
                if (i == j) continue;
                
                if (asteroids[i]->xOffSet > asteroids[j]->xOffSet) {
                    if (asteroids[i]->yOffSet > asteroids[j]->yOffSet) {
                        float iX = asteroids[i]->xOffSet + quadrants[2].x;
                        float iY = asteroids[i]->yOffSet + quadrants[2].y;
                        float jX = asteroids[j]->xOffSet + quadrants[0].x;
                        float jY = asteroids[j]->yOffSet + quadrants[0].y;

                        if(jX > iX && jY > iY) {
                            collisions.push_back(asteroids[i]);
                            collisions.push_back(asteroids[j]);
                        }
                    }
                    else {
                        float iX = asteroids[i]->xOffSet + quadrants[1].x;
                        float iY = asteroids[i]->yOffSet + quadrants[1].y;
                        float jX = asteroids[j]->xOffSet + quadrants[3].x;
                        float jY = asteroids[j]->yOffSet + quadrants[3].y;

                        if(jX > iX && jY < iY) {
                            std::cout << iX << " - " << iY << " | " << jX << " - " << jY << " -> " << "3" << std::endl;
                            collisions.push_back(asteroids[i]);
                            collisions.push_back(asteroids[j]);
                        }
                    }
                }
                else {
                    if (asteroids[i]->yOffSet > asteroids[j]->yOffSet) {
                        float iX = asteroids[i]->xOffSet + quadrants[3].x;
                        float iY = asteroids[i]->yOffSet + quadrants[3].y;
                        float jX = asteroids[j]->xOffSet + quadrants[1].x;
                        float jY = asteroids[j]->yOffSet + quadrants[1].y;

                        if(jX < iX && jY > iY) {
                            collisions.push_back(asteroids[i]);
                            collisions.push_back(asteroids[j]);
                        }  
                    }
                    else {
                        float iX = asteroids[i]->xOffSet + quadrants[0].x;
                        float iY = asteroids[i]->yOffSet + quadrants[0].y;
                        float jX = asteroids[j]->xOffSet + quadrants[2].x;
                        float jY = asteroids[j]->yOffSet + quadrants[2].y;

                        if(jX < iX && jY < iY) {
                            collisions.push_back(asteroids[i]);
                            collisions.push_back(asteroids[j]);
                        }
                    }
                }
                
                for (Asteroid *collision : collisions) {
                    for (int i=0; i<asteroids.size(); i++) {
                        if (asteroids[i] == collision) {
                            asteroids.erase(asteroids.begin()+i);
                        }
                    }
                }
            }
        }
    }
}