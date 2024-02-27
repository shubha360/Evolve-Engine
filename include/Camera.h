#pragma once

#include "IncludeLibs.h"

#include "GLSLProgram.h"

class Camera {
public:
	Camera();
	~Camera();

	bool init(const unsigned int screenWidth, const unsigned int screenHeight);

	void sendMatrixDataToShader(GLSLProgram& shaderProgram);

	glm::ivec2 convertScreenCoordsToWorldCoords(const glm::ivec2& screenCoords);

private:
	glm::mat4 m_mvp = glm::mat4(1.0f);

	glm::mat4 m_projectionMatrix = glm::mat4(1.0f);
	glm::mat4 m_viewMatrix = glm::mat4(1.0f);
	glm::mat4 m_modelMatrix = glm::mat4(1.0f);

	unsigned int m_screenWidth = 0, m_screenHeight = 0;
};

