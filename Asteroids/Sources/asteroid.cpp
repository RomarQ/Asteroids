#include "asteroid.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <GLFW/glfw3.h>

#include <time.h>
#include <stb_image.h>

Asteroid::Asteroid() : 
    shader("../Asteroids/Shaders/asteroid.vs", "../Asteroids/Shaders/asteroid.fs"),
    objectModel("../Asteroids/Objects/asteroid/rock.obj")
{
    srand(time(NULL)); // initialize random seed
    angle = rand() * 360.0f;
    xOffSet = cos(angle) * 20.f;
    yOffSet = sin(angle) * 20.f;

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(xOffSet, yOffSet, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-xOffSet, -yOffSet, 0.0f));

    // 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
    float rotAngle = (rand() % 360);
    modelMatrix = glm::rotate(modelMatrix, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

    // configure instanced array
    // -------------------------
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4), &modelMatrix, GL_STATIC_DRAW);

    for (unsigned int i = 0; i < objectModel.meshes.size(); i++)
    {
        unsigned int VAO = objectModel.meshes[i].VAO;
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

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        shader.setInt("texture_diffuse1", 0);

        glBindVertexArray(0);
    }
}

void Asteroid::render(float width, float height, Camera camera)
{
    // activate shader
    shader.use();
    // Move asteroid according to the angle
    dislocateAsteroid();
    shader.setFloat("xOffset", xOffSet);
    shader.setFloat("yOffset", yOffSet);

    // configure transformation matrices
    glm::mat4 projection = glm::perspective(glm::radians(rand() * 10.0f + 20.0f), width / height, 0.1f, 1000.0f);
    glm::mat4 view = camera.GetViewMatrix();
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);

/*  glm::mat4 projection2 = projection * view;

    glm::vec4 rowX = glm::row(projection2, 0);
    glm::vec4 rowY = glm::row(projection2, 1);
    glm::vec4 rowZ = glm::row(projection2, 2);
    glm::vec4 rowW = glm::row(projection2, 3);

    glm::vec4 planes[6];
    planes[0] = normalize(rowW + rowX);
    planes[1] = normalize(rowW - rowX);
    planes[2] = normalize(rowW + rowY);
    planes[3] = normalize(rowW - rowY);
    planes[4] = normalize(rowW + rowZ);
    planes[5] = normalize(rowW - rowZ);

    
    for(int i = 0; i<6; i++) {
        // Normalizing the planes.

        glm::vec3 normal(planes[i].x, planes[i].y, planes[i].z);
        float length = glm::length(normal);
        planes[i] = -planes[i] / length; // Notice the negation. I don't know why I needed that!!
    }



    // Sphere intersection test.

    for (glm::vec4 plane : planes)
    {
        float dist = plane.x * 0 + plane.y * 0 + plane.z * 0 + plane.w - 20;
        if (dist > 0) std::cout << "true " << dist << std::endl;
        else std::cout << "false " << dist << std::endl;
    } */

    //std::cout << " x: "<<xOffSet<<" y: "<<yOffSet<<" - "<< planes[0].x << std::endl;

    // draw asteroid
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, objectModel.textures_loaded[0].id);
    for (unsigned int i = 0; i < objectModel.meshes.size(); i++)
    {
        glBindVertexArray(objectModel.meshes[i].VAO);
        glDrawElementsInstanced(GL_TRIANGLES, objectModel.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, 1);
        glBindVertexArray(0);
    }
}

void Asteroid::dislocateAsteroid() {
    xOffSet += cos(angle) * asteroidSpeed;
    yOffSet += sin(angle) * asteroidSpeed;
}

void Asteroid::renderAsteroids(vector<Asteroid*> *asteroids, float width, float height, Camera camera)
{
    int totalAsteroids2 = asteroids->size();

    if(totalAsteroids2 < maxAsteroids)
    {
        //totalAsteroids += asteroidCoolDown;
        //std::cout << totalAsteroids2 << " " << totalAsteroids << std::endl;;
        asteroids->push_back(new Asteroid());
    }

    for(int i = 0; i < totalAsteroids2; i++)
    {
        Asteroid *asteroid = asteroids->at(i);
        if(asteroid->xOffSet < -20.f || asteroid->yOffSet > 33.f) 
        {
            delete asteroid;
            asteroids->erase(asteroids->begin()+i);
            totalAsteroids2--;
        }
        else asteroid->render(width, height, camera);
    }
}