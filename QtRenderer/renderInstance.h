#pragma once

#include "geometry.h"
#include "model.h"
#include "framebuffer.h"
#include "dataStructure.hpp"
#include "texture.h"
#include "shader.hpp"
#include "camera.h"
#include <vector>

#include "OctTree/octTree.h"
#include "Scanline Zbuffer/ScanlineZbuffer.h"

class RenderInstance {
public:
	std::shared_ptr<Shader> colorShader;
	std::shared_ptr<Shader> depthShader;
	Camera* camera;
	Model* model;
private:
	int width;
	int height;
	float depth;

	bool multiThread;
	bool useShadow;
	bool useCulling;
	int zbufferType;
	int shadingType;
	
	FrameBuffer shadowFrameBuffer;
	FrameBuffer colorFrameBuffer;

	zPyramid* zpyramid;
	octTree* octtree;
	// PolygonList* polygonList;

public:
	RenderInstance(int width, int height, float depth);
	void render();
	static void init(int width, int height, float depth);
	static RenderInstance& getInstance(int width = 0, int height = 0, float depth = 0.f);
	
	void setUseShadow(bool use);
	void setZbufferType(int type);
	void setMultiThread(bool multiThread);
	void setShadingType(int type);

	void setTextureToShader();

	QImage& getBuffer() {
		return colorFrameBuffer.GetImage();
	}

	void clearBuffer() {
		if(useShadow)
			shadowFrameBuffer.ClearBuffer(Color(255.f, 255.f, 255.f));
		if(zbufferType==0||zbufferType==1)
			colorFrameBuffer.ClearBuffer(Color(255.f, 255.f, 255.f));
		else if(zbufferType==2||zbufferType==3)
			colorFrameBuffer.ClearColorBuffer(Color(255.f, 255.f, 255.f));
	}

	void clearZPyramid()
	{
		zpyramid->resetPyramid(FLT_MAX,zpyramid->root);
	}

	void renderPrimitive(Primitive pri, std::shared_ptr<Shader>& shader, FrameBuffer& frameBuffer);
	void processVertex(Primitive& pri, std::shared_ptr<Shader>& shader);
	void processFragment(Primitive& pri, std::shared_ptr<Shader>& shader, FrameBuffer& frameBuffer);

	void constructFragment(std::vector<Vertex>& triangle, Vec3f& bar, Color& color, Fragment& fragment);

	void saveImage(QString& path);

	bool backgroundCulling(Primitive& pri);

	void ZbufferRender(Primitive& primitive, std::shared_ptr<Shader>& shader, FrameBuffer& frameBuffer);
	void HiZRender(Primitive& primitive,zQuadTreeNode* node,std::shared_ptr<Shader>& shader, FrameBuffer& frameBuffer);
	float RenderMinNode(Primitive& primitive,zQuadTreeNode* node,std::shared_ptr<Shader>& shader, FrameBuffer& frameBuffer);
	void OctHiZRender(octTreeNode* octNode, zQuadTreeNode* quadNode, std::shared_ptr<Shader>& shader, FrameBuffer& frameBuffer);
	void ScanlineZbufferRender(ScanlineZbuffer* scanZbuffer,std::shared_ptr<Shader>& shader,FrameBuffer& frameBuffer);
};
