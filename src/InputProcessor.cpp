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

#include "../include/Evolve/InputProcessor.h"

Evolve::InputProcessor::InputProcessor() {}

Evolve::InputProcessor::~InputProcessor() {}

void Evolve::InputProcessor::update() {
	for (auto& it : keyMap_) {
		previousKeyMap_[it.first] = it.second;
	}
}

void Evolve::InputProcessor::pressKey(unsigned int keyID) {
	keyMap_[keyID] = true;
}

void Evolve::InputProcessor::releaseKey(unsigned int keyID) {
	keyMap_[keyID] = false;
}

bool Evolve::InputProcessor::isKeyDown(unsigned int keyID) {
	auto it = keyMap_.find(keyID);

	if (it != keyMap_.end()) {
		return it->second;
	}
	return false;
}

bool Evolve::InputProcessor::wasKeyDown(unsigned int keyID) {
	auto it = previousKeyMap_.find(keyID);

	if (it != previousKeyMap_.end()) {
		return it->second;
	}
	return false;
}

bool Evolve::InputProcessor::isKeyPressed(unsigned int keyID) {
	return !wasKeyDown(keyID) && isKeyDown(keyID);
}

bool Evolve::InputProcessor::isKeyReleased(unsigned int keyID) {
	return wasKeyDown(keyID) && !isKeyDown(keyID);
}
