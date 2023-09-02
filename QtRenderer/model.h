#pragma once

#include <vector>
#include "geometry.h"
#include "mesh.h"
#include "texture.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model {
public:
    Model(std::string path) { loadModel(path); }
    void draw();
private:
    std::vector<Mesh> meshes;
    Texture diffusemap_;
    Texture normalmap_;
    Texture specularmap_;
    std::string directory;
    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    //void loadTexture(std::string filename, const char* suffix, Texture& img);
    //int loadMaterialTextures(Mesh& mesh, aiMaterial* mat, aiTextureType type);
};