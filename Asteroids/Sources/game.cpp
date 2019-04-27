#include <map>

#include "game.hpp"

/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    GLuint TextureID;   // ID handle of the glyph texture
    glm::ivec2 Size;    // Size of glyph
    glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
    GLuint Advance;    // Horizontal offset to advance to next glyph
};

std::map<GLchar, Character> Characters;
GLuint VAO, VBO;

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
        // Compile and setup the shader
        textShader = new Shader("../Asteroids/Shaders/text.vs", "../Asteroids/Shaders/text.fs");
        glm::mat4 projection = glm::ortho(0.0f, Width, 0.0f, Height);
        textShader->use();

        textShader->setMat4("projection", projection);

        // FreeType
        FT_Library ft;
        // All functions return a value different than 0 whenever an error occurred
        if (FT_Init_FreeType(&ft))
            std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

        // Load font as face
        FT_Face face;
        if (FT_New_Face(ft, "../Asteroids/Fonts/arial.ttf", 0, &face))
            std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

        // Set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, 48);

        // Disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 

        // Load first 128 characters of ASCII set
        for (GLubyte c = 0; c < 128; c++)
        {
            // Load character glyph 
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
            // Generate texture
            GLuint texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );
            // Set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // Now store character for later use
            Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                face->glyph->advance.x
            };
            Characters.insert(std::pair<GLchar, Character>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
        // Destroy FreeType once we're finished
        FT_Done_Face(face);
        FT_Done_FreeType(ft);

        
        // Configure VAO/VBO for texture quads
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
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

        renderText(textShader, std::to_string(score), 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
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

        std::cout << projectiles->size() << std::endl;

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

    void renderText(Shader *shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
    {
        // Activate corresponding render state	
        shader->use();
        shader->setVec3("textColor", color.x, color.y, color.z);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO);

        // Iterate through all characters
        std::string::const_iterator c;
        for (c = text.begin(); c != text.end(); c++) 
        {
            Character ch = Characters[*c];

            GLfloat xpos = x + ch.Bearing.x * scale;
            GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

            GLfloat w = ch.Size.x * scale;
            GLfloat h = ch.Size.y * scale;
            // Update VBO for each character
            GLfloat vertices[6][4] = {
                { xpos,     ypos + h,   0.0, 0.0 },            
                { xpos,     ypos,       0.0, 1.0 },
                { xpos + w, ypos,       1.0, 1.0 },

                { xpos,     ypos + h,   0.0, 0.0 },
                { xpos + w, ypos,       1.0, 1.0 },
                { xpos + w, ypos + h,   1.0, 0.0 }           
            };
            // Render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            // Update content of VBO memory
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // Render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
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