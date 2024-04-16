#version 330 core

layout(location = 0) in vec2 vertexPos;
layout(location = 1) in vec4 vertexColor;
layout(location = 2) in vec2 vertexUV;

layout(location = 0) out vec4 fragmentColor;
layout(location = 1) out vec2 fragmentUV;

uniform mat4 u_mvpMatrix;

void main() {
	gl_Position = u_mvpMatrix * vec4(vertexPos.xy, 0.0, 1.0);

	fragmentColor = vertexColor;
	fragmentUV = vec2(vertexUV.x, 1.0 - vertexUV.y);
}