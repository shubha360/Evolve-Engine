#version 330 core

layout(location = 0) in vec4 fragmentColor;

layout(location = 0) out vec4 finalColor;

void main() {
	finalColor = fragmentColor;
}