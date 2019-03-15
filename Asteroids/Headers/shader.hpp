#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <iostream>

class Shader
{
    private:
        // the program ID
        unsigned int ID;

        void checkCompileErrors(unsigned int shader, std::string type);

    public:
        // constructor reads and builds the shader
        Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
        // use/activate the shader
        void use() const;
        // utility uniform functions
        void setBool(const std::string &name, bool value) const;  
        void setInt(const std::string &name, int value) const;   
        void setFloat(const std::string &name, float value) const;
};
  
#endif