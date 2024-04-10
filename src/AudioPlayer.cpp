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

#include "../include/Evolve/AudioPlayer.h"

Evolve::AudioPlayer::AudioPlayer() {}

Evolve::AudioPlayer::~AudioPlayer() {
	freeAudioPlayer();
}

bool Evolve::AudioPlayer::init() {

	int audioFlags = MIX_INIT_MP3 | MIX_INIT_OGG /*| MIX_INIT_WAVPACK*/;
	if (! (Mix_Init(audioFlags) & audioFlags)) {

		std::string errStr = "Mix_Init error: " + std::string(Mix_GetError());
		EVOLVE_REPORT_ERROR(errStr.c_str(), init);
		return false;
	}

	if (Mix_OpenAudio(48000, AUDIO_F32SYS, 2, 2048) == -1) {
		std::string errStr = "Mix_OpenAudio error: " + std::string(Mix_GetError());

		EVOLVE_REPORT_ERROR(errStr.c_str(), init);
		return false;
	}

	inited_ = true;

	return true;
}

size_t Evolve::AudioPlayer::addSoundEffect(const std::string& path) {

	auto effect = soundEffectCache_.find(path);

	if (effect != soundEffectCache_.end()) {
		return effect->second.second;
	}
	else {
		Mix_Chunk* chunk = Mix_LoadWAV(path.c_str());

		if (chunk == nullptr) {
			
			std::string errStr = "Mix_LoadWAV error: " + std::string(Mix_GetError());
			EVOLVE_REPORT_ERROR(errStr.c_str(), addSoundEffect);
			return -1;
		}
		
		soundEffects_.push_back(chunk);

		size_t id = soundEffects_.size() - 1;

		soundEffectCache_.insert(std::make_pair(path, std::make_pair(chunk, id)));

		return id;
	}
}

size_t Evolve::AudioPlayer::addMusic(const std::string& path) {
	auto effect = musicCache_.find(path);

	if (effect != musicCache_.end()) {
		return effect->second.second;
	}
	else {
		Mix_Music* music = Mix_LoadMUS(path.c_str());

		if (music == nullptr) {
			
			std::string errStr = "Mix_LoadMUS error: " + std::string(Mix_GetError());
			EVOLVE_REPORT_ERROR(errStr.c_str(), addMusic);
			return -1;
		}

		musics_.push_back(music);

		size_t id = musics_.size() - 1;

		musicCache_.insert(std::make_pair(path, std::make_pair(music, id)));

		return id;
	}
}

void Evolve::AudioPlayer::playSoundEffect(const size_t id, int loops) {

	if (id < 0 || id >= soundEffects_.size()) {
		EVOLVE_REPORT_ERROR("Invalid audio ID used", playSoundEffect);
		return;
	}

	if (Mix_PlayChannel(-1, soundEffects_[id], loops) == -1) {

		std::string errStr = "Mix_PlayChannel error: " + std::string(Mix_GetError());
		EVOLVE_REPORT_ERROR(errStr.c_str(), addMusic);
	}
}

void Evolve::AudioPlayer::playMusic(const size_t id, int loops) {

	if (id < 0 || id >= musics_.size()) {
		EVOLVE_REPORT_ERROR("Invalid audio ID used", playMusic);
		return;
	}

	if (Mix_PlayMusic(musics_[id], loops) == -1) {
		
		std::string errStr = "Mix_PlayMusic error: " + std::string(Mix_GetError());
		EVOLVE_REPORT_ERROR(errStr.c_str(), addMusic);
	}
}

void Evolve::AudioPlayer::pauseMusic(const size_t id) {
	Mix_PauseMusic();
}

void Evolve::AudioPlayer::resumeMusic(const size_t id) {
	Mix_ResumeMusic();
}

void Evolve::AudioPlayer::stopMusic(const size_t id) {
	Mix_HaltMusic();
}

void Evolve::AudioPlayer::freeAudioPlayer() {
	
	for (auto& effect : soundEffects_) {
		Mix_FreeChunk(effect);
	}

	for (auto& music : musics_) {
		Mix_FreeMusic(music);
	}
	
	Mix_CloseAudio();
	Mix_Quit();
}