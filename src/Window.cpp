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

bool Evolve::Window::init(const bool fullScreen, const unsigned int windowWidth, const unsigned int windowHeight, 
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

	if (attribResponse < 0) {
		EVOLVE_REPORT_ERROR("Failed to set an SDL_GL attribute. SDL_Error: " + std::string(SDL_GetError()), init);
		return false;
	}

	Uint32 windowFlags = SDL_WINDOW_OPENGL;

	if (fullScreen) {
		SDL_DisplayMode dm;
		SDL_GetCurrentDisplayMode(0, &dm);
		m_windowWidth = dm.w;
		m_windowHeight = dm.h;
		windowFlags |= SDL_WINDOW_FULLSCREEN;
	}
	else {
		m_windowWidth = windowWidth;
		m_windowHeight = windowHeight;
	}

	m_window = SDL_CreateWindow(
		"Tetris",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		m_windowWidth,
		m_windowHeight,
		windowFlags
	);

	if (m_window == nullptr) {
		EVOLVE_REPORT_ERROR("Failed to create window. SDL_Error: " + std::string(SDL_GetError()), init);
		return false;
	}

	SDL_GLContext glContext = SDL_GL_CreateContext(m_window);

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

	GLfloat red   = (GLfloat) clearColor.red   / 255.0f ;
	GLfloat green = (GLfloat) clearColor.green / 255.0f ;
	GLfloat blue  = (GLfloat) clearColor.blue  / 255.0f ;
	GLfloat alpha = (GLfloat) clearColor.alpha / 255.0f ;

	glClearColor(red, green, blue, alpha);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);

	// SDL_GL_SetSwapInterval(1);

	return true;
}

void Evolve::Window::deleteWindow() {
	if (m_window != nullptr) {
		SDL_DestroyWindow(m_window);
		m_windowWidth = 0;
		m_windowHeight = 0;
	}
}

void Evolve::Window::swapBuffer() const {
	SDL_GL_SwapWindow(m_window);
}
