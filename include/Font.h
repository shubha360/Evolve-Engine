#pragma once

#include "IncludeLibs.h"

#include "TextureData.h"
#include "ImageLoader.h"
#include "UVDimension.h"
#include "ColorRGBA.h"
#include "TextureRenderer.h"
#include "ErrorReporter.h"

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
		const ColorRGBA& color, TextureRenderer& textureRenderer);

	unsigned int getLineWidth(const std::string& text);

	unsigned int getLineHeight() const { return (unsigned int) (m_lineHeight * m_fontScale); }

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

	std::vector<UVDimension> m_uvDimensions;
	std::vector<int> m_characterWidths;
};