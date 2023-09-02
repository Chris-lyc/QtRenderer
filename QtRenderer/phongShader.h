#pragma once

#include "shader.hpp"
#include "dataStructure.hpp"
#include "texture.h"
#include <QImage>

class PhongShader : public Shader {
public:
    Texture* diffusemap_;
    Texture* normalmap_;
    Texture* specularmap_;

    Buffer* shadowBuffer;
    Matrix uniform_Mshadow; // transform framebuffer screen coordinates to shadowbuffer screen coordinates
    PhongShader(){};
    virtual ~PhongShader() {};
    virtual bool vertex(Vertex& veretx);
    virtual bool fragment(Fragment& fragment);
};