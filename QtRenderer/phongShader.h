#pragma once

#include "shader.hpp"
#include "dataStructure.hpp"
#include "texture.h"
#include <QImage>

class PhongShader : public Shader {
public:
    //mat<2, 3, float> varying_uv; // uv coords
    //mat<3, 3, float> varying_nrm; // normal coords
    //mat<3, 3, float> varying_tri; // screen coords
    //float depth;
    Texture* diffusemap_;
    Texture* normalmap_;
    Texture* specularmap_;
    //std::shared_ptr<Texture> diffusemap_;
    //std::shared_ptr<Texture> normalmap_;
    //std::shared_ptr<Texture> specularmap_;

    Buffer* shadowBuffer;
    Matrix uniform_Mshadow; // transform framebuffer screen coordinates to shadowbuffer screen coordinates
    //TGAImage* shadowbuffer;
    //mat<4, 4, float> uniform_M;   //  Projection*ModelView
    //mat<4, 4, float> uniform_MIT; // (Projection*ModelView).invert_transpose()
    PhongShader(){};
    virtual ~PhongShader() {};
    //virtual bool loadTexture(std::shared_ptr<Texture> map);
    virtual bool vertex(Vertex& veretx);
    virtual bool fragment(Fragment& fragment);
};