#include "../include/Camera.h"

Camera::Camera() {}

Camera::~Camera() {}

bool Camera::init(const unsigned int screenWidth, const unsigned int screenHeight) {
	this->m_screenWidth = screenWidth;
	this->m_screenHeight = screenHeight;
	
	this->m_projectionMatrix = glm::ortho(0.0f, (float)screenWidth, 0.0f, (float)screenHeight, -1.0f, 1.0f);
	this->m_viewMatrix = glm::mat4(1.0f);
	this->m_modelMatrix = glm::mat4(1.0f);

	this->m_mvp = m_projectionMatrix * m_viewMatrix * m_modelMatrix;

	return true;
}

void Camera::sendMatrixDataToShader(GLSLProgram& shaderProgram) {
	GLint mvpLoc = shaderProgram.getUniformLocation("u_mvpMatrix");
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &m_mvp[0][0]);
}

glm::ivec2 Camera::convertScreenCoordsToWorldCoords(const glm::ivec2& screenCoords) {
	return glm::ivec2(screenCoords.x, m_screenHeight - screenCoords.y);
}

