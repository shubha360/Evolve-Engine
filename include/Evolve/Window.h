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

#include "IncludeLibs.h"

#include "ColorRgba.h"
#include "ErrorReporter.h"

namespace Evolve {

	class Window {
	public:
		Window();
		~Window();

		// the parameters windowWidth and windowHeight are unused if fulscreen is true
		bool init(const char* windowTitle, const bool fullScreen, 
			const unsigned int windowWidth, const unsigned int windowHeight, 
			const ColorRgba& clearColor);

		// wraps the glClear() function
		void clearScreen(GLbitfield mask);

		// wraps the SDL_GL_SwapWindow() function
		void swapBuffer() const;

		void deleteWindow();

		void setClearColor(const ColorRgba& clearColor);

		unsigned int getWindowWidth() const { return windowWidth_; }
		unsigned int getWindowHeight() const { return m_windowHeight; }

	private:
		SDL_Window* window_ = nullptr;
		unsigned int windowWidth_ = 0, m_windowHeight = 0;
		ColorRgba clearColor_ { 0, 0, 0, 0 };
	};
}
