#ifndef TEXT_RENDER_H
#define TEXT_RENDER_H

#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>

#include "shader.hpp"

namespace TextRenders {

    // Holds all state information relevant to a character as loaded using FreeType
    struct Character {
        unsigned int TextureID; // ID handle of the glyph texture
        glm::ivec2 Size;        // Size of glyph
        glm::ivec2 Bearing;     // Offset from baseline to left/top of glyph
        int Advance;            // Horizontal offset to advance to next glyph
    };
    
    class TextRenderer {
        public:
            // Holds a list of pre-compiled Characters
            std::map<char, Character> characters; 
            // Shader used for text rendering
            Shader shader;
            // Constructor
            TextRenderer(float width, float height);
            // Pre-compiles a list of characters from the given font
            void Load(std::string font, unsigned int fontSize);
            // Renders a string of text using the precompiled list of characters
            void renderText(std::string text, float x, float y, float scale, glm::vec3 color = glm::vec3(1.0f));

        private:
            // Render state
            unsigned int VAO, VBO;
    };
}

#endif