#pragma once
#include <cstdlib>
#include <vector>
#include <array>
#include "../dataStructure.hpp"

class zQuadTreeNode
{
public:
    float depth;
    int minX;
    int minY;
    int maxX;
    int maxY;
    zQuadTreeNode* children[4];
    zQuadTreeNode* father;

public:
    // zQuadTreeNode():depth(std::numeric_limits<float>::max()),children(nullptr){}

    zQuadTreeNode():father(nullptr)
    {
        for(int i=0;i<4;++i)
        {
            children[i]=nullptr;
        }
    }

    zQuadTreeNode(float depthValue, int minx, int miny, int maxx, int maxy): depth(depthValue), minX(minx), minY(miny),
                                                                           maxX(maxx), maxY(maxy)
    {
        for (int i = 0; i < 4; ++i)
        {
            children[i] = nullptr;
        }
        father = nullptr;
    }

    bool isLeaf();
    bool containPosition(int x, int y);
    int selectChildNode(int x, int y);

    bool containPolygon(Primitive& primitive);
    bool containBBox(float minx,float miny,float maxx,float maxy);
    bool overlapBBox(float minx,float miny,float maxx,float maxy);

    void update();
};
