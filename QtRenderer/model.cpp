#include "model.h"
#include "renderInstance.h"
#include <QString>
#include <string>
#include <fstream>

void Model::loadModel(std::string path) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));
	
	processNode(scene->mRootNode, scene);
	
	size_t dot = path.find_last_of(".");
	std::string diffusePath;
	std::string normalPath;
	std::string specularPath;
	if (dot != std::string::npos)
	{
		diffusePath = path.substr(0, dot) + std::string("_diffuse.tga");
		normalPath = path.substr(0, dot) + std::string("_nm_tangent.tga");
		specularPath = path.substr(0, dot) + std::string("_spec.tga");
	}
	
	std::ifstream diffuseFile(diffusePath.c_str());
	if(diffuseFile.good())
	{
		diffusemap_=new Texture();
		diffusemap_->loadImage(path, "_diffuse.tga", DIFFUSE);
	}
	std::ifstream normalFile(normalPath.c_str());
	if(normalFile.good())
	{
		normalmap_=new Texture();
		normalmap_->loadImage(path, "_nm_tangent.tga", NORMAL);
	}
	std::ifstream specularFile(specularPath.c_str());
	if(specularFile.good())
	{
		specularmap_=new Texture();
		specularmap_->loadImage(path, "_spec.tga", SPECLUAR);
	}
}

void Model::processNode(aiNode* node, const aiScene* scene){
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene){
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		Vec3f vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.worldCoord = vector;
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.normal = vector;
		if (mesh->mTextureCoords[0]) {
			Vec2f vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.uvCoord = vec;
		}
		else
			vertex.uvCoord = Vec2f(0.f, 0.f);
		vertices.push_back(vertex);
	}
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	return Mesh(vertices, indices);
}

// void Model::draw() {
// 	for (int i = 0; i < meshes.size(); ++i) {
// 		meshes[i].draw();
// 	}
// }

void Model::displayMeshNumber()
{
	for(int i=0;i<meshes.size();++i)
	{
		std::cout<<"Number of meshes: "<<meshes[i].indexs.size()<<std::endl;
	}
}
