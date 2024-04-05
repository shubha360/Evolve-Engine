#pragma once

#include "IncludeLibs.h"

namespace Evolve {
	struct Size2D {
		GLuint Width, Height;

		void set(const GLint x, const GLint y) {
			this->Width = x;
			this->Height = y;
		}

		void set(const glm::ivec2 pos) {
			this->Width = pos.x;
			this->Height = pos.y;
		}
	};
}