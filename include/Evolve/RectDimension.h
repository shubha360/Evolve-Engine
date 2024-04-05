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

#pragma once

namespace Evolve {	

	enum class Origin {
		BOTTOM_LEFT,
		BOTTOM_RIGHT,
		TOP_RIGHT,
		TOP_LEFT,
		CENTER
	};

	class RectDimension {
	public:
		RectDimension();
		RectDimension(Origin origin, int X, int y, unsigned int width, unsigned int height);
		~RectDimension();

		void set(Origin origin, int X, int y, unsigned int width, unsigned int height);

		inline Origin getOrigin() const { return origin_; }

		inline int getLeft() const { return left_; };
		inline int getRight() const { return left_ + width_; };

		inline int getBottom() const { return bottom_; };
		inline int getTop() const { return bottom_ + height_; };
		
		inline unsigned int getWidth() const { return width_; }
		inline unsigned int getHeight() const { return height_; }

		inline int getCenterX() const { return left_ + width_ / 2; };
		inline int getCenterY() const { return bottom_ + height_ / 2; };

	private:
		Origin origin_ = Origin::BOTTOM_LEFT;
		int left_ = 0, bottom_ = 0;
		unsigned int width_ = 0, height_ = 0;

		void findBottomLeft(Origin origin);
	};
}