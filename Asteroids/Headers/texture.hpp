#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class TextureBox {
    public:
        static GLuint loadTexture( const std::string & fName );
        static GLuint loadCubeMap( const std::string & baseName );
        static unsigned char * loadPixels( const std::string & fName, int & w, int & h );
};