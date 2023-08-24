#pragma once

#include "shader.hpp"
#include "dataStructure.hpp"

class PhongShader : public Shader {
public:
    //mat<2, 3, float> varying_uv;
    //mat<3, 3, float> ndc_tri; // triangle in normalized device coordinates
    //mat<3, 3, float> varying_nrm; // normal per vertex to be interpolated by FS
    //mat<3, 3, float> varying_tri;
    //mat<4, 4, float> uniform_Mshadow; // transform framebuffer screen coordinates to shadowbuffer screen coordinates
    //TGAImage* shadowbuffer;
    //mat<4, 4, float> uniform_M;   //  Projection*ModelView
    //mat<4, 4, float> uniform_MIT; // (Projection*ModelView).invert_transpose()
    PhongShader():Shader() {};
    virtual ~PhongShader() {};
    //virtual Vec4f vertex(int iface, int nthvert);
    virtual bool vertex(Vertex& veretx);
    //virtual bool fragment(Vec3f bar, TGAColor& color);
    virtual bool fragment(Fragment& fragment);
};