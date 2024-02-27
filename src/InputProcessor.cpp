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

#include "../include/InputProcessor.h"

InputProcessor::InputProcessor() {
	m_mouseCoords = glm::ivec2(0);
}

InputProcessor::~InputProcessor() {}

void InputProcessor::update() {
	for (auto& it : m_keyMap) {
		m_previousKeyMap[it.first] = it.second;
	}
}

void InputProcessor::pressKey(unsigned int keyID) {
	m_keyMap[keyID] = true;
}

void InputProcessor::releaseKey(unsigned int keyID) {
	m_keyMap[keyID] = false;
}

bool InputProcessor::isKeyDown(unsigned int keyID) {
	auto it = m_keyMap.find(keyID);

	if (it != m_keyMap.end()) {
		return it->second;
	}
	return false;
}

bool InputProcessor::wasKeyDown(unsigned int keyID) {
	auto it = m_previousKeyMap.find(keyID);

	if (it != m_previousKeyMap.end()) {
		return it->second;
	}
	return false;
}

bool InputProcessor::isKeyPressed(unsigned int keyID) {
	return !wasKeyDown(keyID) && isKeyDown(keyID);
}

bool InputProcessor::isKeyReleased(unsigned int keyID) {
	return wasKeyDown(keyID) && !isKeyDown(keyID);
}
