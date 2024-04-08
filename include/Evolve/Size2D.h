#pragma once

#include "IncludeLibs.h"

namespace Evolve {
	struct Size2D {
		GLuint Width, Height;

		void set(const GLuint x, const GLuint y) {
			this->Width = x;
			this->Height = y;
		}

		void set(const glm::ivec2 pos) {
			this->Width = (GLuint) pos.x;
			this->Height = (GLuint) pos.y;
		}

		void set(const Size2D& newSize) {
			Width = newSize.Width;
			Height = newSize.Height;
		}

		bool isEqualTo(const Size2D& other) const {
			return Width == other.Width && Height == other.Height;
		}
	};
}