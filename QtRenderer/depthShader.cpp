#include "depthShader.h"

bool DepthShader::vertex(Vertex& vertex) {
	Vec4f worldCoord_ = Model_ * embed<4>(vertex.worldCoord);
	vertex.worldCoord = proj<3>(worldCoord_ / worldCoord_.w);
	
	Vec4f gl_Vertex = ViewPort_ * Projection_ * View_ * embed<4>(vertex.worldCoord);
	vertex.screenCoord = proj<3>(gl_Vertex / gl_Vertex.w);

	
	Vec4f normal_ = (View_ * Model_).invert_transpose() * embed<4>(vertex.normal, 0.f);
	vertex.normal = proj<3>(normal_);
	return true;
}

bool DepthShader::fragment(Fragment& fragment) {
	fragment.color = Color(255.f, 255.f, 255.f) * (fragment.screenCoord.z / depth);
	return false;
}