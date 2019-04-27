#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <vector>
using namespace std;

#include <glm/glm.hpp>

#include "shader.hpp"

struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
};

struct Texture {
    unsigned int id;
    string type;
    string path;
};

class Mesh {
    private:
        void setupMesh();
    public:
        // Render data
        unsigned int VAO, VBO, EBO;
        // Mesh Data
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<Texture> textures;

        Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
        void Draw(Shader shader);
};

#endif