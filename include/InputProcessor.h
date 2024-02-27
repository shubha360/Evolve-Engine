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