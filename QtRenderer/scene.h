#pragma once

#include "tgaimage.h"
#include "geometry.h"
#include "model.h"
#include "framebuffer.h"
#include "dataStructure.hpp"
#include "texture.h"
#include "shader.hpp"
#include <vector>

class Scene {
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned> indices;
	Texture diffusemap_;
	Texture normalmap_;
	Texture specularmap_;
	std::unique_ptr<Shader> shader;

	Scene(int width, int height);
	void render();
	static void init(int width, int height);
	static Scene& getInstance(int width = 0, int height = 0);
	QImage& getBuffer() { return frameBuffer.GetImage(); };
	void clearBuffer() { frameBuffer.ClearBuffer(TGAColor(255.f, 255.f, 255.f)); }
	void processTriangle(std::vector<Vertex> &tri);
	void triangle(std::vector<Vertex>& triangle);
	void constructFragment(std::vector<Vertex>& triangle, Vec3f& bar, TGAColor& color, Fragment& fragment);
	Vec3f barycentric(Vec2f* pts, Vec2f P);
	void saveImage(QString path);

private:
	int width;
	int height;
	FrameBuffer frameBuffer;
	bool multiThread;
};