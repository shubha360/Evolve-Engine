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

#include "GlslProgram.h"
#include "Font.h"
#include "ErrorReporter.h"
#include "TextureRenderer.h"
#include "ImageLoader.h"
#include "Camera.h"
#include "Gui.h"

namespace Evolve {

	class GuiRenderer {
	public:
		GuiRenderer();
		~GuiRenderer();

		bool init(const std::string& pathToAssets);

		void renderGui(Gui& gui, Camera& camera);

		void freeGuiRenderer();

	private:
		TextureRenderer textureRenderer_;

		TextureData buttonBgTexture_;
		TextureData panelTexture_;

		void getLabelCoordinates(int& X, int& y, const char* label,
			const int componentCenterX, const int componentCenterY, Font& font);
	};
}