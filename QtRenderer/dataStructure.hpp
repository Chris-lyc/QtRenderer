#pragma once

#include "geometry.h"
//#include "tgaimage.h"

using Color = Vec3f;

enum TextureType {
	DEFAULT,
	DIFFUSE,
	NORMAL,
	SPECLUAR
};

struct Buffer {
	//std::vector<float> data;
	float* data;
	int width;
	int height;
	Buffer() {};
	Buffer(int w, int h) :width(w), height(h) { data = (float*)malloc(sizeof(float) * width * height); }
	~Buffer() { free(data); }
};

struct Vertex {
	Vec3f worldCoord;
	Vec3f normal;
	Vec2f uvCoord;
	Vec3f screenCoord;
};

struct Fragment {
	mat<2, 3, float> varying_uv;
	mat<3, 3, float> varying_nrm; // normal per vertex to be interpolated by FS
	mat<3, 3, float> varying_tri;
	Color color;
	Vec3f screenCoord;
	Vec2f uvCoord;

	Vec3f bar;
	//Vec3f normal;
	//Color diffuse;
	//float specular;
	//mat<4, 4, float> uniform_Mshadow; // transform framebuffer screen coordinates to shadowbuffer screen coordinates
};