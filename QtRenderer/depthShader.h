#pragma once

#include "shader.hpp"

class DepthShader :public Shader {
public:
    //mat<3, 3, float> varying_tri;
    float depth;
    DepthShader(){};
    virtual ~DepthShader() {};
    virtual bool vertex(Vertex& vertex);
    virtual bool fragment(Fragment& fragment);
};