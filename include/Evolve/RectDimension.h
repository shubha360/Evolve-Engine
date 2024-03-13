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
		RectDimension(Origin origin, int x, int y, unsigned int width, unsigned int height);
		~RectDimension();

		void set(Origin origin, int x, int y, unsigned int width, unsigned int height);

		inline Origin getOrigin() const { return m_origin; }

		inline int getLeft() const { return m_left; };
		inline int getRight() const { return m_left + m_width; };

		inline int getBottom() const { return m_bottom; };
		inline int getTop() const { return m_bottom + m_height; };
		
		inline unsigned int getWidth() const { return m_width; }
		inline unsigned int getHeight() const { return m_height; }

		inline int getCenterX() const { return m_left + m_width / 2; };
		inline int getCenterY() const { return m_bottom + m_height / 2; };

	private:
		Origin m_origin = Origin::BOTTOM_LEFT;
		int m_left = 0, m_bottom = 0;
		unsigned int m_width = 0, m_height = 0;

		void findBottomLeft(Origin origin);
	};
}