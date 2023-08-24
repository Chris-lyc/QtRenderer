#include "texture.h"

void Texture::loadImage(std::string filename, const char* suffix, Type type) {
	std::string texfile(filename);
	size_t dot = texfile.find_last_of(".");
	if (dot != std::string::npos) {
		texfile = texfile.substr(0, dot) + std::string(suffix);
		if (map.read_tga_file(texfile.c_str())) {
			this->path = texfile.c_str();
			this->width = map.get_width();
			this->height = map.get_height();
			this->type = type;
			std::cerr << "texture file " << texfile << " loading " << "ok" << std::endl;
			map.flip_vertically();
		}
		else {
			std::cerr << "texture file " << texfile << " loading " << "fail" << std::endl;
		}
	}
}

Vec3f Texture::sample2D(Vec2f uvCoord) {
	TGAColor value = map.get(uvCoord.x * map.get_width(), uvCoord.y * map.get_height());
	if (type == DIFFUSE) {  //bgr
		return { value[0] / 1.f,value[1] / 1.f,value[2] / 1.f };
	}
	if (type == NORMAL) {  //rgb
		Vec3f res;
		for (int i = 0; i < 3; ++i) {
			res[i] = value[2 - i] / 255.f * 2.f - 1.f;
		}
		return res;
	}
	if (type == SPECLUAR) {  //gray
		return { value[0] / 1.f,value[1] / 1.f,value[2] / 1.f };
	}
	return {};
}