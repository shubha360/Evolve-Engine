#version 330 core

layout(location = 0) in vec4 fragmentColor;
layout(location = 1) in vec2 fragmentUV;

layout(location = 0) out vec4 finalColor;

uniform sampler2D u_imageSampler;

void main() {
	finalColor = texture(u_imageSampler, fragmentUV) * fragmentColor;
}