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

#include "../include/Evolve/Camera.h"

Evolve::Camera::Camera() {}

Evolve::Camera::~Camera() {}

bool Evolve::Camera::init(const Size2D& screenSize) {
	this->screenSize_ = screenSize;
	
	this->projectionMatrix_ = glm::ortho(0.0f, (float) screenSize_.Width, 0.0f, (float) screenSize_.Height, -1.0f, 1.0f);
	this->viewMatrix_ = glm::mat4(1.0f);
	this->modelMatrix_ = glm::mat4(1.0f);

	this->mvp_ = projectionMatrix_ * viewMatrix_ * modelMatrix_;

	return true;
}

void Evolve::Camera::sendMatrixDataToShader(GlslProgram& shaderProgram) {
	GLint mvpLoc = shaderProgram.getUniformLocation("u_mvpMatrix");
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &mvp_[0][0]);
}

Evolve::Position2D Evolve::Camera::convertScreenCoordsToWorldCoords(const Position2D& screenCoords) {
	return Evolve::Position2D { screenCoords.X, (GLint) screenSize_.Height - screenCoords.Y };
}

