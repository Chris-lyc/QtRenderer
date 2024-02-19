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
    std::vector<Mesh> meshes;
    Texture* diffusemap_;
    Texture* normalmap_;
    Texture* specularmap_;
    
    Model(std::string path):diffusemap_(nullptr),normalmap_(nullptr),specularmap_(nullptr)
    {
        loadModel(path);
    }
    // void draw();
    void displayMeshNumber();
    
private:
    std::string directory;
    
    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
};