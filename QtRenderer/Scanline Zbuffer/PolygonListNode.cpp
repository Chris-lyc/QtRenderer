#include "PolygonListNode.h"

void PolygonListNode::scanlineMove(int currentY)
{
    dy--;
    if(dy<0)return;
        
    if(abs(currentY-p2.y)<EPISON&&needChange!=0)
    {
        if(needChange)dxl=dx3;
    }
    xl+=dxl;
    xr+=dxr;
    zl+=dzx*dxl+dzy;
}