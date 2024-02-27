#pragma once

#include "IncludeLibs.h"

class InputProcessor {
public:
	InputProcessor();
	~InputProcessor();

	void update();

	void pressKey(unsigned int keyID);
	void releaseKey(unsigned int keyID);

	bool isKeyDown(unsigned int keyID);
	bool isKeyPressed(unsigned int keyID);
	bool isKeyReleased(unsigned int keyID);

	void setMouseCoords(int x, int y) { m_mouseCoords.x = x; m_mouseCoords.y = y; }

	glm::ivec2 getMouseCoords() const { return m_mouseCoords; }

private:
	std::unordered_map<unsigned int, bool> m_keyMap;
	std::unordered_map<unsigned int, bool> m_previousKeyMap;
	glm::ivec2 m_mouseCoords;

	bool wasKeyDown(unsigned int keyID);
};