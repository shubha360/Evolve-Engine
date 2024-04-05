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

#include "TextureData.h"
#include "ImageLoader.h"
#include "UvDimension.h"
#include "ColorRgba.h"
#include "TextureRenderer.h"
#include "ErrorReporter.h"

namespace Evolve {

	class Font {
	public:
		Font();
		~Font();

		// only font bitmap that has 16 rows and 16 colummn
		bool initFromBitmap16x16(const std::string& fontName, const std::string& bmpFilePath,
			const float fontScale = 1.0f, const int letterSpacing = 0,
			const int lineSpacing = 0, const int addToSpaceLength = 0);

		bool initFromFontFile(const std::string& fontName, const std::string& fontFilePath, const unsigned int fontSize = 32,
			const float fontScale = 1.0f, const int letterSpacing = 0,
			const int lineSpacing = 0, const int addToSpaceLength = 0);

		void drawTextToRenderer(const std::string& text, const int topLeftX, const int topLeftY,
			const ColorRgba& color, TextureRenderer& textureRenderer);

		unsigned int getLineWidth(const std::string& text);

		unsigned int getLineHeight() const;

		unsigned int getTextHeight(const std::string& text) const;

		std::string getFontName() const { return fontName_; }

		bool isInitialized() const { return initialized_; }

		void setLetterSpacing(const int letterSpacing) { letterSpacing_ = letterSpacing; }
		void setLineSpacing(const int lineSpacing) { lineSpacing_ = lineSpacing; }
		void setAddToSpaceLength(const int addToSpaceLength) { addToSpaceLength_ = addToSpaceLength; }
		void setFontScale(const float fontScale) { fontScale_ = fontScale; }

		void deleteFont();

	private:
		std::string fontName_;
		bool initialized_ = false;

		unsigned int spaceSize_ = 0;
		unsigned int newLine_ = 0;
		unsigned int lineHeight_ = 0;

		int letterSpacing_ = 0;
		int lineSpacing_ = 0;
		int addToSpaceLength_ = 0;

		float fontScale_ = 1.0f;

		TextureData fontTexture_;

		std::vector<UvDimension> uvDimensions_;
		std::vector<int> characterWidths_;
	};
}