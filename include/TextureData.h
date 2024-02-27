#pragma once

#include "IncludeLibs.h"

struct TextureData {
	std::string path = "";
	GLuint id = 0;
	unsigned char* data = nullptr;
	int width = 0, height = 0;
	int bitsPerPixel = 0, bitsPerPixelInFile = 0;
};