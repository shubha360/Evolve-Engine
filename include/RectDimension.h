#pragma once

struct RectDimension {
	int x, y;
	unsigned int width, height;

	void set(int x, int y, unsigned int width, unsigned int height) {
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
	}
};