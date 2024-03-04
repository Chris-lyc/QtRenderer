#include "renderInstance.h"
#include "phongShader.h"
#include "depthShader.h"
#include <tbb/parallel_for.h>
#include <tbb/parallel_for_each.h>
#include <tbb/blocked_range2d.h>

RenderInstance::RenderInstance(int width, int height, float depth) : width(width), height(height), depth(depth),
                                                                     camera(nullptr), model(nullptr),
                                                                     multiThread(true), useShadow(false),
                                                                     useCulling(true), zbufferType(0), shadingType(0),
                                                                     shadowFrameBuffer(width, height, zbufferType),
                                                                     colorFrameBuffer(width, height, zbufferType),
                                                                     zpyramid(nullptr),
                                                                     octtree(nullptr)
{
    colorShader = std::make_shared<PhongShader>();
    depthShader = std::make_shared<DepthShader>();
}

void RenderInstance::init(int width, int height, float depth)
{
    getInstance(width, height, depth);
}

RenderInstance& RenderInstance::getInstance(int width, int height, float depth)
{
    static RenderInstance instance(width, height, depth);
    return instance;
}

void RenderInstance::setUseShadow(bool use)
{
    this->useShadow = use;
    if(use==false)
    {
        shadowFrameBuffer.ClearBuffer(Color(255.f, 255.f, 255.f));
    }
}

void RenderInstance::setZbufferType(int type)
{
    this->zbufferType = type;
    if (this->zbufferType == 0)
    {
    }
    else if(this->zbufferType==1)
    {
    }
    else if (this->zbufferType == 2)
    {
        if (zpyramid == nullptr)
        {
            zpyramid = new zPyramid(width, height);
        }
    }
    else if (this->zbufferType == 3)
    {
        if (zpyramid == nullptr)
        {
            zpyramid = new zPyramid(width, height);
        }
    }
}

void RenderInstance::setShadingType(int type)
{
    this->shadingType=type;
}


void RenderInstance::setMultiThread(bool multiThread)
{
    this->multiThread=multiThread;
}

void RenderInstance::constructFragment(std::vector<Vertex>& triangle, Vec3f& bar, Color& color, Fragment& fragment)
{
    for (int nthvert = 0; nthvert < triangle.size(); ++nthvert)
    {
        fragment.varying_uv.set_col(nthvert, triangle[nthvert].uvCoord);
        fragment.varying_nrm.set_col(nthvert, triangle[nthvert].normal);
        fragment.varying_tri.set_col(nthvert, triangle[nthvert].screenCoord);
    }
    fragment.bar = bar;
    fragment.color = color;
    fragment.screenCoord = fragment.varying_tri * bar;
    fragment.uvCoord = fragment.varying_uv * bar;
}

void RenderInstance::processFragment(Primitive& pri, std::shared_ptr<Shader>& shader, FrameBuffer& frameBuffer)
{
    // zbuffer
    if (zbufferType == 0)
    {
        ZbufferRender(pri, shader, frameBuffer);
    }

    // hierarchy zbuffer
    else if (zbufferType == 2)
    {
        HiZRender(pri, zpyramid->root, shader, frameBuffer);
    }
}

void RenderInstance::ZbufferRender(Primitive& primitive, std::shared_ptr<Shader>& shader, FrameBuffer& frameBuffer)
{
    Vec2i P;
    // Color color(255.f, 255.f, 255.f);

    // rasterization (barycentric coordinates)
    for (P.x = std::floor(primitive.getMinX()); P.x <= std::ceil(primitive.getMaxX()); ++P.x)
    {
        for (P.y = std::floor(primitive.getMinY()); P.y <= std::ceil(primitive.getMaxY()); ++P.y)
        {
            if (P.x < 0 || P.x >= width || P.y < 0 || P.y >= height)continue;
            Vec3f bar = primitive.barycentric(P);
            if (bar.x < 0 || bar.y < 0 || bar.z < 0)continue;
            float frag_depth = primitive.calculateDepth(bar);

            Fragment fragment;

            if (frameBuffer.JudgeDepth(P.x, P.y, frag_depth))
            {
                if(shadingType==0)
                {
                    frameBuffer.SetPixel(P.x, P.y, primitive.color);
                }
                else if(shadingType==1)
                {
                    constructFragment(primitive.triangle, bar, primitive.color, fragment);
                    bool discard = shader->fragment(fragment);
                    if (!discard)
                    {
                        frameBuffer.SetPixel(P.x, P.y, fragment.color);
                    }
                }
            }
        }
    }
}

void RenderInstance::HiZRender(Primitive& primitive, zQuadTreeNode* node, std::shared_ptr<Shader>& shader,
                                            FrameBuffer& frameBuffer)
{
    bool tag = true;
    while (tag && primitive.getMaxZ() < node->depth)
    {
        tag = false;
        for (int i = 0; i < 4; ++i)
        {
            if (node->children[i] != nullptr && node->children[i]->containPolygon(primitive))
            {
                tag = true;
                node = node->children[i];
                break;
            }
        }
    }

    // current node is min node, start render
    if (!tag)
    {
        RenderMinNode(primitive, node, shader, frameBuffer);
        node->update();
    }

    // reject render
    else
    {
        return;
    }
}

// TODO:1.每次拿整个primitive去比的 子节点的depth在剔除中都没用上 2.子节点的范围剔除是对整个primitive的，会导致所有子节点都走一遍
float RenderInstance::RenderMinNode(Primitive& primitive, zQuadTreeNode* node, std::shared_ptr<Shader>& shader,
                                    FrameBuffer& frameBuffer)
{
    if (node == nullptr)
    {
        return -1;
    }

    // no need to render
    if (node->maxX < primitive.getMinX() || node->minX > primitive.getMaxX()
        || node->maxY < primitive.getMinY() || node->minY > primitive.getMaxY())
    {
        return node->depth;
    }

    if (node->minX == node->maxX && node->minY == node->maxY)
    {
        int loc = node->minY * width + node->minX;

        Vec2i P = {node->minX, node->minY};
        Vec3f bar = primitive.barycentric(P);

        if (bar.x < 0 || bar.y < 0 || bar.z < 0)return node->depth;

        float z = primitive.calculateDepth(bar);

        // access ztest, shading!
        if (z < zpyramid->zbuffer[loc].depth)
        {
            zpyramid->zbuffer[loc].depth = z;
            
            if(shadingType==0)
            {
                frameBuffer.SetPixel(P.x, P.y, primitive.color);
            }
            else if(shadingType==1)
            {
                Fragment fragment;
                constructFragment(primitive.triangle, bar, primitive.color, fragment);
                bool discard = shader->fragment(fragment);
                if (!discard)
                {
                    frameBuffer.SetPixel(P.x, P.y, fragment.color);
                }
            }
        }
        return zpyramid->zbuffer[loc].depth;
    }
    // update node between leaf node and min node
    else
    {
        float maxZ = 0;
        for (zQuadTreeNode* child : node->children)
        {
            float tmpZ = RenderMinNode(primitive, child, shader, frameBuffer);
            maxZ = std::max(maxZ, tmpZ);
        }
        node->depth = maxZ;
        return node->depth;
    }
}

void RenderInstance::OctHiZRender(octTreeNode* octNode, zQuadTreeNode* quadNode, std::shared_ptr<Shader>& shader,
                                  FrameBuffer& frameBuffer)
{
    // not overlap, reject
    if(!quadNode->overlapBBox(octNode->minPoint.x,octNode->minPoint.y,octNode->maxPoint.x,octNode->maxPoint.y))
    {
        return;
    }
    
    bool tag = true;
    while (tag && octNode->getMaxZ() < quadNode->depth)
    {
        tag = false;
        for (int i = 0; i < 4; ++i)
        {
            if (quadNode->children[i] != nullptr && quadNode->children[i]->containBBox(
                octNode->minPoint.x, octNode->minPoint.y, octNode->maxPoint.x, octNode->maxPoint.y))
            {
                tag = true;
                quadNode = quadNode->children[i];
                break;
            }
        }
    }

    // current quad node is minest for current oct node
    if (!tag)
    {
        if(multiThread)
        {
            tbb::parallel_for(tbb::blocked_range<size_t>(0,octNode->faceIndex.size()),[&](tbb::blocked_range<size_t> r)
            {
               for(size_t i=r.begin();i<r.end();++i)
               {
                   RenderMinNode(octtree->mesh->primitives[octNode->faceIndex[i]],quadNode,shader,frameBuffer);
                   quadNode->update();
               } 
            });
        }
        else
        {
            for(int index:octNode->faceIndex)
            {
                RenderMinNode(octtree->mesh->primitives[index], quadNode, shader, frameBuffer);
                quadNode->update();
            }
        }
        
        if(!octNode->isLeaf)
        {
            if(multiThread)
            {
                tbb::parallel_for(tbb::blocked_range<size_t>(0,8),[&](tbb::blocked_range<size_t> r)
                {
                    for(size_t i=r.begin();i<r.end();++i)
                    {
                        if(octNode->children[i]!=nullptr)
                        {
                            OctHiZRender(octNode->children[i],quadNode,shader,frameBuffer);
                        }
                    }
                });
            }
            else
            {
                for(int i=0;i<8;++i)
                {
                    if(octNode->children[i]!=nullptr)
                    {
                        OctHiZRender(octNode->children[i],quadNode,shader,frameBuffer);
                    }
                }
            }
        }
    }

    // octNode->getMaxZ() >= quadNode->depth, reject render
    else
    {
        return;
    }
}

void RenderInstance::ScanlineZbufferRender(ScanlineZbuffer* scanZbuffer, std::shared_ptr<Shader>& shader, FrameBuffer& frameBuffer)
{
    for(int y=height-1;y>=0;--y)
    {
        scanZbuffer->activePolygonList->addFrontRange(scanZbuffer->selectedPolygonListVec[y]);
        PolygonListNode* node=scanZbuffer->activePolygonList->head->next;
        while(node!=nullptr)
        {
            int x1=std::ceil(std::min(node->xl,node->xr));
            int x2=std::floor(std::max(node->xl,node->xr));
            float depth=node->zl;
            for(int x=x1;x<=x2;++x)
            {
                if(x<0)continue;
                if(x>=width)break;
                if(frameBuffer.JudgeDepth(x,y,depth))
                {
                    if(shadingType==0)
                    {
                        frameBuffer.SetPixel(x, y, node->color);
                    }
                    
                    // else if(shadingType==1)
                    // {
                    //     Primitive pri=scanZbuffer->mesh->primitives[node->id];
                    //     Fragment fragment;
                    //     Vec3f bar = pri.barycentric(Vec2f(x,y));
                    //     if (bar.x < 0 || bar.y < 0 || bar.z < 0)continue;
                    //     
                    //     constructFragment(pri.triangle, bar, node->color, fragment);
                    //     bool discard = shader->fragment(fragment);
                    //     if (!discard)
                    //     {
                    //         frameBuffer.SetPixel(x, y, fragment.color);
                    //     }
                    // }
                }    
            }
            
            node->scanlineMove(y);
            PolygonListNode* tmp=node;
            node=node->next;
            if(tmp->dy<0)
            {
                scanZbuffer->activePolygonList->removeNode(tmp);
            }
        }
    }
}

void RenderInstance::processVertex(Primitive& pri, std::shared_ptr<Shader>& shader)
{
    for (int i = 0; i < 3; ++i)
    {
        shader->vertex(pri.triangle[i]);
    }
    pri.calculateBBox();
}

void RenderInstance::renderPrimitive(Primitive pri, std::shared_ptr<Shader>& shader, FrameBuffer& frameBuffer)
{
    processVertex(pri, shader);
    processFragment(pri, shader, frameBuffer);
}

void RenderInstance::render()
{
    for (Mesh mesh : model->meshes)
    {
        int tmpZbufferType=zbufferType;
        if(shadingType==1&&zbufferType==1)
        {
            zbufferType=0;
        }
        
        if (multiThread)
        {
            if (useShadow)
            {
                // use raw zbuffer to do shadow
                int tmpType = zbufferType;
                setZbufferType(0);

                static_cast<DepthShader*>(depthShader.get())->depth = depth;
                tbb::parallel_for(tbb::blocked_range<size_t>(0, mesh.primitives.size()),[&](tbb::blocked_range<size_t> r)
                {
                    for (size_t i = r.begin(); i < r.end(); ++i)
                    {
                        renderPrimitive(mesh.primitives[i], depthShader, shadowFrameBuffer);
                    }
                });

                static_cast<PhongShader*>(colorShader.get())->shadowBuffer = shadowFrameBuffer.GetDepthBuffer();
                Matrix M = depthShader->ViewPort_ * depthShader->Projection_ * depthShader->View_ * depthShader->Model_;
                static_cast<PhongShader*>(colorShader.get())->uniform_Mshadow =
                    M * (colorShader->ViewPort_ * colorShader->Projection_ * colorShader->View_ * colorShader->Model_).invert();

                setZbufferType(tmpType);
            }
            // each primitive do vertex, do fragment
            if (zbufferType == 0 || zbufferType == 2)
            {
                tbb::parallel_for(tbb::blocked_range<size_t>(0, mesh.primitives.size()),[&](tbb::blocked_range<size_t> r)
                {
                    for (size_t i = r.begin(); i < r.end(); ++i)
                    {
                        if (useCulling)
                            if (backgroundCulling(mesh.primitives[i]))
                                continue;
                        renderPrimitive(mesh.primitives[i], colorShader, colorFrameBuffer);
                    }
                });
            }
            // all primitive do vertex, build octtree, do fragment
            if (zbufferType == 1 || zbufferType== 3)
            {
                tbb::parallel_for(tbb::blocked_range<size_t>(0, mesh.primitives.size()),[&](tbb::blocked_range<size_t> r)
                {
                    for (size_t i = r.begin(); i < r.end(); ++i)
                    {
                        if (useCulling)
                            if (backgroundCulling(mesh.primitives[i]))
                                continue;
                        processVertex(mesh.primitives[i], colorShader);
                    }
                });

                if(zbufferType==1)
                {
                    ScanlineZbuffer* scanlineZbuffer=new ScanlineZbuffer(&mesh,width,height);
                    ScanlineZbufferRender(scanlineZbuffer,colorShader,colorFrameBuffer);
                    delete scanlineZbuffer;
                }
                else if(zbufferType==3)
                {
                    octtree = new octTree(&mesh,multiThread);
                    OctHiZRender(octtree->root,zpyramid->root,colorShader,colorFrameBuffer);
                    delete octtree;
                }
            }
        }
        else
        {
            if (useShadow)
            {
                // use raw zbuffer to do shadow
                int tmpType = zbufferType;
                setZbufferType(0);

                static_cast<DepthShader*>(depthShader.get())->depth = depth;
                for (size_t i = 0; i < mesh.primitives.size(); ++i)
                {
                    renderPrimitive(mesh.primitives[i], depthShader, shadowFrameBuffer);
                }

                static_cast<PhongShader*>(colorShader.get())->shadowBuffer = shadowFrameBuffer.GetDepthBuffer();
                Matrix M = depthShader->ViewPort_ * depthShader->Projection_ * depthShader->View_ * depthShader->Model_;
                static_cast<PhongShader*>(colorShader.get())->uniform_Mshadow =
                    M * (colorShader->ViewPort_ * colorShader->Projection_ * colorShader->View_ * colorShader->Model_).invert();

                setZbufferType(tmpType);
            }
            // each primitive do vertex, do fragment
            if (zbufferType == 0 || zbufferType == 2)
            {
                for (size_t i = 0; i < mesh.primitives.size(); ++i)
                {
                    if (useCulling)
                        if (backgroundCulling(mesh.primitives[i]))
                            continue;
                    renderPrimitive(mesh.primitives[i], colorShader, colorFrameBuffer);
                }
            }
            
            // all primitive do vertex, build octtree, do fragment
            if (zbufferType == 1 || zbufferType== 3)
            {
                for (size_t i = 0; i < mesh.primitives.size(); ++i)
                {
                    if (useCulling)
                        if (backgroundCulling(mesh.primitives[i]))
                            continue;
                    processVertex(mesh.primitives[i], colorShader);
                }

                if(zbufferType==1)
                {
                    ScanlineZbuffer* scanlineZbuffer=new ScanlineZbuffer(&mesh,width,height);
                    ScanlineZbufferRender(scanlineZbuffer,colorShader,colorFrameBuffer);
                    delete scanlineZbuffer;
                }
                else if(zbufferType==3)
                {
                    octtree = new octTree(&mesh,multiThread);
                    OctHiZRender(octtree->root,zpyramid->root,colorShader,colorFrameBuffer);
                }
            }
        }

        zbufferType=tmpZbufferType;
    }
}

void RenderInstance::saveImage(QString& filePath)
{
    colorFrameBuffer.SaveImage(filePath);
    //shadowFrameBuffer.SaveImage(filePath);
}

void RenderInstance::setTextureToShader()
{
    std::static_pointer_cast<PhongShader>(colorShader)->diffusemap_ = model->diffusemap_;
    std::static_pointer_cast<PhongShader>(colorShader)->normalmap_ = model->normalmap_;
    std::static_pointer_cast<PhongShader>(colorShader)->specularmap_ = model->specularmap_;
}

bool RenderInstance::backgroundCulling(Primitive& pri)
{
    Vec3f t0t1 = pri.triangle[1].worldCoord - pri.triangle[0].worldCoord;
    Vec3f t0t2 = pri.triangle[2].worldCoord - pri.triangle[0].worldCoord;
    Vec3f n = cross(t0t1, t0t2);
    Vec3f avgPoint = (pri.triangle[0].worldCoord + pri.triangle[1].worldCoord + pri.triangle[2].worldCoord) / 3.f;
    Vec3f cameraToTri = avgPoint - camera->getPosition();
    if (n * cameraToTri > 0)
    {
        pri.isCulled=true;
        return true;
    }
    return false;
}
