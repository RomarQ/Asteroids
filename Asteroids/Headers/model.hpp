#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <vector>
using namespace std;

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <stb_image.h>

#include "shader.hpp"
#include "mesh.hpp"

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);

class Model 
{   
    private:
        /*  Functions   */
        void loadModel(string path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
    public:
        /*  Model Data */
        vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
        vector<Mesh> meshes;
        string directory;
        bool gammaCorrection;
        
        /*  Functions   */
        Model(string const &path, bool gamma = false);
        void Draw(Shader shader);
};

#endif