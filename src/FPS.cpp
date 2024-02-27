#include "../include/FPS.h"

FPS::FPS() { }

FPS::~FPS() { }

bool FPS::init(float fps) {
	setFps(fps);
	return true;
}

void FPS::beginFrame() {
	this->frameStartTicks = (float)SDL_GetTicks();
}

const bool FPS::endFrame() const {
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

float FPS::calculateFps() {

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
