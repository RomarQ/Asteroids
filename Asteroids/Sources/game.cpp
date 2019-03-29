#include "game.hpp"

namespace Asteroids {

    btDefaultCollisionConfiguration* collisionConfiguration;
    btCollisionDispatcher* dispatcher;
    btBroadphaseInterface* overlappingPairCache;
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamicWorld;

    float Width, Height;

    Camera camera;

    float lastX = 0;
    float lastY = 0;
    bool firstMouse = true;

    // timing
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    /*
    *   Object radius
    */
    float asteroidRadius = 0.14;

    Game::Game(float width, float height, GLFWwindow* window) : 
        State(GAME_ACTIVE)
    {
        collisionConfiguration = new btDefaultCollisionConfiguration();
        dispatcher = new btCollisionDispatcher(collisionConfiguration);
        overlappingPairCache = new btDbvtBroadphase();
        solver = new btSequentialImpulseConstraintSolver;
        dynamicWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

        dynamicWorld->setGravity(btVector3(0.0, -10.0, 0.0));
        
        Width = width;
        Height = height;
        camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f));

        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);

        // tell GLFW to capture our mouse
        //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    Game::~Game()
    {

    }

    void Game::Init()
    {
        Asteroids::loadModel("../Asteroids/Objects/asteroid/asteroid.obj");
        SpaceShips::loadModel("../Asteroids/Objects/spaceship/spaceship.obj");
        Projectiles::loadModel("../Asteroids/Objects/projectile/projectile.obj");

        // build and compile the Space Ship
        // ------------------------------------
        spaceShip = new SpaceShip("../Asteroids/Shaders/space_ship.vs", "../Asteroids/Shaders/space_ship.fs");
    }

    void Game::Update(float dt)
    {

    }

    void Game::Render()
    {
        Game::checkProjectileCollisions();

        spaceShip->render(Width, Height, camera);
        Asteroids::renderAsteroids(Width, Height, camera);
        Projectiles::renderProjectiles(Width, Height);
    }

    void Game::ProcessInput(GLFWwindow *window)
    {
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, true);
        } 
        if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        {   
            spaceShip->moveForward();
        }
        if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        {   
            spaceShip->moveBackward();
        }
        if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        {   
            spaceShip->rotateLeft();
        }
        if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        {   
            spaceShip->rotateRight();
        }
        if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {   
            Projectiles::fireProjectile(Width, Height, spaceShip->xOffSet, spaceShip->yOffSet, spaceShip->angle);
        }

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.ProcessKeyboard(FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.ProcessKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.ProcessKeyboard(RIGHT, deltaTime);

        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    }

    void Game::checkProjectileCollisions()
    {
        /*
        *       y
        *   | 1 | 0 |
        *   ----+-----> x
        *   | 2 | 3 |
        */
        glm::vec2 quadrants[4] = {
            glm::vec2(asteroidRadius, asteroidRadius),
            glm::vec2(-asteroidRadius, asteroidRadius),
            glm::vec2(-asteroidRadius, -asteroidRadius),
            glm::vec2(asteroidRadius, -asteroidRadius)
        };

        vector<Projectile*> *projectiles = Projectiles::getProjectiles();
        vector<Asteroid*> *asteroids = Asteroids::getAsteroids();

        vector<Asteroid*> aCollisions;
        vector<Projectile*> pCollisions;

        for (int i = 0; i<projectiles->size(); i++) {
            for (int j = 0; j<asteroids->size(); j++) {
                //std::cout << projectiles->at(i)->xOffSet << " " << asteroids->at(j)->xOffSet << " " << projectiles->at(i)->yOffSet << " " << asteroids->at(j)->yOffSet << std::endl;

                if (projectiles->at(i)->xOffSet > asteroids->at(j)->xOffSet) {
                    if (projectiles->at(i)->yOffSet > asteroids->at(j)->yOffSet) {
                        float iX = projectiles->at(i)->xOffSet + quadrants[2].x;
                        float iY = projectiles->at(i)->yOffSet + quadrants[2].y;
                        float jX = asteroids->at(j)->xOffSet + quadrants[0].x;
                        float jY = asteroids->at(j)->yOffSet + quadrants[0].y;

                        if(jX > iX && jY > iY) {
                            pCollisions.push_back(projectiles->at(i));
                            aCollisions.push_back(asteroids->at(j));
                        }
                    }
                    else {
                        float iX = projectiles->at(i)->xOffSet + quadrants[1].x;
                        float iY = projectiles->at(i)->yOffSet + quadrants[1].y;
                        float jX = asteroids->at(j)->xOffSet + quadrants[3].x;
                        float jY = asteroids->at(j)->yOffSet + quadrants[3].y;

                        if(jX > iX && jY < iY) {
                            pCollisions.push_back(projectiles->at(i));
                            aCollisions.push_back(asteroids->at(j));
                        }
                    }
                }
                else {
                    if (projectiles->at(i)->yOffSet > asteroids->at(j)->yOffSet) {
                        float iX = projectiles->at(i)->xOffSet + quadrants[3].x;
                        float iY = projectiles->at(i)->yOffSet + quadrants[3].y;
                        float jX = asteroids->at(j)->xOffSet + quadrants[1].x;
                        float jY = asteroids->at(j)->yOffSet + quadrants[1].y;

                        if(jX < iX && jY > iY) {
                            pCollisions.push_back(projectiles->at(i));
                            aCollisions.push_back(asteroids->at(j));
                        }  
                    }
                    else {
                        float iX = projectiles->at(i)->xOffSet + quadrants[0].x;
                        float iY = projectiles->at(i)->yOffSet + quadrants[0].y;
                        float jX = asteroids->at(j)->xOffSet + quadrants[2].x;
                        float jY = asteroids->at(j)->yOffSet + quadrants[2].y;

                        if(jX < iX && jY < iY) {
                            pCollisions.push_back(projectiles->at(i));
                            aCollisions.push_back(asteroids->at(j));
                        }
                    }
                }
            }
        }

        for (Projectile *collision : pCollisions) {
            for (int i=0; i<projectiles->size(); i++) {
                if (projectiles->at(i) == collision) {
                    projectiles->erase(projectiles->begin()+i);
                    break;
                }
            }
        }

        for (Asteroid *collision : aCollisions) {
            for (int i=0; i<asteroids->size(); i++) {
                if (asteroids->at(i) == collision) {
                    asteroids->erase(asteroids->begin()+i);
                    break;
                }
            }
        }
    }

    // glfw: whenever the mouse moves, this callback is called
    // -------------------------------------------------------
    void mouse_callback(GLFWwindow* window, double xpos, double ypos)
    {
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        camera.ProcessMouseMovement(xoffset, yoffset);
    }

    // glfw: whenever the mouse scroll wheel scrolls, this callback is called
    // ----------------------------------------------------------------------
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
        camera.ProcessMouseScroll(yoffset);
    }

    // glfw: whenever the window size changed (by OS or user resize) this callback function executes
    // ---------------------------------------------------------------------------------------------
    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        // make sure the viewport matches the new window dimensions; note that width and 
        // height will be significantly larger than specified on retina displays.
        glViewport(0, 0, width, height);
        Width = width;
        Height = height;
    }
}