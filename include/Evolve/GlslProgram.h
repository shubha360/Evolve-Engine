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

#include "ErrorReporter.h"

namespace Evolve {

	class GlslProgram {
	public:
		GlslProgram();
		~GlslProgram();

		bool compileAndLinkShaders(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
		GLint getUniformLocation(const std::string& uniformName);
		void useProgram();
		void unuseProgram();

		void freeProgram();

	private:
		GLuint m_programID = 0;
		GLuint m_vertexShaderID = 0;
		GLuint m_fragmentShaderID = 0;

		std::unordered_map<std::string, GLint> m_uniformCache;

		// Compiles a single shader, return the shader id
		// shaderType should be either GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
		GLuint compileShader(const std::string& shaderPath, const GLenum shaderType);
	};
}