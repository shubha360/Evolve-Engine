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

#include "../include/Evolve/GlslProgram.h"

Evolve::GlslProgram::GlslProgram() {}

Evolve::GlslProgram::~GlslProgram() {
	freeProgram();
}

bool Evolve::GlslProgram::compileAndLinkShaders(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) {

	programID_ = glCreateProgram();

	vertexShaderID_ = compileShader(vertexShaderPath, GL_VERTEX_SHADER);
	fragmentShaderID_ = compileShader(fragmentShaderPath, GL_FRAGMENT_SHADER);

	if (vertexShaderID_ == 0 || fragmentShaderID_ == 0) {
		EVOLVE_REPORT_ERROR("Unable to compile shaders.", compileAndLinkShaders);
		return false;
	}

	glAttachShader(programID_, vertexShaderID_);
	glAttachShader(programID_, fragmentShaderID_);

	glLinkProgram(programID_);

	GLint isLinked = 0;
	glGetProgramiv(programID_, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(programID_, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(programID_, maxLength, &maxLength, &infoLog[0]);

		glDeleteProgram(programID_);
		
		glDeleteShader(vertexShaderID_);
		glDeleteShader(fragmentShaderID_);

		std::string errStr = "Failed to link program. " + std::string(&infoLog[0]);

		EVOLVE_REPORT_ERROR(errStr.c_str(), compileAndLinkShaders);
		return false;
	}

	glDetachShader(programID_, vertexShaderID_);
	glDetachShader(programID_, fragmentShaderID_);

	return true;
}

GLint Evolve::GlslProgram::getUniformLocation(const std::string& uniformName) {
	
	auto it = m_uniformCache.find(uniformName);

	if (it != m_uniformCache.end()) {
		return it->second;
	}
	else {
		GLint location = glGetUniformLocation(programID_, uniformName.c_str());

		if (location == GL_INVALID_INDEX) {
			std::string errStr = "Uniform " + std::string(uniformName) + " was not found in the shader.";

			EVOLVE_REPORT_ERROR(errStr.c_str(), getUniformLocation);
		}
		else {
			m_uniformCache.insert(make_pair(uniformName, location));
		}

		return location;
	}
}

void Evolve::GlslProgram::useProgram() {
	glUseProgram(programID_);
}

void Evolve::GlslProgram::unuseProgram() {
	glUseProgram(0);
}

void Evolve::GlslProgram::freeProgram() {
	if (vertexShaderID_ != 0) {
		glDeleteShader(vertexShaderID_);
		vertexShaderID_ = 0;
	}
	if (fragmentShaderID_ != 0) {
		glDeleteShader(fragmentShaderID_);
		fragmentShaderID_ = 0;
	}
	if (programID_ != 0) {
		glDeleteProgram(programID_);
		programID_ = 0;
	}
}

GLuint Evolve::GlslProgram::compileShader(const std::string& shaderPath, const GLenum shaderType) {

	// open shader file
	std::ifstream shaderFile(shaderPath, std::ios::binary);

	if (shaderFile.fail()) {
		perror(shaderPath.c_str());
		shaderFile.close();
		return 0;
	}

	// read entire file
	std::string shaderContent(std::istreambuf_iterator<char> { shaderFile }, {});

	shaderFile.close();

	// create shader
	GLuint shaderID = glCreateShader(shaderType);

	char const* source = shaderContent.c_str();

	// send the data for shader
	glShaderSource(shaderID, 1, &source, nullptr);

	// compile shader
	glCompileShader(shaderID);

	GLint isCompiled = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(shaderID, maxLength, &maxLength, &errorLog[0]);

		// Exit with failure.
		glDeleteShader(shaderID);

		std::string shaderName = (shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment");

		std::string errStr = "Failed to compile " + shaderName + " shader.\n" + std::string(&errorLog[0]);

		EVOLVE_REPORT_ERROR(errStr.c_str(), compileShader);
		return 0;
	}
	return shaderID;
}
