#pragma once

struct UVDimension {
	float bottomLeftX, bottomLeftY;
	float width, height;

	void set(float bottomLeftX, float bottomLeftY, float width, float height) {
		this->bottomLeftX = bottomLeftX;
		this->bottomLeftY = bottomLeftY;
		this->width = width;
		this->height = height;
	}
};