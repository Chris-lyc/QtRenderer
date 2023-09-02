#include "mesh.h"
#include "renderInstance.h"
void Mesh::draw() {
	RenderInstance::getInstance().vertices = vertices;
	RenderInstance::getInstance().indices = indices;
	RenderInstance::getInstance().render();
}