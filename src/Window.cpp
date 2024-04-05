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

#include "../include/Evolve/Window.h"

Evolve::Window::Window() {}

Evolve::Window::~Window() {
	deleteWindow();
}

bool Evolve::Window::init(const char* windowTitle, const bool fullScreen, 
	const unsigned int windowWidth, const unsigned int windowHeight,
	const ColorRgba& clearColor) {	
	
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		EVOLVE_REPORT_ERROR("Failed to initialize SDL. SDL_Error: " + std::string(SDL_GetError()), init);
		return false;
	}

	int attribResponse = 0;

	attribResponse += SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	attribResponse += SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	attribResponse += SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	attribResponse += SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// SDL_GL_SetAttribute returns 0 on success and negative error code on failure
	if (attribResponse < 0) {
		EVOLVE_REPORT_ERROR("Failed to set an SDL_GL attribute. SDL_Error: " + std::string(SDL_GetError()), init);
		return false;
	}

	Uint32 windowFlags = SDL_WINDOW_OPENGL;

	// get the screen size if in fullscreen
	if (fullScreen) {
		SDL_DisplayMode dm;
		SDL_GetCurrentDisplayMode(0, &dm);
		
		windowWidth_ = dm.w;
		m_windowHeight = dm.h;
		
		windowFlags |= SDL_WINDOW_FULLSCREEN;
	}
	else {
		windowWidth_ = windowWidth;
		m_windowHeight = windowHeight;
	}

	window_ = SDL_CreateWindow(
		windowTitle,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		windowWidth_,
		m_windowHeight,
		windowFlags
	);

	if (window_ == nullptr) {
		EVOLVE_REPORT_ERROR("Failed to create window. SDL_Error: " + std::string(SDL_GetError()), init);
		return false;
	}

	SDL_GLContext glContext = SDL_GL_CreateContext(window_);

	if (glContext == nullptr) {
		EVOLVE_REPORT_ERROR("Failed to create GL context. SDL_Error: " + std::string(SDL_GetError()), init);
		return false;
	}

	GLenum response = glewInit();

	if (response != GLEW_OK) {
		EVOLVE_REPORT_ERROR("Failed to initialize GLEW.", init);
		return false;
	}

	printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
	
	setClearColor(clearColor);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);

	// sdl function that limits the fps to 60, not needed if using the Evolve::Fps class
	// SDL_GL_SetSwapInterval(1);

	return true;
}

void Evolve::Window::clearScreen(GLbitfield mask) {
	glClear(mask);
}

void Evolve::Window::swapBuffer() const {
	SDL_GL_SwapWindow(window_);
}

void Evolve::Window::deleteWindow() {
	if (window_ != nullptr) {
		SDL_DestroyWindow(window_);
		windowWidth_ = 0;
		m_windowHeight = 0;
	}

	SDL_Quit();
}

void Evolve::Window::setClearColor(const ColorRgba& clearColor) {
	clearColor_ = clearColor;

	GLfloat Red		= (GLfloat) clearColor_.Red		/ 255.0f;
	GLfloat Green	= (GLfloat) clearColor_.Green	/ 255.0f;
	GLfloat Blue	= (GLfloat) clearColor_.Blue	/ 255.0f;
	GLfloat Alpha	= (GLfloat) clearColor_.Alpha	/ 255.0f;

	glClearColor(Red, Green, Blue, Alpha);
}
