#pragma once

#include "shader.hpp"

class DepthShader :public Shader {
public:
    float depth;
    DepthShader(){};
    virtual ~DepthShader() {};
    virtual bool vertex(Vertex& vertex);
    virtual bool fragment(Fragment& fragment);
};