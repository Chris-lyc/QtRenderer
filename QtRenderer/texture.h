#pragma once

#include "geometry.h"
#include "stb_image.h"
#include "dataStructure.hpp"

class Texture {
public:
    Texture() : width(0), height(0), type(DEFAULT), channels(0), map(nullptr) {};
    ~Texture() { if(map!=NULL)stbi_image_free(map); }
    TextureType getType();
    void loadImage(std::string filename, const char* suffix, TextureType type);
    Color sample2D(Vec2f uvCoord);
private:
    unsigned char* map;
    int width;
    int height;
    int channels;
    TextureType type;
};