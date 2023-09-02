#include "renderInstance.h"
#include "phongShader.h"
#include "depthShader.h"
#include <tbb/parallel_for.h>
#include <tbb/parallel_for_each.h>
#include <tbb/blocked_range2d.h>

RenderInstance::RenderInstance(int width, int height, float depth) :width(width), height(height), depth(depth),
multiThread(true), colorFrameBuffer(width, height), shadowFrameBuffer(width, height), useShadow(false) {
    colorShader = std::make_shared<PhongShader>();
    depthShader = std::make_shared<DepthShader>();
}

void RenderInstance::init(int width, int height, float depth) {
    getInstance(width, height, depth);
}

RenderInstance& RenderInstance::getInstance(int width, int height, float depth) {
    static RenderInstance instance(width, height, depth);
    return instance;
}

void RenderInstance::setUseShadow(bool use) {
    this->useShadow = use;
}

Vec3f RenderInstance::barycentric(Vec2f* pts, Vec2f P) {
    Vec2f AB = pts[1] - pts[0];
    Vec2f AC = pts[2] - pts[0];
    Vec2f PA = pts[0] - P;
    Vec3f v1 = Vec3f(AB.x, AC.x, PA.x);
    Vec3f v2 = Vec3f(AB.y, AC.y, PA.y);
    Vec3f res = cross(v1, v2);
    if (std::abs(res.z) < 1e-2)return Vec3f(-1, 1, 1);
    return { 1.f - (res.x + res.y) / res.z ,res.x / res.z,res.y / res.z };
}

void RenderInstance::constructFragment(std::vector<Vertex>& triangle, Vec3f& bar, Color& color, Fragment& fragment) {
    for (int nthvert = 0; nthvert < triangle.size(); ++nthvert) {
        fragment.varying_uv.set_col(nthvert, triangle[nthvert].uvCoord);
        fragment.varying_nrm.set_col(nthvert, triangle[nthvert].normal);
        fragment.varying_tri.set_col(nthvert, triangle[nthvert].screenCoord);
    }
    fragment.bar = bar;
    fragment.color = color;
    fragment.screenCoord = fragment.varying_tri * bar;
    fragment.uvCoord = fragment.varying_uv * bar;
}

void RenderInstance::triangle(std::vector<Vertex>& triangle, std::shared_ptr<Shader>& shader, FrameBuffer& frameBuffer) {
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
    Color color(255.f, 255.f, 255.f);
    for (P.x = std::floor(bbox_min.x); P.x <= std::ceil(bbox_max.x); ++P.x) {
        for (P.y = std::floor(bbox_min.y); P.y <= std::ceil(bbox_max.y); ++P.y) {
            if (P.x < 0 || P.x >= width || P.y < 0 || P.y >= height)continue;
            Vec3f bar = barycentric(pts_bar, P);
            if (bar.x < 0 || bar.y < 0 || bar.z < 0)continue;
            float frag_depth = triangle[0].screenCoord.z * bar.x + triangle[1].screenCoord.z * bar.y + triangle[2].screenCoord.z * bar.z;
            Fragment fragment;

            if (frameBuffer.JudgeDepth(P.x, P.y, frag_depth)) {
                constructFragment(triangle, bar, color, fragment);

                bool discard = shader->fragment(fragment);
                if (!discard) {
                    frameBuffer.SetPixel(P.x, P.y, fragment.color);
                }
            }
        }
    }
}

void RenderInstance::processTriangle(std::vector<Vertex> tri, std::shared_ptr<Shader>& shader,FrameBuffer& frameBuffer) {

    for (int i = 0; i < 3; ++i) {
        shader->vertex(tri[i]);
    }
    triangle(tri, shader, frameBuffer);
}



void RenderInstance::render() {
    std::vector<std::vector<Vertex>> triangles;
    for (int i = 0; i < indices.size(); i += 3)
    {
        assert(i + 1 < indices.size() && i + 2 < indices.size());
        triangles.push_back({ vertices.at(indices.at(i)), vertices.at(indices.at(i + 1)), vertices.at(indices.at(i + 2)) });
    }

    std::static_pointer_cast<PhongShader>(colorShader)->diffusemap_ = diffusemap_;
    std::static_pointer_cast<PhongShader>(colorShader)->normalmap_ = normalmap_;
    std::static_pointer_cast<PhongShader>(colorShader)->specularmap_ = specularmap_;
    

    if (multiThread) {
        if (useShadow) {
            dynamic_cast<DepthShader*>(depthShader.get())->depth = depth;
            tbb::parallel_for(tbb::blocked_range<size_t>(0, triangles.size()), [&](tbb::blocked_range<size_t> r) {
                for (size_t i = r.begin(); i < r.end(); i++) {
                    processTriangle(triangles[i], depthShader,shadowFrameBuffer);
                }
            });

            
            dynamic_cast<PhongShader*>(colorShader.get())->shadowBuffer = shadowFrameBuffer.GetDepthBuffer();
            Matrix M = depthShader->ViewPort_ * depthShader->Projection_ * depthShader->View_ * depthShader->Model_;
            dynamic_cast<PhongShader*>(colorShader.get())->uniform_Mshadow = 
                M * (colorShader->ViewPort_ * colorShader->Projection_ * colorShader->View_ * colorShader->Model_).invert();
            
            tbb::parallel_for(tbb::blocked_range<size_t>(0, triangles.size()), [&](tbb::blocked_range<size_t> r) {
                for (size_t i = r.begin(); i < r.end(); i++) {
                    processTriangle(triangles[i], colorShader, colorFrameBuffer);
                }
            });
        }
        else {
            tbb::parallel_for(tbb::blocked_range<size_t>(0, triangles.size()), [&](tbb::blocked_range<size_t> r) {
                for (size_t i = r.begin(); i < r.end(); i++) {
                    processTriangle(triangles[i], colorShader, colorFrameBuffer);
                }
            });
        }
    }
    else {
        for (int i = 0; i < triangles.size(); i++) {
            processTriangle(triangles[i], colorShader, colorFrameBuffer);
        }
    }
}

void RenderInstance::saveImage(QString& filePath) {
    colorFrameBuffer.SaveImage(filePath);
    //shadowFrameBuffer.SaveImage(filePath);
}
