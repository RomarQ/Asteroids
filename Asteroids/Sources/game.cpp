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
    float shipRadius = 0.15;

    Game::Game(float width, float height, GLFWwindow* window) : 
        State(GAME_ACTIVE),
        score(0.0),
        scoreDisplay(width, height)
    { 
        Width = width;
        Height = height;
        camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f));

        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);

        // capture our mouse
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

    void Game::Render()
    {
        Game::checkProjectileCollisions();
        Game::checkAsteroidCollisions();

        if(Game::checkShipCollisions()) {
            std::cout << "ACIDENTE" << std::endl;
        }

        spaceShip->render(Width, Height, camera);
        Asteroids::renderAsteroids(Width, Height, camera);
        Projectiles::renderProjectiles(Width, Height, camera);

        scoreDisplay.renderText(std::to_string(score), 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
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
        vector<Asteroid*> *asteroids = getAsteroids();

        vector<Asteroid*> collisions;

        AABB hitBox = asteroidHitbox();

        for (Asteroid * asteroid : *asteroids) {
            
            float width = abs(asteroid->xOffSet - spaceShip->xOffSet);
            float height = abs(asteroid->yOffSet - spaceShip->yOffSet);

            float distance = sqrt(width*width + height*height);

            float minDistanceForImpact = hitBox.max.x + shipRadius;

            if ( minDistanceForImpact > distance ) {
                return true;
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

        AABB aHitBox = asteroidHitbox();
        AABB pHitBox = projectileHitbox();

        for (long unsigned int i = 0; i < projectiles->size(); i++) {
            for (long unsigned int j = 0; j < asteroids->size(); j++) {

                float width = abs(projectiles->at(i)->xOffSet - asteroids->at(j)->xOffSet);
                float height = abs(projectiles->at(i)->yOffSet - asteroids->at(j)->yOffSet);

                float distance = sqrt(width*width + height*height);

                float minDistanceForImpact = aHitBox.max.x + pHitBox.max.x;

                if ( minDistanceForImpact > distance ) {
                    aCollisions.push_back(asteroids->at(j));
                    pCollisions.push_back(projectiles->at(i));
                }
            }
        }

        for (Projectile *collision : pCollisions) {
            for (long unsigned int i=0; i<projectiles->size(); i++) {
                if (projectiles->at(i) == collision) {
                    projectiles->erase(projectiles->begin()+i);
                    break;
                }
            }
        }

        for (Asteroid *collision : aCollisions) {
            for (long unsigned int i=0; i<asteroids->size(); i++) {
                if (asteroids->at(i) == collision) {
                    score += asteroids->at(i)->type;
                    asteroids->erase(asteroids->begin()+i);
                    break;
                }
            }
        }
    }

    void Game::checkAsteroidCollisions()
    {   
        vector<Asteroid*> *asteroids = getAsteroids();

        vector<Asteroid*> collisions;

        AABB hitBox = asteroidHitbox();

        for (long unsigned int a1 = 0; a1 < asteroids->size(); a1++) {
            for (long unsigned int a2 = 0; a2 < asteroids->size(); a2++) {
                if (a1 <= a2) continue;
                
                float width = abs(asteroids->at(a1)->xOffSet - asteroids->at(a2)->xOffSet);
                float height = abs(asteroids->at(a1)->yOffSet - asteroids->at(a2)->yOffSet);

                float distance = sqrt(width*width + height*height);

                float minDistanceForImpact = 2*hitBox.max.x;

                if ( minDistanceForImpact > distance ) {
                    //FORMULA: 2*(V dot N)*N - V
                    glm::vec2 vecA1 = glm::vec2(cos(asteroids->at(a1)->angle), sin(asteroids->at(a1)->angle));
                    glm::vec2 vecA2 = glm::vec2(cos(asteroids->at(a2)->angle), sin(asteroids->at(a2)->angle));
                    glm::vec2 nVecA1 = 2*glm::dot(vecA1, vecA2)*vecA2-vecA1;
                    glm::vec2 nVecA2 = 2*glm::dot(vecA2, vecA1)*vecA1-vecA2;

                    asteroids->at(a1)->angle = glm::degrees(acos(nVecA1.x));
                    asteroids->at(a2)->angle = glm::degrees(acos(nVecA2.x));
                }
            }
        }
    }

    int PositiveAngle(int angle) {
        return angle < 0 ? 360+angle : angle;
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