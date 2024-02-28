/*
Copyright (c) 2024 Raquibul Islam

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include "Position2D.h"
#include "ColorRgba.h"
#include "TextureCoords2D.h"

struct Vertex2D {
	Position2D position;
	ColorRgba color;
	TextureCoords2D textureCoords;

	void setPosition(GLint x, GLint y) {
		position.set(x, y);
	}

	void setColor(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha) {
		color.set(red, green, blue, alpha);
	}

	void setColor(ColorRgba& newColor) {
		color.set(newColor.red, newColor.green, newColor.blue, newColor.alpha);
	}

	void setTextureCoords(GLfloat u, GLfloat v) {
		textureCoords.set(u, v);
	}
};