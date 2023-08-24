#pragma once

#include "shader.hpp"

class DepthShader :public Shader {
public:
    mat<3, 3, float> varying_tri;
    DepthShader():Shader() {};
    virtual ~DepthShader() {};
    virtual Vec4f vertex(int iface, int nthvert);
    virtual bool fragment(Vec3f bar, TGAColor& color);
};