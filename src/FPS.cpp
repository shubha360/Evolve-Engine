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

#include "../include/Evolve/Fps.h"

Evolve::Fps::Fps() { }

Evolve::Fps::~Fps() { }

bool Evolve::Fps::init(float fps) {
	setFps(fps);
	return true;
}

void Evolve::Fps::beginFrame() {
	this->frameStartTicks = (float)SDL_GetTicks();
}

const bool Evolve::Fps::endFrame() const {
	float frameTick = (float)SDL_GetTicks() - frameStartTicks;

	float desiredFrameTime = 1000.0f / desiredFps;

	// return false if frame time is more than desired time
	if (frameTick > desiredFrameTime) {
		printf("Frame time is more than desired max frame time.\n");
		return false;
	}

	else if (frameTick < desiredFrameTime) {
		SDL_Delay((Uint32) (desiredFrameTime - frameTick));
	}

	return true;
}

float Evolve::Fps::calculateFps() {

	static float prevTick = (float)SDL_GetTicks();

	float currentTick = (float)SDL_GetTicks();

	frameTimes[currentFrame % NUM_SAMPLES] = currentTick - prevTick;

	prevTick = currentTick;

	int valueCount = NUM_SAMPLES;

	if (currentFrame < NUM_SAMPLES) {
		valueCount = currentFrame;
	}

	float frameTimeAverage = 0.0f;

	for (int i = 0; i < NUM_SAMPLES; i++) {
		frameTimeAverage += frameTimes[i];
	}

	frameTimeAverage /= (float)valueCount;

	if (frameTimeAverage > 0.0f) {
		currentFps = 1000.0f / frameTimeAverage;
	}

	currentFrame++;

	return currentFps;
}
