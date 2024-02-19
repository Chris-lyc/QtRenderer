#include "zQuadTreeNode.h"

#define EPISON 1e-6

bool zQuadTreeNode::isLeaf()
{
    return (minX == maxX) && (minY == maxY);
}

bool zQuadTreeNode::containPosition(int x, int y)
{
    return x >= minX && x <= maxX && y >= minY && y <= maxY;
}

bool zQuadTreeNode::containPolygon(Primitive& primitive)
{
    return std::ceil(primitive.getMaxX()) <= maxX && std::ceil(primitive.getMaxY()) <= maxY
        && std::floor(primitive.getMinX()) >= minX && std::floor(primitive.getMinY()) >= minY;
}

bool zQuadTreeNode::containBBox(float minx, float miny, float maxx, float maxy)
{
    return this->minX < minx && this->minY < miny && this->maxX > maxx && this->maxY > maxy;
}

bool zQuadTreeNode::overlapBBox(float minx, float miny, float maxx, float maxy)
{
    return this->minX < maxx && this->maxX > minx && this->minY < maxy && this->maxY > miny;
}

int zQuadTreeNode::selectChildNode(int x, int y)
{
    int midx = (minX + maxX) / 2;
    int midy = (minY + maxY) / 2;
    if (x >= minX && x <= midx)
    {
        if (y >= minY && y <= midy)return 0;
        else return 3;
    }
    else
    {
        if (y >= minY && y <= midy)return 1;
        else return 2;
    }
}

// update from current node (current node is updated)
void zQuadTreeNode::update()
{
    zQuadTreeNode* tmp = this;
    while (tmp->father != nullptr)
    {
        float maxZ = 0;
        for (int i = 0; i < 4; ++i)
        {
            if (tmp->father->children[i] != nullptr)
            {
                maxZ = std::max(maxZ, tmp->father->children[i]->depth);
            }
        }
        // father dont need update
        if (std::abs(tmp->father->depth - maxZ) < EPISON)
        {
            break;
        }
        // update father
        else
        {
            tmp->father->depth = maxZ;
            tmp = tmp->father;
        }
    }
}
