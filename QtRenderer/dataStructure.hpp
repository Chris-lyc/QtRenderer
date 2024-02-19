#pragma once

#include "geometry.h"

using Color = Vec3f;

enum TextureType
{
    DEFAULT,
    DIFFUSE,
    NORMAL,
    SPECLUAR
};

struct Buffer
{
    float* data;
    int width;
    int height;

    Buffer(int w, int h) : width(w), height(h)
    {
        data = (float*)malloc(sizeof(float) * width * height);
    }

    ~Buffer()
    {
        free(data);
    }

    float getPixel(int x, int y)
    {
        return data[y * width + x];
    }

    void setPixel(int x, int y, float value)
    {
        data[y * width + x] = value;
    }
};

struct Vertex
{
    Vec3f worldCoord;
    Vec3f normal;
    Vec2f uvCoord;
    Vec3f screenCoord;
};

struct Fragment
{
    mat<2, 3, float> varying_uv;
    mat<3, 3, float> varying_nrm; // normal per vertex to be interpolated by FS
    mat<3, 3, float> varying_tri;
    Color color;
    Vec3f screenCoord;
    Vec2f uvCoord;

    Vec3f bar;
};

struct Primitive
{
    std::vector<Vertex> triangle;
    Vec3f bbox_max;
    Vec3f bbox_min;

    bool isCulled;

    Vec3f color;

    Primitive(std::vector<Vertex>& triangle) : triangle(triangle), isCulled(false)
    {
        color.x=rand()%255;
        color.y=rand()%255;
        color.z=rand()%255;
    }

    Primitive(Vertex a, Vertex b, Vertex c) : isCulled(false)
    {
        triangle.push_back(a);
        triangle.push_back(b);
        triangle.push_back(c);

        color.x=rand()%255;
        color.y=rand()%255;
        color.z=rand()%255;
    }

    void calculateBBox()
    {
        bbox_max = {
            -FLT_MAX, -FLT_MAX, -FLT_MAX
        };
        bbox_min = {
            FLT_MAX, FLT_MAX, FLT_MAX
        };

        for (int i = 0; i < triangle.size(); ++i)
        {
            bbox_max.x = std::max(bbox_max.x, triangle[i].screenCoord.x);
            bbox_max.y = std::max(bbox_max.y, triangle[i].screenCoord.y);
            bbox_max.z = std::max(bbox_max.z, triangle[i].screenCoord.z);
            bbox_min.x = std::min(bbox_min.x, triangle[i].screenCoord.x);
            bbox_min.y = std::min(bbox_min.y, triangle[i].screenCoord.y);
            bbox_min.z = std::min(bbox_min.z, triangle[i].screenCoord.z);
        }
    }

    bool belongTo3DBBox(Vec3f minp, Vec3f maxp)
    {
        return minp.x < bbox_min.x && minp.y < bbox_min.y && minp.z < bbox_min.z
            && maxp.x > bbox_max.x && maxp.y > bbox_max.y && maxp.z > bbox_max.z;
    }

    float getMaxX()
    {
        return bbox_max.x;
    }

    float getMaxY()
    {
        return bbox_max.y;
    }

    float getMaxZ()
    {
        return bbox_max.z;
    }

    float getMinX()
    {
        return bbox_min.x;
    }

    float getMinY()
    {
        return bbox_min.y;
    }

    float getMinZ()
    {
        return bbox_min.z;
    }

    Vec3f barycentric(Vec2f P)
    {
        Vec2f pts[3];
        for (int i = 0; i < 3; ++i)
        {
            pts[i] = proj<2>(triangle[i].screenCoord);
        }

        Vec2f AB = pts[1] - pts[0];
        Vec2f AC = pts[2] - pts[0];
        Vec2f PA = pts[0] - P;
        Vec3f v1 = Vec3f(AB.x, AC.x, PA.x);
        Vec3f v2 = Vec3f(AB.y, AC.y, PA.y);
        Vec3f res = cross(v1, v2);
        if (std::abs(res.z) < 1e-2)return Vec3f(-1, 1, 1);
        return {1.f - (res.x + res.y) / res.z, res.x / res.z, res.y / res.z};
    }

    float calculateDepth(Vec3f bar)
    {
        return triangle[0].screenCoord.z * bar.x + triangle[1].screenCoord.z * bar.y + triangle[2].
            screenCoord.z * bar.z;
    }
};
