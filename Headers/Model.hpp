#ifndef MODEL_H_GUARD
#define MODEL_H_GUARD
#pragma once

// glad: an OpenGL function loader: https://github.com/Dav1dde/glad
#include <glad/glad.h>
// assimp: model loading: https://github.com/assimp/assimp
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>


#include <vector>
#include <string>

#include "Mesh.hpp"
#include "Shader.hpp"

class Model {
public:
    /* Functions */
    Model(GLchar* path);
    void Draw(Shader shader);
private:
    /* Model Data */
    std::vector<Texture> textures_loaded;
    std::vector<Mesh> meshes;
    std::string directory;
    /* Functions */
    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

#endif//MODEL_H_GUARD
