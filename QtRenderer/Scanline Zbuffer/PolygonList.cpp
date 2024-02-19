#include "PolygonList.h"

void PolygonList::addFront(PolygonListNode* node)
{
    PolygonListNode* tmp=head->next;
    head->next=node;
    node->next=tmp;

    if(tmp!=nullptr)
    {
        tmp->pre=node;    
    }
    node->pre=head;

    if(tail==nullptr)
    {
        tail=node;
    }
}

void PolygonList::addFrontRange(PolygonList* list)
{
    if(list->head->next==nullptr)
    {
        return;
    }

    list->tail->next=head->next;
    if(head->next==nullptr)
    {
        tail=list->tail;
    }
    else
    {
        head->next->pre=list->tail;
    }
    head->next=list->head->next;
    list->head->next->pre=head;
        
    list->head->next=nullptr;
    list->tail=nullptr;
}

void PolygonList::removeNode(PolygonListNode* node)
{
    node->pre->next=node->next;
    if(node==tail)
    {
        tail=node->pre;
    }
    else
    {
        node->next->pre=node->pre;
    }
    delete(node);
}