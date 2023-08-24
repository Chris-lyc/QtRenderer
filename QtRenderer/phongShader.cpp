#include "phongShader.h"

bool PhongShader::vertex(Vertex& vertex) {
	Vec4f worldCoord_ = Model_ * embed<4>(vertex.worldCoord);
	vertex.worldCoord = proj<3>(worldCoord_ / worldCoord_.w);
    //Vec4f tmp= Projection_ * View_ * embed<4>(vertex.worldCoord);
    //Vec3f tmp_3 = proj<3>(tmp / tmp.w);
	Vec4f gl_Vertex = ViewPort_ * Projection_ * View_ * embed<4>(vertex.worldCoord);
	vertex.screenCoord = proj<3>(gl_Vertex / gl_Vertex.w);
    Vec4f normal_ = (View_ * Model_).invert_transpose() * embed<4>(vertex.normal, 0.f);
	vertex.normal = proj<3>(normal_);
	return true;
}

bool PhongShader::fragment(Fragment& fragment) {
	//Vec4f sb_p = uniform_Mshadow * embed<4>(varying_tri * bar);
    //sb_p = sb_p / sb_p.w;
    //float shadow = .3 + .7 * ((shadowbuffer->get(int(sb_p[0] + .5), int(sb_p[1] + .5)))[0] < sb_p[2] + 0.5);

    Vec3f bn = (fragment.varying_nrm * fragment.bar).normalize();
    mat<3, 3, float> A;
    A[0] = fragment.varying_tri.col(1) - fragment.varying_tri.col(0);
    A[1] = fragment.varying_tri.col(2) - fragment.varying_tri.col(0);
    A[2] = bn;
    mat<3, 3, float> AI = A.invert();
    Vec3f i = (AI * Vec3f(fragment.varying_uv[0][1] - fragment.varying_uv[0][0], fragment.varying_uv[0][2] - fragment.varying_uv[0][0], 0.f)).normalize();
    Vec3f j = (AI * Vec3f(fragment.varying_uv[1][1] - fragment.varying_uv[1][0], fragment.varying_uv[1][2] - fragment.varying_uv[1][0], 0.f)).normalize();
    mat<3, 3, float> B;
    B.set_col(0, i);
    B.set_col(1, j);
    B.set_col(2, bn);
    Vec3f n = (B * fragment.normal).normalize(); // world-coordinate normal
    Vec3f l = light_dir;
    // diffuse
    float diff = std::max(0.f, n * l);
    // specular
    Vec3f r = (n * (n * l * 2.f) - l).normalize(); // reflect light
    //std::cout << uv_coord.x <<" "<<uv_coord.y<<" "<< model->specular(uv_coord)<< std::endl;
    float spec = pow(std::max(r.z, 0.0f), fragment.specular); // ????????
    if (fragment.specular < 1e-6)spec = std::max(r.z, 0.0f);

    TGAColor c = fragment.diffuse;
    for (int i = 0; i < 3; i++) {
        //color[i] = std::min<float>(20 + (float)c[i] * shadow * (diff + .6 * spec), 255);
        //fragment.color[i] = std::min<float>(((float)c[i]), 255);
        //fragment.color[i] = std::min<float>(((float)c[i] * spec), 255);
        fragment.color[i] = std::min<float>(0 + (float)c[i] * (diff + .6 * spec), 255);
        //color[i] = std::min<float>(0 + (float)c[i] * shadow * (diff), 255);
        //color[i] = (r[i] + 1.f) * 255.f / 2.f;
        //fragment.color[i] =100* bn[2-i];
        //color[i] = 255.f;
    }
    return false;                              // no, we do not discard this pixel
}
