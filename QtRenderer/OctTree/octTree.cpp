#include "octTree.h"
#include "../shader.hpp"

#include <tbb/parallel_for.h>
#include <tbb/parallel_for_each.h>
#include <tbb/blocked_range2d.h>

int octTreeNodeThreshold=100;

octTree::octTree(Mesh* mesh, bool multiThread):mesh(nullptr),root(nullptr),multiThread(multiThread)
{
    this->mesh=mesh;
    
    Vec3f minp={FLT_MAX,FLT_MAX,FLT_MAX};
    Vec3f maxp={-FLT_MAX,-FLT_MAX,-FLT_MAX};
    std::vector<unsigned int> subIndex;

    for(int index:mesh->indexs)
    {
        Primitive pri=mesh->primitives[index];
        if(pri.isCulled)continue;
    
        minp.x=std::min(minp.x,pri.getMinX());
        minp.y=std::min(minp.y,pri.getMinY());
        minp.z=std::min(minp.z,pri.getMinZ());
        maxp.x=std::max(maxp.x,pri.getMaxX());
        maxp.y=std::max(maxp.y,pri.getMaxY());
        maxp.z=std::max(maxp.z,pri.getMaxZ());
        subIndex.push_back(index);
    }
        
    root=buildSceneOctTree(minp,maxp,subIndex);
}

octTreeNode* octTree::buildSceneOctTree(Vec3f minp, Vec3f maxp, std::vector<unsigned int>& subIndex)
{
    if(subIndex.size()==0)
    {
        return nullptr;
    }
    octTreeNode* node=new octTreeNode(minp,maxp);

    // if num of primitives < threshold, all push to current node
    if(subIndex.size()<octTreeNodeThreshold)
    {
        for(int index:subIndex)
        {
            node->faceIndex.emplace_back(index);
        }
        
        node->isLeaf=true;
        return node;
    }

    Vec3f midp=(minp+maxp)/2;

    //----------------
    // (): after
    // 2(6) 3(7)
    // 0(4) 1(5)
    //----------------
    float axis[3][3]={
        minp.x,midp.x,maxp.x,
        minp.y,midp.y,maxp.y,
        minp.z,midp.z,maxp.z
    };

    std::vector<std::vector<unsigned int>> nextIndex(8);
        
    bool tag;   // use to break
    for(int index:subIndex)
    {
        tag=true;
    
        for(int dirz=0;dirz<2;++dirz)
        {
            for(int diry=0;diry<2;++diry)
            {
                for(int dirx=0;dirx<2;++dirx)
                {
                    Vec3f minpNext={axis[0][dirx],axis[1][diry],axis[2][dirz]};
                    Vec3f maxpNext={axis[0][dirx+1],axis[1][diry+1],axis[2][dirz+1]};
                    if(mesh->primitives[index].belongTo3DBBox(minpNext,maxpNext))
                    {
                        nextIndex[dirz*4+diry*2+dirx].push_back(index);
                        tag=false;
                        break;
                    }
                }
                if(tag==false)break;
            }
            if(tag==false)break;
        }
        // not belong to any one sub node, then belong to this node
        if(tag==true)
        {
            node->faceIndex.push_back(index);
        }
    }
    
    // no sub node
    if(node->faceIndex.size()==subIndex.size())
    {
        node->isLeaf=true;
        return node;
    }

    // recursive process sub node
    for(int dirz=0;dirz<2;++dirz)
    {
        for(int diry=0;diry<2;++diry)
        {
            for(int dirx=0;dirx<2;++dirx)
            {
                Vec3f minpNext={axis[0][dirx],axis[1][diry],axis[2][dirz]};
                Vec3f maxpNext={axis[0][dirx+1],axis[1][diry+1],axis[2][dirz+1]};
                int index=dirz*4+diry*2+dirx;
                node->children[index]=buildSceneOctTree(minpNext,maxpNext,nextIndex[index]);
            }
        }
    }
    return node;
}
