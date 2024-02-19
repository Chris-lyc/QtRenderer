#pragma once

#include "PolygonListNode.h"
#include "../model.h"

class PolygonList
{
public:
    // with head node
    PolygonListNode* head;
    PolygonListNode* tail;

public:
    PolygonList()
    {
        head=new PolygonListNode();
        tail=nullptr;
    }

    void addFront(PolygonListNode* node);

    void addFrontRange(PolygonList* list);

    void removeNode(PolygonListNode* node);

    ~PolygonList()
    {
        PolygonListNode* node=head;
        while(node!=nullptr)
        {
            PolygonListNode* tmp=node;
            node=node->next;
            delete(tmp);
        }
    }
    
};
