#pragma once

#include "geometry.h"
#include "model.h"
#include "framebuffer.h"
#include "dataStructure.hpp"
#include "texture.h"
#include "shader.hpp"
#include <vector>

class RenderInstance {
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned> indices;
	Texture* diffusemap_;
	Texture* normalmap_;
	Texture* specularmap_;
	std::shared_ptr<Shader> colorShader;
	std::shared_ptr<Shader> depthShader;

	RenderInstance(int width, int height, float depth);
	void render();
	static void init(int width, int height, float depth);
	static RenderInstance& getInstance(int width = 0, int height = 0, float depth = 0.f);
	
	void setUseShadow(bool use);

	QImage& getBuffer() { return colorFrameBuffer.GetImage(); };
	void clearBuffer() { 
		shadowFrameBuffer.ClearBuffer(Color(255.f, 255.f, 255.f));
		colorFrameBuffer.ClearBuffer(Color(255.f, 255.f, 255.f)); 
	}

	void processTriangle(std::vector<Vertex> tri, std::shared_ptr<Shader>& shader, FrameBuffer& frameBuffer);
	void triangle(std::vector<Vertex>& triangle, std::shared_ptr<Shader>& shader, FrameBuffer& frameBuffer);
	void constructFragment(std::vector<Vertex>& triangle, Vec3f& bar, Color& color, Fragment& fragment);
	Vec3f barycentric(Vec2f* pts, Vec2f P);
	void saveImage(QString& path);

private:
	int width;
	int height;
	float depth;
	FrameBuffer shadowFrameBuffer;
	FrameBuffer colorFrameBuffer;
	bool multiThread;
	bool useShadow;
};