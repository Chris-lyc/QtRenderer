#pragma once

#include "dataStructure.hpp"
#include <vector>

class Mesh
{
public:
    // std::vector<Vertex> vertices;
    // std::vector<unsigned int> indices;
    std::vector<Primitive> primitives;
    std::vector<unsigned int> indexs;

    Mesh()
    {
    }

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
    {
        for (int i = 0; i < indices.size(); i += 3)
        {
            assert(i + 1 < indices.size() && i + 2 < indices.size());
            indexs.push_back(primitives.size());
            primitives.push_back(
                {
                    vertices.at(indices.at(i)),
                    vertices.at(indices.at(i + 1)),
                    vertices.at(indices.at(i + 2))
                }
            );
        }
    }

    // void draw();
};
