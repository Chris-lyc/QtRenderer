#pragma once

#include "../model.h"
#include "octTreeNode.h"

class octTree
{
public:
    Mesh* mesh;
    octTreeNode* root;
    bool multiThread;

    octTree():mesh(nullptr),root(nullptr),multiThread(false){}
    
    octTree(Mesh* mesh, bool multiThread=false);

    ~octTree()
    {
        delete root;
    }
    
    octTreeNode* buildSceneOctTree(Vec3f minp, Vec3f maxp, std::vector<unsigned int>& subIndex);
};
