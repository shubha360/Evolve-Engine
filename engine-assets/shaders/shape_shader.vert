#version 330 core

layout(location = 0) in vec2 vertexPos;
layout(location = 1) in vec4 vertexColor;

layout(location = 0) out vec4 fragmentColor;

uniform mat4 u_mvpMatrix;

void main() {
	gl_Position = u_mvpMatrix * vec4(vertexPos.xy, 0.0, 1.0);

	fragmentColor = vertexColor;
}