#pragma once

#include"dataStructure.hpp"
#include <vector>

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    Mesh() {};
    void draw();
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) :vertices(vertices), indices(indices) {};
};