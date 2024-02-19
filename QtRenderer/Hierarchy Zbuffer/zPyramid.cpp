#include "zPyramid.h"

#include <iostream>

zPyramid::zPyramid(int width, int height): width(width), height(height)
{
    zbuffer = new zQuadTreeNode[width * height];

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int loc = y * width + x;
            zbuffer[loc].minX = zbuffer[loc].maxX = x;
            zbuffer[loc].minY = zbuffer[loc].maxY = y;
            zbuffer[loc].depth = FLT_MAX;
        }
    }
    root = buildZPyramid(0, 0, width - 1, height - 1, nullptr);
}

zQuadTreeNode* zPyramid::buildZPyramid(int minx, int miny, int maxx, int maxy, zQuadTreeNode* father)
{
    if (minx > maxx || miny > maxy || minx < 0 || miny < 0)
    {
        return nullptr;
    }

    // leaf
    if (minx == maxx && miny == maxy)
    {
        int loc = miny * width + minx;
        zbuffer[loc].father = father;
        return &(zbuffer[loc]);
    }

    // not leaf
    zQuadTreeNode* node = new zQuadTreeNode(std::numeric_limits<float>::max(), minx, miny, maxx, maxy);
    node->father = father;

    int midx = (maxx + minx) / 2;

    int midy = (maxy + miny) / 2;

    // -----------------
    //  3  2
    //  0  1
    // -----------------
    node->children[0] = buildZPyramid(minx, miny, midx, midy, node);
    node->children[1] = buildZPyramid(midx + 1, miny, maxx, midy, node);
    node->children[2] = buildZPyramid(midx + 1, midy + 1, maxx, maxy, node);
    node->children[3] = buildZPyramid(minx, midy + 1, midx, maxy, node);

    return node;
}

void zPyramid::resetPyramid(float value,zQuadTreeNode* node)
{
    if(node!=nullptr)
    {
        node->depth=value;
        for(int i=0;i<4;++i)
        {
            if(node->children[i]!=nullptr)
                resetPyramid(value,node->children[i]);
        }
    }
}











// void zPyramid::updatePyramid(zQuadTreeNode* node)
// {
//     zQuadTreeNode* tmp = node;
//     while (tmp->father != nullptr)
//     {
//         // find farthest value
//         float newValue = std::numeric_limits<float>::min();
//         for (int i = 0; i < tmp->father->children.size(); ++i)
//         {
//             if (tmp->father->children[i] != nullptr)
//             {
//                 newValue = std::max(newValue, tmp->father->children[i]->depth);
//             }
//         }
//         // renew current father
//         if (std::abs(newValue - tmp->father->depth) > EPISON)
//         {
//             tmp->father->depth = newValue;
//             tmp = tmp->father;
//         }
//         // if not need to renew current father, then fathers are also no need to renew
//         else return;
//     }
// }


// bool zPyramid::zTest(int x, int y, float depth)
// {
//     // std::cout<<depth<<std::endl;
//     zQuadTreeNode* node=this->root;
//     while(!node->isLeaf())
//     {
//         // nearer than the farthest, test in children
//         if(depth<node->depth)
//         {
//             int index=node->selectChildNode(x,y);
//             node=node->children[index];
//         }
//         
//         // farther than the farthest, return false
//         else
//         {
//             return false;
//         }
//     }
//
//     // nearer than leaf node, renew zpyramid
//     if(depth<node->depth)
//     {
//         node->depth=depth;
//         updatePyramid(node);
//     }
//     // farther than leaf node, return false
//     else return false;
//     
//
//     return true;
// }
