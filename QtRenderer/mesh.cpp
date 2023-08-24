#include "mesh.h"
#include "scene.h"
void Mesh::draw() {
	Scene::getInstance().vertices = vertices;
	Scene::getInstance().indices = indices;
	Scene::getInstance().render();
}