#pragma once

#include "IncludeLibs.h"

struct TextureCoords2D {
	GLfloat u;
	GLfloat v;

	void set(GLfloat u, GLfloat v) {
		this->u = u;
		this->v = v;
	}
};