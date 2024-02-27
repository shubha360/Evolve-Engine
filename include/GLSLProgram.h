#pragma once

#include "IncludeLibs.h"

#include "ErrorReporter.h"

class GLSLProgram {
public:
	GLSLProgram();
	~GLSLProgram();

	bool compileAndLinkShaders(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	GLint getUniformLocation(const std::string& uniformName);
	void useProgram();
	void unuseProgram();

	void freeProgram();

private:
	GLuint m_programID = 0;
	GLuint m_vertexShaderID = 0;
	GLuint m_fragmentShaderID = 0;

	// Compiles a single shader, return the shader id
	// shaderType should be either GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
	GLuint compileShader(const std::string& shaderPath, const GLenum shaderType);
};