#include "depthShader.h"

//Vec4f DepthShader::vertex(int iface, int nthvert) {
//    Vec4f gl_Vertex = ViewPort_ * Projection_ * View_ * embed<4>(model->vert(iface, nthvert));
//    varying_tri.set_col(nthvert, proj<3>(gl_Vertex / gl_Vertex[3]));
//    return gl_Vertex;
//}
//
//bool DepthShader::fragment(Vec3f bar, TGAColor& color) {
//    Vec3f p = varying_tri * bar;
//    color = TGAColor(255, 255, 255) * (p.z / depth);
//    return false;
//}