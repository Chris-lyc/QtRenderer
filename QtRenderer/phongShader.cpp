#include "phongShader.h"

bool PhongShader::vertex(Vertex& vertex) {
	Vec4f worldCoord_ = Model_ * embed<4>(vertex.worldCoord);
	vertex.worldCoord = proj<3>(worldCoord_ / worldCoord_.w);

    //Vec4f tmp = View_ * embed<4>(vertex.worldCoord);
    //tmp = Projection_ * tmp;
    //tmp = tmp / tmp.w;
    //std::cout << tmp.x << " " << tmp.y << " " << tmp.z << std::endl;

	Vec4f gl_Vertex = ViewPort_ * Projection_ * View_ * embed<4>(vertex.worldCoord);
	vertex.screenCoord = proj<3>(gl_Vertex / gl_Vertex.w);
    Vec4f normal_ = (View_ * Model_).invert_transpose() * embed<4>(vertex.normal, 0.f);
	vertex.normal = proj<3>(normal_);
	return true;
}

bool PhongShader::fragment(Fragment& fragment) {
	//Vec4f sb_p = uniform_Mshadow * embed<4>(varying_tri * bar);
    //sb_p = sb_p / sb_p.w;
    //float shadow = .3 + .7 * ( (shadowbuffer->get(int(sb_p[0] + .5), int(sb_p[1] + .5)) )[0] < sb_p[2] + 0.5);

    float shadow=1.f;
    Vec4f sb_p = uniform_Mshadow * embed<4>(fragment.varying_tri * fragment.bar);
    sb_p = sb_p / sb_p.w;
    int x = int(sb_p[0] + .5);
    int y = int(sb_p[1] + .5);
    //std::cout << sb_p[2] << std::endl;
    
    if (x >= 0 && x < shadowBuffer->width && y >= 0 && y < shadowBuffer->height) {
        //std::cout << shadowBuffer->data[y * shadowBuffer->width + x] << std::endl;
        shadow = .3 + .7 * (shadowBuffer->data[y*shadowBuffer->width+x]+1.f> sb_p[2]);
    }
    //std::cout << shadow << std::endl;
    
    //std::cout << fragment.screenCoord.z << std::endl;

    Vec2f uvCoord = fragment.varying_uv * fragment.bar;

    Vec3f bn = (fragment.varying_nrm * fragment.bar).normalize(); // plane normal
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
    Vec3f normal = normalmap_->sample2D(uvCoord); // tangent normal
    Vec3f n = (B * normal).normalize(); // world-coordinate normal

    Vec3f l = light_dir.normalize();
    // diffuse
    float diff = std::max(0.f, n * l);
    // specular
    Vec3f r = (n * (n * l * 2.f) - l).normalize(); // reflect light
    //std::cout << uv_coord.x <<" "<<uv_coord.y<<" "<< model->specular(uv_coord)<< std::endl;
    float specular = specularmap_->sample2D(uvCoord).x;
    float spec = pow(std::max(r.z, 0.0f), specular); // ????????
    if (specular < 1e-6)spec = std::max(r.z, 0.0f);

    Color color = diffusemap_->sample2D(uvCoord);

    for (int i = 0; i < 3; i++) {
        //color[i] = std::min<float>(20 + (float)c[i] * shadow * (diff + .6 * spec), 255);
        //fragment.color[i] = std::min<float>(((float)c[i]), 255);
        //fragment.color[i] = std::min<float>(((float)c[i] * spec), 255);
        fragment.color[i] = std::min<float>(0 + (float)color[i]  * shadow *(diff + .6 * spec), 255);
        //color[i] = std::min<float>(0 + (float)c[i] * shadow * (diff), 255);
        //color[i] = (r[i] + 1.f) * 255.f / 2.f;
        //fragment.color[i] =100* bn[2-i];
        //color[i] = 255.f;
    }
    //std::cout << fragment.color[0] << " " << fragment.color[1] << " " << fragment.color[2] << std::endl;
    return false;                              // no, we do not discard this pixel
}
