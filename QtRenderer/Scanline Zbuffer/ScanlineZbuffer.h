#pragma once
#include <vector>

#include "PolygonList.h"

class ScanlineZbuffer
{
public:
    std::vector<PolygonList*> selectedPolygonListVec;
    PolygonList* activePolygonList;

    Mesh* mesh;

    int maxWidth;
    int maxHeight;

public:
    ScanlineZbuffer(){}
    
    ScanlineZbuffer(Mesh* mesh, int width, int height) : mesh(mesh), maxWidth(width), maxHeight(height), activePolygonList(nullptr)
    {
        selectedPolygonListVec.resize(height);
        for(int i=0;i<selectedPolygonListVec.size();++i)
        {
            selectedPolygonListVec[i]=new PolygonList();
        }
        activePolygonList=new PolygonList();

        for(int index:mesh->indexs)
        {
            Primitive pri=mesh->primitives[index];
            
            if(pri.isCulled)continue;

            if(pri.getMaxX()>=maxWidth||pri.getMinX()<0||pri.getMaxY()>=maxHeight||pri.getMinY()<0)continue;
            
            PolygonListNode* node=new PolygonListNode(pri);
            
            // if ymax > screen.y, then discard
            if(node->ymax>=height)
            {
                delete node;
                continue;
            }

            node->id=index;
            
            selectedPolygonListVec[node->ymax]->addFront(node);
        }
    }

    ~ScanlineZbuffer()
    {
        for(int i=0;i<selectedPolygonListVec.size();++i)
        {
            delete selectedPolygonListVec[i];
        }
        selectedPolygonListVec.clear();
        selectedPolygonListVec.shrink_to_fit();

        delete activePolygonList;
    }
};
