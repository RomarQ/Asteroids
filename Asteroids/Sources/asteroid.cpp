#include "asteroid.hpp"

namespace Asteroids {
    /*
    * Where Asteroids will be stored
    */
    vector<Asteroid*> asteroids;

    /*
    * Timestamp in seconds with 6 decimal units (0.000000)
    * This timestamp allows to know when to spawn a new asteroid
    */
    float lastAsteroidTimestamp = 0;
    /*
    *   Global Model, that is used by all Asteroid Objects
    */
    Model *model;
    /*
    *   HitBox used for collision
    */
    AABB hitBox;
    /*
    * max x,z coordinates based on screen ratio
    */
    float max_X = 4.7;
    float max_Y = 2.9;

    Asteroid::Asteroid(float width, float height, int difficulty, Camera camera) : 
        shader("../Asteroids/Shaders/asteroid.vs", "../Asteroids/Shaders/asteroid.fs"),
        angle(glm::radians(((double) rand() / RAND_MAX) * 360.0f)),
        speed(ASTEROID_MOVEMENT_SPEED * difficulty)
    {
        init();
    }

    Asteroid::Asteroid(float width, float height, int difficulty, Camera camera, float angle) : 
        shader("../Asteroids/Shaders/asteroid.vs", "../Asteroids/Shaders/asteroid.fs"),
        angle(angle),
        speed(ASTEROID_MOVEMENT_SPEED * difficulty)
    {
        init();
    }
    
    Asteroid::~Asteroid() {
        delete model;
    }

    void Asteroid::init() {
        generateCoordinates();
        shader.use();
        glm::mat4 modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(ASTEROID_SCALE, ASTEROID_SCALE, ASTEROID_SCALE));
        shader.setMat4("modelMatrix", modelMatrix);

        /*
        *   Buffer configuration
        */
        unsigned int buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4), &modelMatrix, GL_STATIC_DRAW);

        unsigned int VAO = model->meshes.at(0).VAO;
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

    bool Asteroid::render(float width, float height, Camera camera)
    {
        // activate shader
        shader.use();

        // Move asteroid according to the angle
        dislocateAsteroid();
        rotateAsteroid();

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

        if(!camera.isInsideFrustum(projection * view)) {
            return false;
        }

        // Draw asteroid
        model->Draw(shader);

        return true;
    }

    void Asteroid::rotateAsteroid()
    {
        if ((lastRotationTimestamp+ASTEROID_ROTATION_COOLDOWN) > glfwGetTime()) return;

        lastRotationTimestamp = glfwGetTime();
        rotation = rotation >= glm::pi<float>()*2 ? .0f : rotation + ASTEROID_ROTATION_SPEED;
    }

    void Asteroid::dislocateAsteroid()
    {
        if ((lastMovementTimestamp+ASTEROID_MOVEMENT_COOLDOWN) > glfwGetTime())
            return;
        
        lastMovementTimestamp = glfwGetTime();

        xOffSet += glm::cos(angle) * ASTEROID_MOVEMENT_SPEED;
        yOffSet += glm::sin(angle) * ASTEROID_MOVEMENT_SPEED;
    }

    void loadModel(string modelPath) {
        model = new Model(modelPath);
        hitBox = getAABB(model->meshes.at(0), ASTEROID_SCALE);
    }

    AABB asteroidHitbox() {
        return hitBox;
    }

    void renderAsteroids(float width, float height, int difficulty, Camera camera)
    {
        if(Asteroids::asteroids.size() < ASTEROID_MAX && readyToSpawn())
        {
            lastAsteroidTimestamp = glfwGetTime();
            Asteroids::asteroids.push_back(new Asteroid(width, height, difficulty, camera));
        }

        for(unsigned int i = 0; i < asteroids.size(); i++)
        {
            Asteroid *asteroid = asteroids.at(i);
            if(!asteroid->render(width, height, camera)) {
                asteroids.erase(asteroids.begin()+i);
            }
        }
    }

    void renderTest(float width, float height, int difficulty, Camera camera, float angle)
    {
        if(Asteroids::asteroids.size() == 0)
        {
            Asteroids::asteroids.push_back(new Asteroid(width, height, difficulty, camera, angle));
            Asteroids::asteroids.push_back(new Asteroid(width, height, difficulty, camera, angle+(glm::pi<float>())));

            Asteroids::asteroids.push_back(new Asteroid(width, height, difficulty, camera, angle+=(glm::pi<float>()/4)));
            Asteroids::asteroids.push_back(new Asteroid(width, height, difficulty, camera, angle+(glm::pi<float>())));

            Asteroids::asteroids.push_back(new Asteroid(width, height, difficulty, camera, angle+=(glm::pi<float>()/4)));
            Asteroids::asteroids.push_back(new Asteroid(width, height, difficulty, camera, angle+(glm::pi<float>())));

            Asteroids::asteroids.push_back(new Asteroid(width, height, difficulty, camera, angle+=(glm::pi<float>()/4)));
            Asteroids::asteroids.push_back(new Asteroid(width, height, difficulty, camera, angle+(glm::pi<float>())));
        }

        for(unsigned int i = 0; i < asteroids.size(); i++)
        {
            Asteroid *asteroid = asteroids.at(i);
            if(!asteroid->render(width, height, camera)) {
                asteroids.erase(asteroids.begin()+i);
            }
        }
    }

    bool readyToSpawn()
    {
        return (glfwGetTime() > lastAsteroidTimestamp+ASTEROID_SPAWN_COOLDOWN);
    }

    void Asteroid::generateCoordinates()
    {       
        xOffSet = glm::cos(angle)*max_X;
        yOffSet = glm::sin(angle)*max_Y;
        angle += glm::pi<float>();
    }

    vector<Asteroid*> * getAsteroids()
    {
        return &asteroids;
    }

    void destroyAll()
    {   
        asteroids.clear();
    }
}