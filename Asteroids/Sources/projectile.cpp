#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <GLFW/glfw3.h>

#include "projectile.hpp"

namespace Projectiles {
    /*
    * Where Projectiles will be stored
    */
    vector<Projectile*> projectiles;

    /*
    * Global Model, that is used by all Projectile Objects
    */
    Model *model;

    /*
    * Timestamp in seconds with 6 decimal units (0.000000)
    * This timestamp allows to know when a projectile can be fired
    */
    float lastProjectileTimestamp = 0;

    Projectile::Projectile(float width, float height, float x, float y, float angle) : 
        shader("../Asteroids/Shaders/asteroid.vs", "../Asteroids/Shaders/asteroid.fs"),
        xOffSet(x),
        yOffSet(y),
        angle(angle)
    {
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05, 0.05, 0.05));

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

    Projectile::~Projectile()
    {
        delete model;
        for (Projectile *p : projectiles) delete p;
    }

    bool Projectile::render(float width, float height)
    {
        // activate shader
        shader.use();

        // Move projectile according to the angle
        dislocateProjectile();

        // configure transformation matrices
        glm::mat4 projection = glm::perspective(glm::radians(45.f), width / height, 0.1f, 1000.0f);
        projection = glm::translate(projection, glm::vec3(xOffSet, yOffSet, 0.0f));

        glm::mat4 view = glm::lookAt(
            glm::vec3(0.0f, 6.0f, 0.0f), // Looking from top
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 1.0f)
        );
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

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

    void Projectile::dislocateProjectile()
    {
        xOffSet += cos(angle) * PROJECTILE_SPEED;
        yOffSet += sin(angle) * PROJECTILE_SPEED;

        //adjustOffSets();
        
        shader.setFloat("xOffset", xOffSet);
        shader.setFloat("yOffset", yOffSet);
    }

    void loadModel(string modelPath) {
        model = new Model(modelPath);
    }

    bool fireProjectile(float width, float height, float x, float y, float angle)
    {
        if (readyToFire()) {
            lastProjectileTimestamp = glfwGetTime();
            projectiles.push_back(new Projectile(width, height, x, y, angle));
        }

        return false;
    }

    void renderProjectiles(float width, float height)
    {
        for(unsigned int i = 0; i < projectiles.size(); i++)
        {
            Projectile *projectile = projectiles.at(i);
            //std::cout << asteroid->xOffSet << "  " << asteroid->yOffSet << std::endl;
            if(!projectile->render(width, height)) {
                projectiles.erase(projectiles.begin()+i);
            }
        }
    }

    bool readyToFire()
    {
        return (glfwGetTime() > lastProjectileTimestamp+PROJECTILE_COOLDOWN);
    }
}