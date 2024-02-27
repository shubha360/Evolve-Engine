#pragma once

#include "IncludeLibs.h"

#include "ColorRGBA.h"
#include "ErrorReporter.h"

class Window {
public:
	Window();
	~Window();

	bool init(const bool fullScreen, const unsigned int windowWidth, const unsigned int windowHeight, const ColorRGBA& clearColor);

	void swapBuffer() const;

	void deleteWindow();

	const unsigned int getWindowWidth() const { return m_windowWidth; }
	const unsigned int getWindowHeight() const { return m_windowHeight; }

private:
	SDL_Window* m_window = nullptr;
	unsigned int m_windowWidth = 0, m_windowHeight = 0;
};

