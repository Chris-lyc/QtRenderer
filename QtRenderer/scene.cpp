#include "scene.h"
#include "phongShader.h"
#include "depthShader.h"
#include <tbb/parallel_for.h>
#include <tbb/parallel_for_each.h>
#include <tbb/blocked_range2d.h>

Scene::Scene(int width, int height) :width(width), height(height),
multiThread(true), frameBuffer(width, height) {
    shader = std::make_unique<PhongShader>();
}

void Scene::init(int width, int height) {
    getInstance(width, height);
}

Scene& Scene::getInstance(int width, int height) {
    static Scene instance(width, height);
    return instance;
}

Vec3f Scene::barycentric(Vec2f* pts, Vec2f P) {
    Vec2f AB = pts[1] - pts[0];
    Vec2f AC = pts[2] - pts[0];
    Vec2f PA = pts[0] - P;
    Vec3f v1 = Vec3f(AB.x, AC.x, PA.x);
    Vec3f v2 = Vec3f(AB.y, AC.y, PA.y);
    Vec3f res = cross(v1, v2);
    if (std::abs(res.z) < 1e-2)return Vec3f(-1, 1, 1);
    return { 1.f - (res.x + res.y) / res.z ,res.x / res.z,res.y / res.z };
}

void Scene::constructFragment(std::vector<Vertex>& triangle, Vec3f& bar, TGAColor& color, Fragment& fragment) {
    for (int nthvert = 0; nthvert < triangle.size(); ++nthvert) {
        fragment.varying_uv.set_col(nthvert, triangle[nthvert].uvCoord);
        fragment.varying_nrm.set_col(nthvert, triangle[nthvert].normal);
        fragment.varying_tri.set_col(nthvert, triangle[nthvert].screenCoord);
    }
    fragment.bar = bar;
    fragment.color = color;
    fragment.uvCoord = fragment.varying_uv * fragment.bar;
    fragment.normal = normalmap_.sample2D(fragment.uvCoord);
    Vec3i diffuse = diffusemap_.sample2D(fragment.uvCoord);
    fragment.diffuse = TGAColor(diffuse.x,diffuse.y,diffuse.z);
    fragment.specular = specularmap_.sample2D(fragment.uvCoord).x;
}

void Scene::triangle(std::vector<Vertex>& triangle) {
    Vec2f bbox_max = { -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max() };
    Vec2f bbox_min = { std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };
    Vec2f clamp(frameBuffer.get_width() - 1, frameBuffer.get_height() - 1);
    for (int i = 0; i < 3; i++) {
        bbox_max.x = std::max(bbox_max.x, triangle[i].screenCoord.x);
        bbox_max.y = std::max(bbox_max.y, triangle[i].screenCoord.y);
        bbox_min.x = std::min(bbox_min.x, triangle[i].screenCoord.x);
        bbox_min.y = std::min(bbox_min.y, triangle[i].screenCoord.y);
    }

    Vec2f pts_bar[3];
    for (int i = 0; i < 3; ++i) {
        pts_bar[i] = proj<2>(triangle[i].screenCoord);
    }

    Vec2i P;
    TGAColor color(255.f,255.f,255.f);
    for (P.x = std::floor(bbox_min.x); P.x <= std::ceil(bbox_max.x); ++P.x) {
        for (P.y = std::floor(bbox_min.y); P.y <= std::ceil(bbox_max.y); ++P.y) {
            if (P.x<0 || P.x>=width || P.y<0 || P.y>=height)continue;
            Vec3f bar = barycentric(pts_bar, P);
            if (bar.x < 0 || bar.y < 0 || bar.z < 0)continue;
            float frag_depth = triangle[0].screenCoord.z * bar.x + triangle[1].screenCoord.z * bar.y + triangle[2].screenCoord.z * bar.z;
            //float w = pts[0][3] * bar.x + pts[1][3] * bar.y + pts[2][3] * bar.z;
            //float frag_depth = z / w;
            Fragment fragment;
            
            if (frameBuffer.JudgeDepth(P.x, P.y, frag_depth)) {
                constructFragment(triangle, bar, color, fragment);

                bool discard = shader->fragment(fragment);
                if (!discard) {
                    //zbuffer.set(P.x, P.y, TGAColor(frag_depth));
                    frameBuffer.SetPixel(P.x, P.y, fragment.color);
                }
            }
            //if (frag_depth > zbuffer.get(P.x,P.y)[0]) {
            //    bool discard = shader.fragment(bar, color);
            //    if (!discard) {
            //        zbuffer.set(P.x, P.y, TGAColor(frag_depth));
            //        image.set(P.x, P.y, color);
            //    }
            //}
        }
    }
}

void Scene::processTriangle(std::vector<Vertex> &tri) {
    for (int i = 0; i < 3; ++i){
        shader->vertex(tri[i]);
    }
    triangle(tri);

    //std::vector<Triangle> completedTriangleList = ClipTriangle(tri);
    //for (auto& ctri : completedTriangleList)
    //{
    //    ExecutePerspectiveDivision(ctri);
    //    ConvertToScreen(ctri);
    //    if (renderMode == FACE)RasterizationTriangle(ctri);
    //    else if (renderMode == EDGE) WireframedTriangle(ctri);
    //    else if (renderMode == VERTEX) PointedTriangle(ctri);
    //}
}



void Scene::render() {
    std::vector<std::vector<Vertex>> triangles;
    for (int i = 0; i < indices.size(); i += 3)
    {
        assert(i + 1 < indices.size() && i + 2 < indices.size());
        triangles.push_back({ vertices.at(indices.at(i)), vertices.at(indices.at(i + 1)), vertices.at(indices.at(i + 2)) });
    }
    if (multiThread) {
        tbb::parallel_for(tbb::blocked_range<size_t>(0, triangles.size()), [&](tbb::blocked_range<size_t> r) {
            for (size_t i = r.begin(); i < r.end(); i++) {
                processTriangle(triangles[i]);
            }
        });
    }
    else {
        for (int i = 0; i < triangles.size(); i++) {
            processTriangle(triangles[i]);
        }
    }
    //std::cout << model->nfaces() << " faces finish!" << std::endl;
    //frameBuffer.SaveImage("output/test.png");
    //Image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    //Image.write_tga_file("output/output.tga");
    //zbuffer.flip_vertically();
    //zbuffer.write_tga_file("output/zbuffer.tga");
    //delete model;
}

void Scene::saveImage(QString filePath) {
    frameBuffer.SaveImage(filePath);
}
