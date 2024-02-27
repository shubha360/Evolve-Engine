#include "../include/Window.h"

Window::Window() {}

Window::~Window() {
	deleteWindow();
}

bool Window::init(const bool fullScreen, const unsigned int windowWidth, const unsigned int windowHeight, const ColorRGBA& clearColor)
{	
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		REPORT_ERROR("Failed to initialize SDL. SDL_Error: " + std::string(SDL_GetError()), init);
		return false;
	}

	int attribResponse = 0;

	attribResponse += SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	attribResponse += SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	attribResponse += SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	attribResponse += SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	if (attribResponse < 0) {
		REPORT_ERROR("Failed to set an SDL_GL attribute. SDL_Error: " + std::string(SDL_GetError()), init);
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
		REPORT_ERROR("Failed to create window. SDL_Error: " + std::string(SDL_GetError()), init);
		return false;
	}

	SDL_GLContext glContext = SDL_GL_CreateContext(m_window);

	if (glContext == nullptr) {
		REPORT_ERROR("Failed to create GL context. SDL_Error: " + std::string(SDL_GetError()), init);
		return false;
	}

	GLenum response = glewInit();

	if (response != GLEW_OK) {
		REPORT_ERROR("Failed to initialize GLEW.", init);
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

void Window::deleteWindow() {
	if (m_window != nullptr) {
		SDL_DestroyWindow(m_window);
		m_windowWidth = 0;
		m_windowHeight = 0;
	}
}

void Window::swapBuffer() const {
	SDL_GL_SwapWindow(m_window);
}
