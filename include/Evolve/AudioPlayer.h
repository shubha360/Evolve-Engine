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
#include "ErrorReporter.h"

namespace Evolve {
	
	/*class Music {
	public:
		friend class AudioPlayer;

		void play(const int loops = 0);

	private:
		Mix_Music* music_ = nullptr;
	};

	class SoundEffect {
	public:
		friend class AudioPlayer;

		void play(const int loops = 0);

		void pause();
		void resume();
		void stop();

	private:
		Mix_Chunk* chunk_ = nullptr;
	};*/

	class AudioPlayer {
	public:
		AudioPlayer();
		~AudioPlayer();

		bool init();

		size_t addSoundEffect(const std::string& path);
		size_t addMusic(const std::string& path);		

		void playSoundEffect(const size_t id, int loops);

		void playMusic(const size_t id, int loops);
		void pauseMusic(const size_t id);
		void resumeMusic(const size_t id);
		void stopMusic(const size_t id);

		void freeAudioPlayer();		

	private:
		bool inited_ = false;
		
		std::vector<Mix_Chunk*> soundEffects_;
		std::vector<Mix_Music*> musics_;

		std::map<std::string, std::pair<Mix_Chunk*, size_t>> soundEffectCache_;
		std::map<std::string, std::pair<Mix_Music*, size_t>> musicCache_;
	};
}
