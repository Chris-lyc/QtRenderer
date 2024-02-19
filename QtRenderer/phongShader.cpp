#include "phongShader.h"

bool PhongShader::vertex(Vertex& vertex)
{
    Vec4f worldCoord_ = Model_ * embed<4>(vertex.worldCoord);
    vertex.worldCoord = proj<3>(worldCoord_ / worldCoord_.w);

    Vec4f gl_Vertex = ViewPort_ * Projection_ * View_ * embed<4>(vertex.worldCoord);
    vertex.screenCoord = proj<3>(gl_Vertex / gl_Vertex.w);
    
    Vec4f normal_ = (View_ * Model_).invert_transpose() * embed<4>(vertex.normal, 0.f);
    vertex.normal = proj<3>(normal_);

    return true;
}

bool PhongShader::fragment(Fragment& fragment)
{
    if(diffusemap_==nullptr||normalmap_==nullptr||specularmap_==nullptr)
    {
        return false;
    }
    
    float shadow = 1.f;
    Vec4f sb_p = uniform_Mshadow * embed<4>(fragment.varying_tri * fragment.bar);
    sb_p = sb_p / sb_p.w;
    int x = static_cast<int>(sb_p[0] + .5);
    int y = static_cast<int>(sb_p[1] + .5);

    if (x >= 0 && x < shadowBuffer->width && y >= 0 && y < shadowBuffer->height)
    {
        shadow = .3f + .7f * (shadowBuffer->getPixel(x, y) + 1.f > sb_p[2]);
    }

    Vec2f uvCoord = fragment.varying_uv * fragment.bar;

    Vec3f bn = (fragment.varying_nrm * fragment.bar).normalize(); // plane normal
    mat<3, 3, float> A;
    A[0] = fragment.varying_tri.col(1) - fragment.varying_tri.col(0);
    A[1] = fragment.varying_tri.col(2) - fragment.varying_tri.col(0);
    A[2] = bn;
    mat<3, 3, float> AI = A.invert();
    Vec3f i = (AI * Vec3f(fragment.varying_uv[0][1] - fragment.varying_uv[0][0],
                          fragment.varying_uv[0][2] - fragment.varying_uv[0][0], 0.f)).normalize();
    Vec3f j = (AI * Vec3f(fragment.varying_uv[1][1] - fragment.varying_uv[1][0],
                          fragment.varying_uv[1][2] - fragment.varying_uv[1][0], 0.f)).normalize();
    mat<3, 3, float> B;
    B.set_col(0, i);
    B.set_col(1, j);
    B.set_col(2, bn);
    Vec3f normal={0,1,0};
    if(normalmap_!=nullptr)
        normal = normalmap_->sample2D(uvCoord); // tangent normal
    
    Vec3f n = (B * normal).normalize(); // world-coordinate normal

    Vec3f l = light_dir.normalize();
    // diffuse
    float diff = std::max(0.f, n * l);
    // specular
    Vec3f r = (n * (n * l * 2.f) - l).normalize(); // reflect light
    float spec = 0;
    if(specularmap_!=nullptr)
    {
        float specular = specularmap_->sample2D(uvCoord).x;
        spec = pow(std::max(r.z, 0.0f), specular);
        if (specular < 1e-6)spec = std::max(r.z, 0.0f);
    }

    Color color(50,50,50);
    if(diffusemap_!=nullptr)
        color=diffusemap_->sample2D(uvCoord);
    for (int i = 0; i < 3; i++)
    {
        fragment.color[i] = std::max<float>(0,std::min<float>(0 + (float)color[i] * shadow * (diff + .6 * spec), 255.f));
    }
    return false; // no, we do not discard this pixel
}
