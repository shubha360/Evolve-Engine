#version 330 core

in vec4 fragmentColor;
in vec2 fragmentUV;

out vec4 finalColor;

uniform sampler2D u_imageSampler;

void main() {
	finalColor = texture(u_imageSampler, fragmentUV) * fragmentColor;
}