#pragma once

#include "IncludeLibs.h"

struct ColorRGBA {
	GLubyte red;
	GLubyte green;
	GLubyte blue;
	GLubyte alpha;

	void set(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha) {
		this->red = red;
		this->green = green;
		this->blue = blue;
		this->alpha = alpha;
	}
};