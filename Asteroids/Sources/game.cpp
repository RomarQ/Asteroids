#include "game.hpp"

namespace Games {
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
    float projectileRadius = 0.09;
    float shipRadius = 0.15;

    /*
        *   Collision Points of the obejct
        * 
        *       y
        *   | 1 | 0 |
        *   ----+-----> x
        *   | 2 | 3 |
        */
    glm::vec2 projectileQuadrants[4] = {
        glm::vec2(projectileRadius, projectileRadius),
        glm::vec2(-projectileRadius, projectileRadius),
        glm::vec2(-projectileRadius, -projectileRadius),
        glm::vec2(projectileRadius, -projectileRadius)
    };
    glm::vec2 shipQuadrants[4] = {
        glm::vec2(shipRadius, shipRadius),
        glm::vec2(-shipRadius, shipRadius),
        glm::vec2(-shipRadius, -shipRadius),
        glm::vec2(shipRadius, -shipRadius)
    };

    Game::Game(float width, float height, GLFWwindow* window) : 
        State(GAME_ACTIVE),
        score(0.0)
    { 
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
        //std::cout << "Score: " << score << std::endl;
        Game::checkProjectileCollisions();
        Game::checkAsteroidCollisions();

        if(Game::checkShipCollisions()) {
            std::cout << "ACIDENTE" << std::endl;
        }

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

    bool Game::checkShipCollisions()
    {
        vector<Asteroid*> *asteroids = Asteroids::getAsteroids();

        for (int i = 0; i<asteroids->size(); i++) {

            if (spaceShip->xOffSet > asteroids->at(i)->xOffSet) {
                if (spaceShip->yOffSet > asteroids->at(i)->yOffSet) {
                    float sX = spaceShip->xOffSet + shipQuadrants[2].x;
                    float sY = spaceShip->yOffSet + shipQuadrants[2].y;
                    float aX = asteroids->at(i)->xOffSet + ASTEROID_QUADRANTS[0].x * asteroids->at(i)->type;
                    float aY = asteroids->at(i)->yOffSet + ASTEROID_QUADRANTS[0].y * asteroids->at(i)->type;

                    if(aX > sX && aY > sY) return true;
                }
                else {
                    float sX = spaceShip->xOffSet + shipQuadrants[1].x;
                    float sY = spaceShip->yOffSet + shipQuadrants[1].y;
                    float aX = asteroids->at(i)->xOffSet + ASTEROID_QUADRANTS[3].x * asteroids->at(i)->type;
                    float aY = asteroids->at(i)->yOffSet + ASTEROID_QUADRANTS[3].y * asteroids->at(i)->type;

                    if(aX > sX && aY < sY) return true;
                }
            }
            else {
                if (spaceShip->yOffSet > asteroids->at(i)->yOffSet) {
                    float sX = spaceShip->xOffSet + shipQuadrants[3].x;
                    float sY = spaceShip->yOffSet + shipQuadrants[3].y;
                    float aX = asteroids->at(i)->xOffSet + ASTEROID_QUADRANTS[1].x * asteroids->at(i)->type;
                    float aY = asteroids->at(i)->yOffSet + ASTEROID_QUADRANTS[1].y * asteroids->at(i)->type;

                    if(aX < sX && aY > sY) return true;
                }
                else {
                    float sX = spaceShip->xOffSet + shipQuadrants[0].x;
                    float sY = spaceShip->yOffSet + shipQuadrants[0].y;
                    float aX = asteroids->at(i)->xOffSet + ASTEROID_QUADRANTS[2].x * asteroids->at(i)->type;
                    float aY = asteroids->at(i)->yOffSet + ASTEROID_QUADRANTS[2].y * asteroids->at(i)->type;

                    if(aX < sX && aY < sY) return true;
                }
            }
        }

        return false;
    }

    void Game::checkProjectileCollisions()
    {
        vector<Projectile*> *projectiles = Projectiles::getProjectiles();
        vector<Asteroid*> *asteroids = Asteroids::getAsteroids();

        vector<Asteroid*> aCollisions;
        vector<Projectile*> pCollisions;

        for (int i = 0; i<projectiles->size(); i++) {
            for (int j = 0; j<asteroids->size(); j++) {
               
                if (projectiles->at(i)->xOffSet > asteroids->at(j)->xOffSet) {
                    if (projectiles->at(i)->yOffSet > asteroids->at(j)->yOffSet) {
                        float iX = projectiles->at(i)->xOffSet + projectileQuadrants[2].x;
                        float iY = projectiles->at(i)->yOffSet + projectileQuadrants[2].y;
                        float jX = asteroids->at(j)->xOffSet + ASTEROID_QUADRANTS[0].x * asteroids->at(j)->type;
                        float jY = asteroids->at(j)->yOffSet + ASTEROID_QUADRANTS[0].y * asteroids->at(j)->type;

                        if(jX > iX && jY > iY) {
                            pCollisions.push_back(projectiles->at(i));
                            aCollisions.push_back(asteroids->at(j));
                        }
                    }
                    else {
                        float iX = projectiles->at(i)->xOffSet + projectileQuadrants[1].x;
                        float iY = projectiles->at(i)->yOffSet + projectileQuadrants[1].y;
                        float jX = asteroids->at(j)->xOffSet + ASTEROID_QUADRANTS[3].x * asteroids->at(j)->type;
                        float jY = asteroids->at(j)->yOffSet + ASTEROID_QUADRANTS[3].y * asteroids->at(j)->type;

                        if(jX > iX && jY < iY) {
                            pCollisions.push_back(projectiles->at(i));
                            aCollisions.push_back(asteroids->at(j));
                        }
                    }
                }
                else {
                    if (projectiles->at(i)->yOffSet > asteroids->at(j)->yOffSet) {
                        float iX = projectiles->at(i)->xOffSet + projectileQuadrants[3].x;
                        float iY = projectiles->at(i)->yOffSet + projectileQuadrants[3].y;
                        float jX = asteroids->at(j)->xOffSet + ASTEROID_QUADRANTS[1].x * asteroids->at(j)->type;
                        float jY = asteroids->at(j)->yOffSet + ASTEROID_QUADRANTS[1].y * asteroids->at(j)->type;

                        if(jX < iX && jY > iY) {
                            pCollisions.push_back(projectiles->at(i));
                            aCollisions.push_back(asteroids->at(j));
                        }  
                    }
                    else {
                        float iX = projectiles->at(i)->xOffSet + projectileQuadrants[0].x;
                        float iY = projectiles->at(i)->yOffSet + projectileQuadrants[0].y;
                        float jX = asteroids->at(j)->xOffSet + ASTEROID_QUADRANTS[2].x * asteroids->at(j)->type;
                        float jY = asteroids->at(j)->yOffSet + ASTEROID_QUADRANTS[2].y * asteroids->at(j)->type;

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
                    score += maxScore / asteroids->at(i)->type;
                    asteroids->erase(asteroids->begin()+i);
                    break;
                }
            }
        }
    }

    void Game::checkAsteroidCollisions()
    {   
        vector<Asteroid*> *asteroids = Asteroids::getAsteroids();

        vector<Asteroid*> collisions;

        for (int i = 0; i<asteroids->size(); i++) {
            for (int j = 0; j<asteroids->size(); j++) {
                if (i == j) continue;
                
                if (asteroids->at(i)->xOffSet > asteroids->at(j)->xOffSet) {
                    if (asteroids->at(i)->yOffSet > asteroids->at(j)->yOffSet) {
                        float iX = asteroids->at(i)->xOffSet + ASTEROID_QUADRANTS[2].x * asteroids->at(i)->type;
                        float iY = asteroids->at(i)->yOffSet + ASTEROID_QUADRANTS[2].y * asteroids->at(i)->type;
                        float jX = asteroids->at(j)->xOffSet + ASTEROID_QUADRANTS[0].x * asteroids->at(j)->type;
                        float jY = asteroids->at(j)->yOffSet + ASTEROID_QUADRANTS[0].y * asteroids->at(j)->type;

                        if(jX > iX && jY > iY) {
                            collisions.push_back(asteroids->at(i));
                            collisions.push_back(asteroids->at(j));
                        }
                    }
                    else {
                        float iX = asteroids->at(i)->xOffSet + ASTEROID_QUADRANTS[1].x * asteroids->at(i)->type;
                        float iY = asteroids->at(i)->yOffSet + ASTEROID_QUADRANTS[1].y * asteroids->at(i)->type;
                        float jX = asteroids->at(j)->xOffSet + ASTEROID_QUADRANTS[3].x * asteroids->at(j)->type;
                        float jY = asteroids->at(j)->yOffSet + ASTEROID_QUADRANTS[3].y * asteroids->at(j)->type;

                        if(jX > iX && jY < iY) {
                            std::cout << iX << " - " << iY << " | " << jX << " - " << jY << " -> " << "3" << std::endl;
                            collisions.push_back(asteroids->at(i));
                            collisions.push_back(asteroids->at(j));
                        }
                    }
                }
                else {
                    if (asteroids->at(i)->yOffSet > asteroids->at(j)->yOffSet) {
                        float iX = asteroids->at(i)->xOffSet + ASTEROID_QUADRANTS[3].x * asteroids->at(i)->type;
                        float iY = asteroids->at(i)->yOffSet + ASTEROID_QUADRANTS[3].y * asteroids->at(i)->type;
                        float jX = asteroids->at(j)->xOffSet + ASTEROID_QUADRANTS[1].x * asteroids->at(j)->type;
                        float jY = asteroids->at(j)->yOffSet + ASTEROID_QUADRANTS[1].y * asteroids->at(j)->type;

                        if(jX < iX && jY > iY) {
                            collisions.push_back(asteroids->at(i));
                            collisions.push_back(asteroids->at(j));
                        }  
                    }
                    else {
                        float iX = asteroids->at(i)->xOffSet + ASTEROID_QUADRANTS[0].x * asteroids->at(i)->type;
                        float iY = asteroids->at(i)->yOffSet + ASTEROID_QUADRANTS[0].y * asteroids->at(i)->type;
                        float jX = asteroids->at(j)->xOffSet + ASTEROID_QUADRANTS[2].x * asteroids->at(j)->type;
                        float jY = asteroids->at(j)->yOffSet + ASTEROID_QUADRANTS[2].y * asteroids->at(j)->type;

                        if(jX < iX && jY < iY) {
                            collisions.push_back(asteroids->at(i));
                            collisions.push_back(asteroids->at(j));
                        }
                    }
                }
                
                for (Asteroids::Asteroid *collision : collisions) {
                    for (int i=0; i<asteroids->size(); i++) {
                        if (asteroids->at(i) == collision) {
                            asteroids->erase(asteroids->begin()+i);
                        }
                    }
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