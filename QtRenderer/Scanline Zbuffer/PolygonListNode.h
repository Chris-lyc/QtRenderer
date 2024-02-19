#pragma once
#include <algorithm>
#include <array>
#include <queue>

#include "../camera.h"
#include "../dataStructure.hpp"
#include "../geometry.h"

#define EPISON 1e-6

class PolygonListNode
{
public:
    Vec2f p1,p2,p3; // three vertex
    float z1,z2,z3; // three vertex
    
    Vec3f abc; // plane function
    float dzx,dzy; // z increment of one pixel right / down

    int ymax;  // max of y
    int dy; // number of scanline
    
    float xl,xr; // two intersection point x
    float dxl,dxr; // x increment of on pixel down
    float dx3; // after mid point, use it to change one of the dx
    
    float zl; // left intersection point z

    int id; // id of primitive

    bool needChange; // if need to change dx after mid point 0: no change 1:dxl->dx3 2:dxr->dx3

    Vec3f color;

    PolygonListNode* next;
    PolygonListNode* pre;
    
public:
    PolygonListNode():next(nullptr),pre(nullptr)
    {
        
    }
    
    PolygonListNode(Primitive& pri) : next(nullptr), pre(nullptr), needChange(false)
    {
        Vertex a=pri.triangle[0];
        Vertex b=pri.triangle[1];
        Vertex c=pri.triangle[2];
        
        color=pri.color;
        
        std::vector<Vertex> vec={a,b,c};
        std::sort(vec.begin(),vec.end(),[](Vertex a,Vertex b)->bool
        {
            if(int(a.screenCoord.y)==int(b.screenCoord.y))
            {
                return a.screenCoord.x<b.screenCoord.x;
            }
            return int(a.screenCoord.y)>int(b.screenCoord.y);
        });
        
        p1={float(int(vec[0].screenCoord.x)),float(int(vec[0].screenCoord.y))};
        p2={float(int(vec[1].screenCoord.x)),float(int(vec[1].screenCoord.y))};
        p3={float(int(vec[2].screenCoord.x)),float(int(vec[2].screenCoord.y))};
        z1=vec[0].screenCoord.z;
        z2=vec[1].screenCoord.z;
        z3=vec[2].screenCoord.z;
        
        Vec3f tmp1={p1.x,p1.y,z1};
        Vec3f tmp2={p2.x,p2.y,z2};
        Vec3f tmp3={p3.x,p3.y,z3};
        abc=cross(tmp1-tmp2,tmp1-tmp3);
        if(abs(abc.z)<EPISON)
        {
            dzx=0;
            dzy=0;
        }
        else
        {
            dzx=-abc.x/abc.z;
            dzy=abc.y/abc.z;    
        }
        
        ymax=p1.y;
        dy=p1.y-p3.y;
        
        if(abs(p1.y-p2.y)<EPISON&&abs(p2.y-p3.y)<EPISON)
        {
            xl=float(p1.x);
            xr=float(p3.x);
            dxl=0;
            dxr=0;
            dx3=0;
        }
        else if(abs(p1.x-p2.x)<EPISON&&abs(p2.x-p3.x)<EPISON)
        {
            xl=float(p1.x);
            xr=float(p1.x);
            dxl=0;
            dxr=0;
            dx3=0;
        }
        else if(abs(p1.y-p2.y)<EPISON)
        {
            xl=float(p1.x);
            xr=float(p2.x);
            dxl=-(p3.x-p1.x)/(p3.y-p1.y);
            dxr=-(p3.x-p2.x)/(p3.y-p2.y);
            dx3=0;
        }
        else if(abs(p2.y-p3.y)<EPISON)
        {
            xl=float(p1.x);
            xr=float(p1.x);
            dxl=-(p2.x-p1.x)/(p2.y-p1.y);
            dxr=-(p3.x-p1.x)/(p3.y-p1.y);
            dx3=0;
        }
        else
        {
            xl=float(p1.x);
            xr=float(p1.x);
            dxl=-(p2.x-p1.x)/(p2.y-p1.y);
            dxr=-(p3.x-p1.x)/(p3.y-p1.y);
            dx3=-(p3.x-p2.x)/(p3.y-p2.y);
            needChange=true;
        }
        
        zl=z1;
    }

    void scanlineMove(int currentY);
    
};
