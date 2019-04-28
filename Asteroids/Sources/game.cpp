#include "game.hpp"

namespace Games {
    float Width, Height;

    Camera camera;

    TextRenderer *textDisplay;

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

    /*
    *   Timestamp in seconds with 6 decimal units (0.000000)
    *   This timestamp allows to delay key press when selecting a difficulty level
    */
    float lastKeyPressTimestamp = 0;

    Game::Game(float width, float height, GLFWwindow* window) : 
        State(GAME_MENU),
        score(0.0)
    { 
        Width = width;
        Height = height;
        camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f));

        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);

        // capture our mouse
        //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        textDisplay = new TextRenderer(width, height);
    }

    Game::~Game()
    {

    }

    void Game::updateGameState(GameState state) {
        State = state;

        switch (state) {
            case GAME_ACTIVE:
                Asteroids::destroyAll();
                Projectiles::destroyAll();

                score = 0.0;
                lives = GAME_MAX_LIVES;
                spaceShip->xOffSet = 0;
                spaceShip->yOffSet = 0;

                break;
            case GAME_MENU:
                break;
            case GAME_OVER:

                break;
        }
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
        switch (State)
        {
            case GAME_ACTIVE:

                Game::checkProjectileCollisions();
                Game::checkAsteroidCollisions();
                Game::checkShipCollisions();

                spaceShip->render(Width, Height, camera);
                Asteroids::renderAsteroids(Width, Height, difficulty, camera);
                Projectiles::renderProjectiles(Width, Height, camera);

                textDisplay->renderText(std::to_string(score), 10.0f, 10.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));

                textDisplay->renderText("Lives: " + std::to_string(lives), Width - 125.0f, 10.0f, 0.5f, glm::vec3(0.5, 0.5f, 0.0f));
                break;

            case GAME_MENU:

                textDisplay->renderText("Press ENTER to start", 250.0f, Height / 2, 1.0f);
                textDisplay->renderText("Press A or D to select level", 245.0f, Height / 2 - 40.0f, 0.75f);
                textDisplay->renderText(
                    difficulty == 1 ? "Easy" : "Hard",
                    440.0f, 
                    Height / 2 - 120.0f,
                    1.5f, 
                    difficulty == 1 ? glm::vec3(0.0, 0.5, 0.0) : glm::vec3(0.5, 0.0, 0.0)
                );
                break;

            case GAME_OVER:

                textDisplay->renderText("GAME OVER", 250.0f, Height / 2, 1.0f, glm::vec3(0.0, 0.5, 0.0));
                textDisplay->renderText("Final Score: " + std::to_string(score), 245.0f, Height / 2 - 40.0f, 0.75f);
                textDisplay->renderText(
                    "Press R to go back to menu",
                    245.0f, 
                    Height / 2 - 150.0f,
                    0.5f, 
                    glm::vec3(0.5, 0.5, 0.0)
                );
                break;

            default:
                break;
        }
    }

    void Game::ProcessInput(GLFWwindow *window)
    {
        switch (State)
        {
            case GAME_ACTIVE:

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

                break;

            case GAME_MENU:

                if (glfwGetTime() < lastKeyPressTimestamp+MENU_KEY_PRESS_COOLDOWN)
                    break;

                if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                {
                    difficulty > 1 ? difficulty-- : difficulty = GAME_MAX_DIFFFICULTY;
                    lastKeyPressTimestamp = glfwGetTime();
                } 
                if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                {   
                    difficulty < GAME_MAX_DIFFFICULTY ? difficulty++ : difficulty = 1;
                    lastKeyPressTimestamp = glfwGetTime();
                }
                if(glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
                {   
                    updateGameState(GAME_ACTIVE);
                    lastKeyPressTimestamp = glfwGetTime();
                }
                break;

            case GAME_OVER:

                if (glfwGetTime() < lastKeyPressTimestamp+MENU_KEY_PRESS_COOLDOWN)
                    break;

                if(glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
                {
                    updateGameState(GAME_MENU);
                    lastKeyPressTimestamp = glfwGetTime();
                } 

                break;
        }
    }

    void Game::checkShipCollisions()
    {
        vector<Asteroid*> *asteroids = getAsteroids();

        vector<Asteroid*> collisions;

        AABB hitBox = asteroidHitbox();

        for (unsigned long int i = 0; i < asteroids->size(); i++) {
            
            float width = abs(asteroids->at(i)->xOffSet - spaceShip->xOffSet);
            float height = abs(asteroids->at(i)->yOffSet - spaceShip->yOffSet);

            float distance = sqrt(width*width + height*height);

            float minDistanceForImpact = hitBox.max.x + shipRadius;

            if ( minDistanceForImpact > distance ) {
                if (lives == 1)
                    updateGameState(GAME_OVER);

                asteroids->erase(asteroids->begin()+i);

                lives--;
            }
        }
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
                    score += difficulty;
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
        glViewport(0, 0, width, height);
    }
}