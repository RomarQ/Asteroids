#include "game.hpp"

namespace Games {
    float Width, Height;

    Camera camera;

    TextRenderer *textDisplay;

    /*
    *   Ship radius
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

        textDisplay = new TextRenderer(width, height);
    }

    Game::~Game() {}

    void Game::updateGameState(GameState state) {
        State = state;

        switch (state) {
            case GAME_ACTIVE:

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

        /*
        *   Build and compile the Space Ship
        */
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
                
                if(State == GAME_OVER) {
                    Asteroids::destroyAll();
                    Projectiles::destroyAll();
                }

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

            case GAME_TEST:
            
                Game::checkAsteroidCollisions();
                Asteroids::renderTest(Width, Height, difficulty, camera, .0f);

                break;

            default:
                break;
        }
    }

    void Game::ProcessInput(GLFWwindow *window)
    {
        switch (State) {
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

                break;
            case GAME_MENU:

                if (glfwGetTime() < lastKeyPressTimestamp+MENU_KEY_PRESS_COOLDOWN)
                    break;

                if(glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
                {   
                    updateGameState(GAME_TEST);
                    lastKeyPressTimestamp = glfwGetTime();
                }
                if(glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
                {   
                    updateGameState(GAME_ACTIVE);
                    lastKeyPressTimestamp = glfwGetTime();
                }

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
            case GAME_TEST:

                if (glfwGetTime() < lastKeyPressTimestamp+MENU_KEY_PRESS_COOLDOWN)
                    break;

                if(glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
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

                if ( minDistanceForImpact >= distance ) {
                    // FORMULA: V - 2*(V dot N)*N

                    glm::vec2 vecA1 = glm::vec2(glm::cos(asteroids->at(a1)->angle), glm::sin(asteroids->at(a1)->angle));
                    glm::vec2 vecA2 = glm::vec2(glm::cos(asteroids->at(a2)->angle), glm::sin(asteroids->at(a2)->angle));
                    // Normalized Vectors
                    glm::vec2 nVecA1 = glm::normalize(vecA1 - vecA2);
                    glm::vec2 nVecA2 = glm::normalize(vecA2 - vecA1);

                    // Alternative -> glm::reflect(vecA1, nVecA1)
                    glm::vec2 newVecA1 = vecA1 - 2*glm::dot(vecA1, nVecA1)*nVecA1;

                    // Alternative -> glm::reflect(vecA2, nVecA2)
                    glm::vec2 newVecA2 = vecA2 - 2*glm::dot(vecA2, nVecA2)*nVecA2;
                    
                    // Update angles
                    asteroids->at(a1)->angle = glm::atan(newVecA1.y, newVecA1.x);
                    asteroids->at(a2)->angle = glm::atan(newVecA2.y, newVecA2.x);
                }
            }
        }
    }

    int PositiveAngle(int angle) {
        return angle < 0 ? 360+angle : angle;
    }

    // glfw: whenever the window size changed (by OS or user resize) this callback function executes
    // ---------------------------------------------------------------------------------------------
    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }
}