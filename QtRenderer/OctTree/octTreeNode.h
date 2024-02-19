#pragma once
#include <vector>

#include "../geometry.h"

class octTreeNode
{
public:
    std::vector<int> faceIndex;
    Vec3f minPoint;
    Vec3f maxPoint;
    octTreeNode* children[8];
    bool isLeaf;

    octTreeNode(float minx, float miny, float minz, float maxx, float maxy, float maxz):
        minPoint({minx, miny, minz}), maxPoint({maxx, maxy, maxz}), isLeaf(false)
    {
        for (int i = 0; i < 8; ++i)
        {
            children[i] = nullptr;
        }
    }

    octTreeNode(Vec3f minp,Vec3f maxp): minPoint(minp), maxPoint(maxp), isLeaf(false)
    {
        for (int i = 0; i < 8; ++i)
        {
            children[i] = nullptr;
        }
    }

    ~octTreeNode()
    {
        for(octTreeNode* node:children)
        {
            if(node!=nullptr)
            {
                delete node;
            }
        }
    }

    float getMaxZ();
};
