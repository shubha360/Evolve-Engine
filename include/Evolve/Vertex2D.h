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

#include "IncludeLibs.h"

#include "Position2D.h"
#include "ColorRgba.h"
#include "TextureCoords2D.h"

namespace Evolve {

	struct Vertex2D {
		Position2D Position {};
		ColorRgba Color {};
		TextureCoords2D TextureCoords {};

		void setPosition(const GLint x, const GLint y) {
			Position.set(x, y);
		}

		void setPosition(const Position2D& pos) {
			Position.set(pos.X, pos.Y);
		}

		void setPosition(const glm::ivec2& pos) {
			Position.set(pos);
		}

		void setColor(const GLubyte Red, const GLubyte Green, const GLubyte Blue, const GLubyte Alpha) {
			Color.set(Red, Green, Blue, Alpha);
		}

		void setColor(const ColorRgba& newColor) {
			Color.set(newColor.Red, newColor.Green, newColor.Blue, newColor.Alpha);
		}

		void setTextureCoords(const GLfloat u, const GLfloat v) {
			TextureCoords.set(u, v);
		}
	};
}