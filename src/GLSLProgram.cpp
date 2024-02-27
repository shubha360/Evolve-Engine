#include "../include/GLSLProgram.h"

GLSLProgram::GLSLProgram() {}

GLSLProgram::~GLSLProgram() {
	freeProgram();
}

bool GLSLProgram::compileAndLinkShaders(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) {

	m_programID = glCreateProgram();

	m_vertexShaderID = compileShader(vertexShaderPath, GL_VERTEX_SHADER);
	m_fragmentShaderID = compileShader(fragmentShaderPath, GL_FRAGMENT_SHADER);

	if (m_vertexShaderID == 0 || m_fragmentShaderID == 0) {
		REPORT_ERROR("Unable to compile shaders.", compileAndLinkShaders);
		return false;
	}

	glAttachShader(m_programID, m_vertexShaderID);
	glAttachShader(m_programID, m_fragmentShaderID);

	glLinkProgram(m_programID);

	GLint isLinked = 0;
	glGetProgramiv(m_programID, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(m_programID, maxLength, &maxLength, &infoLog[0]);

		glDeleteProgram(m_programID);
		
		glDeleteShader(m_vertexShaderID);
		glDeleteShader(m_fragmentShaderID);

		REPORT_ERROR("Failed to link program. " + std::string(&infoLog[0]), compileAndLinkShaders);
		return false;
	}

	glDetachShader(m_programID, m_vertexShaderID);
	glDetachShader(m_programID, m_fragmentShaderID);

	return true;
}

GLint GLSLProgram::getUniformLocation(const std::string& uniformName) {
	GLint location = glGetUniformLocation(m_programID, uniformName.c_str());

	if (location == GL_INVALID_INDEX) {
		REPORT_ERROR("Uniform " + std::string(uniformName) + " was not found in the shader.", getUniformLocation);
	}

	return location;
}

void GLSLProgram::useProgram() {
	glUseProgram(m_programID);
}

void GLSLProgram::unuseProgram() {
	glUseProgram(0);
}

void GLSLProgram::freeProgram() {
	if (m_vertexShaderID != 0) {
		glDeleteShader(m_vertexShaderID);
		m_vertexShaderID = 0;
	}
	if (m_fragmentShaderID != 0) {
		glDeleteShader(m_fragmentShaderID);
		m_fragmentShaderID = 0;
	}
	if (m_programID != 0) {
		glDeleteProgram(m_programID);
		m_programID = 0;
	}
}

GLuint GLSLProgram::compileShader(const std::string& shaderPath, const GLenum shaderType) {

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

		REPORT_ERROR("Failed to compile " + shaderName + " shader.\n" + std::string(&errorLog[0]), compileShader);
		return 0;
	}
	return shaderID;
}
