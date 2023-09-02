#include "depthShader.h"

bool DepthShader::vertex(Vertex& vertex) {
	Vec4f worldCoord_ = Model_ * embed<4>(vertex.worldCoord);
	vertex.worldCoord = proj<3>(worldCoord_ / worldCoord_.w);
	
	//Vec4f tmp = View_ * embed<4>(vertex.worldCoord);
	//tmp = Projection_ * tmp;
	//tmp = tmp / tmp.w;
	//std::cout << tmp.x << " " << tmp.y << " " << tmp.z << std::endl;
	
	Vec4f gl_Vertex = ViewPort_ * Projection_ * View_ * embed<4>(vertex.worldCoord);
	vertex.screenCoord = proj<3>(gl_Vertex / gl_Vertex.w);

	//std::cout << vertex.screenCoord.x <<" "<<vertex.screenCoord.y<<" "<<vertex.screenCoord.z<< std::endl;
	
	Vec4f normal_ = (View_ * Model_).invert_transpose() * embed<4>(vertex.normal, 0.f);
	vertex.normal = proj<3>(normal_);
	return true;
}

bool DepthShader::fragment(Fragment& fragment) {
	//Vec3f p = varying_tri * fragment.bar; // screen coord
	//std::cout << fragment.screenCoord.z << std::endl;
	fragment.color = Color(255.f, 255.f, 255.f) * (fragment.screenCoord.z / depth);
	//std::cout <<fragment.color.z << std::endl;
	//fragment.color = Color(255.f, 255.f, 255.f);
	return false;
}