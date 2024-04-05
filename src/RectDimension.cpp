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

#include "../include/Evolve/RectDimension.h"

Evolve::RectDimension::RectDimension() {}

Evolve::RectDimension::RectDimension(Origin origin, int X, int y, unsigned int width, unsigned int height) {
	set(origin, X, y, width, height);
}

Evolve::RectDimension::~RectDimension() {}

void Evolve::RectDimension::set(Origin origin, int X, int y, unsigned int width, unsigned int height) {
	origin_ = origin;
	left_ = X;
	bottom_ = y;
	width_ = width;
	height_ = height;

	findBottomLeft(origin);
}

void Evolve::RectDimension::findBottomLeft(Origin origin) {
	switch (origin) {
	case Origin::BOTTOM_LEFT:
		left_ = left_;
		bottom_ = bottom_;
		break;

	case Origin::BOTTOM_RIGHT:
		left_ = left_ - width_;
		bottom_ = bottom_;
		break;

	case Origin::TOP_RIGHT:
		left_ = left_ - width_;
		bottom_ = bottom_ - height_;
		break;

	case Origin::TOP_LEFT:
		left_ = left_;
		bottom_ = bottom_ - height_;
		break;

	case Origin::CENTER:
		left_ = left_ - width_ / 2;
		bottom_ = bottom_ - height_ / 2;
		break;
	}
}


