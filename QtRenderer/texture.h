#pragma once

#include "tgaimage.h"
#include "geometry.h"


class Texture {
public:
    enum Type {
        DEFAULT,
        DIFFUSE,
        NORMAL,
        SPECLUAR
    };
    
    Texture() : width(0), height(0),type(DEFAULT) {};
    void loadImage(std::string filename, const char* suffix, Type type);
    Vec3f sample2D(Vec2f uvCoord);
private:
	TGAImage map;
    int width;
    int height;
    Type type;
    std::string path;
};