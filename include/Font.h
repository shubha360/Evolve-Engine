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

		unsigned int getLineHeight() const { return (unsigned int)(m_lineHeight * m_fontScale); }

		std::string getFontName() const { return m_fontName; }

		bool isInitialized() const { return m_initialized; }

		void setLetterSpacing(const int letterSpacing) { m_letterSpacing = letterSpacing; }
		void setLineSpacing(const int lineSpacing) { m_lineSpacing = lineSpacing; }
		void setAddToSpaceLength(const int addToSpaceLength) { m_addToSpaceLength = addToSpaceLength; }
		void setFontScale(const float fontScale) { m_fontScale = fontScale; }

		void deleteFont();

	private:
		std::string m_fontName;
		bool m_initialized = false;

		unsigned int m_spaceSize = 0;
		unsigned int m_newLine = 0;
		unsigned int m_lineHeight = 0;

		int m_letterSpacing = 0;
		int m_lineSpacing = 0;
		int m_addToSpaceLength = 0;

		float m_fontScale = 1.0f;

		TextureData m_fontTexture;

		std::vector<UvDimension> m_uvDimensions;
		std::vector<int> m_characterWidths;
	};
}