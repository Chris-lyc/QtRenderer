#pragma once

#include"dataStructure.hpp"
#include <vector>

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    //std::vector<Texture> textures;

    Mesh() {};
    void draw();
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) :vertices(vertices), indices(indices) {};
    
    //Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    //void Draw(Shader shader);
};