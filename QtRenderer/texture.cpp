#include "texture.h"

TextureType Texture::getType() {
	return type;
}

void Texture::loadImage(std::string filename, const char* suffix, TextureType type) {
	std::string texfile(filename);
	size_t dot = texfile.find_last_of(".");
	if (dot != std::string::npos) {
		texfile = texfile.substr(0, dot) + std::string(suffix);
		int width, height, nrChannels;
		map = stbi_load(texfile.c_str(), &width, &height, &nrChannels, 0);
		if (map) {
			this->width =width;
			this->height = height;
			this->channels = nrChannels;
			this->type = type;
			std::cerr << "texture file " << texfile << " loading " << "ok" << std::endl;
		}
		else {
			std::cerr << "texture file " << texfile << " loading " << "fail" << std::endl;
		}
	}
}

Color Texture::sample2D(Vec2f uvCoord) {
	unsigned char* pixelOffset = map + (int(uvCoord.x * width) + (height - int(uvCoord.y * height)) * width) * channels;
	unsigned char r;
	unsigned char g;
	unsigned char b;
	if (channels == 4) {
		r = pixelOffset[0];
		g = pixelOffset[1];
		b = pixelOffset[2];
	}
	else if (channels == 3) {
		r = pixelOffset[0];
		g = pixelOffset[1];
		b = pixelOffset[2];
	}
	else if (channels == 1) {
		r = pixelOffset[0];
		g = pixelOffset[0];
		b = pixelOffset[0];
	}
	else {
		std::cerr << "texture file has wrong channels!" << std::endl;
	}
	
	if (type == DIFFUSE) {
		return { r * 1.f,g * 1.f,b * 1.f };
	}
	if (type == NORMAL) {
		return { r / 255.f * 2.f - 1.f,g / 255.f * 2.f - 1.f,b / 255.f * 2.f - 1.f };
	}
	if (type == SPECLUAR) {
		return { r * 1.f,g * 1.f,b * 1.f };
	}
	return {};
}