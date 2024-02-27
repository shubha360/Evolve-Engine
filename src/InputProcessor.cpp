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
