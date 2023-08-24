#pragma once

#include "geometry.h"
#include "tgaimage.h"

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
	TGAColor color;
	Vec2f uvCoord;
	Vec3f normal;
	Vec3f bar;
	TGAColor diffuse;
	float specular;
	//mat<4, 4, float> uniform_Mshadow; // transform framebuffer screen coordinates to shadowbuffer screen coordinates
};