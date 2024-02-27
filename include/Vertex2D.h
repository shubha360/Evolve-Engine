#pragma once

#include "Position2D.h"
#include "ColorRGBA.h"
#include "TextureCoords2D.h"

struct Vertex2D {
	Position2D position;
	ColorRGBA color;
	TextureCoords2D textureCoords;

	void setPosition(GLint x, GLint y) {
		position.set(x, y);
	}

	void setColor(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha) {
		color.set(red, green, blue, alpha);
	}

	void setColor(ColorRGBA& newColor) {
		color.set(newColor.red, newColor.green, newColor.blue, newColor.alpha);
	}

	void setTextureCoords(GLfloat u, GLfloat v) {
		textureCoords.set(u, v);
	}
};