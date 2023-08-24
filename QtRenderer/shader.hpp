#pragma once

#include "geometry.h"
#include "tgaimage.h"
#include "dataStructure.hpp"

class Shader {
public:
	Matrix Model_;
	Matrix View_;
	Matrix Projection_;
	Matrix ViewPort_;

	Vec3f light_dir;
	Vec3f eye;
	//float depth;

	Shader() {};
	virtual ~Shader() {};
	virtual bool vertex(Vertex& vertex) = 0;
	virtual bool fragment(Fragment& fragment) = 0;
	//virtual Vec4f vertex(int iface, int nthvert) = 0;
	//virtual bool fragment(Vec3f bar, TGAColor& color) = 0;
};