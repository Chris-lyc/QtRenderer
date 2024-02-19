#pragma once

#include "zQuadTreeNode.h"
#include "../dataStructure.hpp"

class zPyramid
{
public:
    zQuadTreeNode* root;
    int width;
    int height;
    zQuadTreeNode* zbuffer;
public:

    zPyramid(int width,int height);

    zQuadTreeNode* buildZPyramid(int minx,int miny,int maxx,int maxy,zQuadTreeNode* father);

    void resetPyramid(float value,zQuadTreeNode* node);

};
