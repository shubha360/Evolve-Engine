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

#include "../include/Evolve/Font.h"

Evolve::Font::Font() { }

Evolve::Font::~Font() {
	deleteFont();
}

bool Evolve::Font::initFromBitmap16x16(const char* fontName, const char* bmpFilePath,
	const float fontScale /*= 1.0f*/, const int letterSpacing /*= 5*/, 
	const int lineSpacing /*= 5*/, const int addToSpaceLength /*= 0*/) {

	ImageLoader::LoadTextureFromImage(bmpFilePath, fontTexture_, 1);

	if (fontTexture_.data == nullptr) {
		EVOLVE_REPORT_ERROR("Failed to load image at " + std::string(bmpFilePath) + " for bitmap font.", initFromBitmap16x16);
		return false;
	}

	const int CELL_WIDTH = fontTexture_.width / 16;
	const int CELL_HEIGHT = fontTexture_.height / 16;

	int top = CELL_HEIGHT;
	int bottom = 0;
	int aBottom = 0;

	int currentCellX = 0, currentCellY = 0;
	int currentPixelX = 0, currentPixelY = 0;

	UvDimension currentUV = {};

	unsigned int currentChar = 0;

	for (int row = 0; row < 16; row++) {
		for (int column = 0; column < 16; column++) {

			currentCellX = CELL_WIDTH * column;
			currentCellY = CELL_HEIGHT * row;

			currentUV.set(
				(float) CELL_WIDTH * column / (float) fontTexture_.width,

				// if texture Height is 10 and cell Height is 2
				// for row = 0, (10 - ((0 + 1) * 2)) = 10 - 2 = 8
				// for row = 1, (10 - ((1 + 1) * 2)) = 10 - 4 = 6
				// for row = 4, (10 - ((4 + 1) * 2)) = 10 - 10 = 10
				(float)(fontTexture_.height - ((row + 1) * CELL_HEIGHT)) / (float)fontTexture_.height,
				
				
				(float) CELL_WIDTH / (float) fontTexture_.width,
				(float) CELL_HEIGHT / (float) fontTexture_.height
			);

			// Calculate the top
			for (int i = 0; i < CELL_HEIGHT; i++) {
				for (int j = 0; j < CELL_WIDTH; j++) {
					
					currentPixelX = currentCellX + j;
					currentPixelY = currentCellY + i;
					
					unsigned char currentPixel = fontTexture_.data[currentPixelY * fontTexture_.width + currentPixelX];

					if (currentPixel != 0) {
						if (i < top) {
							top = i;
						}

						j = CELL_WIDTH;
						i = CELL_WIDTH;
					}
				}
			}

			// Calculate the bottom
			for (int i = CELL_HEIGHT - 1; i >= 0; i--) {
				for (int j = 0; j < CELL_WIDTH; j++) {

					currentPixelX = currentCellX + j;
					currentPixelY = currentCellY + i;

					unsigned char currentPixel = fontTexture_.data[currentPixelY * fontTexture_.width + currentPixelX];

					if (currentPixel != 0) {
						if (currentChar == 'A') {
							aBottom = i;
						}

						if (i > bottom) {
							bottom = i;
						}

						j = CELL_WIDTH;
						i = -1;
					}
				}
			}

			// Calculate the left
			for (int j = 0; j < CELL_WIDTH; j++) {
				for (int i = 0; i < CELL_HEIGHT; i++) {
					currentPixelX = currentCellX + j;
					currentPixelY = currentCellY + i;

					unsigned char currentPixel = fontTexture_.data[currentPixelY * fontTexture_.width + currentPixelX];

					if (currentPixel != 0) {
						currentUV.BottomLeftX = (float) currentPixelX / (float) fontTexture_.width;

						// the left of current character
						characterWidths_[currentChar] = j;

						i = CELL_HEIGHT;
						j = CELL_WIDTH;
					}
				}
			}


			// Calculate the right
			for (int j = CELL_WIDTH - 1; j >= 0; j--) {
				for (int i = 0; i < CELL_HEIGHT; i++) {
					currentPixelX = currentCellX + j;
					currentPixelY = currentCellY + i;

					unsigned char currentPixel = fontTexture_.data[currentPixelY * fontTexture_.width + currentPixelX];

					if (currentPixel != 0) {
						currentUV.Width = 
							( ( (float) currentPixelX + 1) / (float) fontTexture_.width ) - currentUV.BottomLeftX;

						// now setting the actual Width
						characterWidths_[currentChar] = j - characterWidths_[currentChar];

						i = CELL_HEIGHT;
						j = -1;
					}
				}
			}

			uvDimensions_[currentChar] = currentUV;
			currentChar++;
		}
	}

	ImageLoader::BufferTextureData(fontTexture_);
	ImageLoader::FreeTexture(fontTexture_);

	spaceSize_ = CELL_WIDTH / 2;
	newLine_ = aBottom - top;
	lineHeight_ = bottom - top;

	for (int i = 0; i < 256; i++) {
		uvDimensions_[i].Height = (float)lineHeight_ / (float)fontTexture_.height;
	}

	fontName_ = fontName;
	initialized_ = true;

	fontScale_ = fontScale;
	letterSpacing_ = letterSpacing;
	lineSpacing_ = lineSpacing;
	addToSpaceLength_ = addToSpaceLength;

	return true;
}

bool Evolve::Font::initFromFontFile(const char* fontName, const char* fontFilePath,
	const unsigned int fontSize /*= 32*/,
	const float fontScale /*= 1.0f*/, const int letterSpacing /*= 5*/, 
	const int lineSpacing /*= 5*/, const int addToSpaceLength /*= 0*/) {

	const unsigned int TOTAL_FONTS = 256;

	static FT_Library library;
	FT_Error error = FT_Init_FreeType(&library);
	if (error) {
		EVOLVE_REPORT_ERROR("Failed to initialize FreeType.", initFromFontFile);
		return false;
	}

	int maxCellWidth = 0;
	int maxCellHeight = 0;
	int maxBearing = 0;
	int maxHang = 0;

	std::vector<TextureData> characterTextures;
	characterTextures.resize(TOTAL_FONTS);

	std::vector<FT_Glyph_Metrics> characterMetrics;
	characterMetrics.resize(TOTAL_FONTS);

	FT_Face face = 0;
	error = FT_New_Face(library, fontFilePath, 0, &face);
	if (error) {
		EVOLVE_REPORT_ERROR("Failed to create font face.", initFromFontFile);

		FT_Done_FreeType(library);
		return false;
	}

	error = FT_Set_Pixel_Sizes(face, 0, fontSize);
	if (error) {
		EVOLVE_REPORT_ERROR("Failed to set font size.", initFromFontFile);

		FT_Done_Face(face);
		FT_Done_FreeType(library);
		return false;
	}

	for (int i = 0; i < TOTAL_FONTS; i++) {

		error = FT_Load_Char(face, i, FT_LOAD_RENDER);
		if (error) {
			EVOLVE_REPORT_ERROR("Failed to load character " + std::to_string((char)i) + ".", initFromFontFile);

			FT_Done_Face(face);
			FT_Done_FreeType(library);
			return false;
		}

		characterMetrics[i] = face->glyph->metrics;

		characterTextures[i].width = face->glyph->bitmap.width;
		characterTextures[i].height = face->glyph->bitmap.rows;

		characterTextures[i].data = new unsigned char[characterTextures[i].width * characterTextures[i].height];

		memcpy(characterTextures[i].data, face->glyph->bitmap.buffer,
			(size_t) characterTextures[i].width * characterTextures[i].height);

		if (characterMetrics[i].horiBearingY / 64 > maxBearing) {
			maxBearing = characterMetrics[i].horiBearingY / 64;
		}

		if (characterMetrics[i].width / 64 > maxCellWidth) {
			maxCellWidth = characterMetrics[i].width / 64;

		}

		int glyphHang = (characterMetrics[i].height - characterMetrics[i].horiBearingY) / 64;
		if (glyphHang > maxHang)
		{
			maxHang = glyphHang;
		}
	}

	maxCellHeight = maxBearing + maxHang;

	unsigned int textureWidth = maxCellWidth * 16;
	unsigned int textureHeight = maxCellHeight * 16;

	fontTexture_.data = new unsigned char[textureWidth * textureHeight];
	memset(fontTexture_.data, 0, (size_t) textureWidth * textureHeight);

	fontTexture_.width = textureWidth;
	fontTexture_.height = textureHeight;
	fontTexture_.bitsPerPixel = 1;

	UvDimension currentUV = {};
	unsigned int currentChar = 0;

	unsigned int currentCellX = 0;
	unsigned int currentCellY = 0;

	for (int row = 0; row < 16; row++) {
		for (int column = 0; column < 16; column++) {

			currentCellX = maxCellWidth * column;
			currentCellY = maxCellHeight * row;

			currentUV.set(
				(float)maxCellWidth * column / (float)fontTexture_.width,

				// if texture Height is 10 and cell Height is 2
				// for row = 0, (10 - ((0 + 1) * 2)) = 10 - 2 = 8
				// for row = 1, (10 - ((1 + 1) * 2)) = 10 - 4 = 6
				// for row = 4, (10 - ((4 + 1) * 2)) = 10 - 10 = 10
				(float)(fontTexture_.height - ((row + 1) * maxCellHeight)) / (float)fontTexture_.height,


				(float)(characterMetrics[currentChar].width / 64) / (float)fontTexture_.width,
				(float)maxCellHeight / (float)fontTexture_.height
			);

			uvDimensions_[currentChar] = currentUV;
			characterWidths_[currentChar] = characterMetrics[currentChar].width / 64;

			int currentCharTop = currentCellY + maxBearing - characterMetrics[currentChar].horiBearingY / 64;

			unsigned char* destPixels = fontTexture_.data;
			unsigned char* srcPixels = characterTextures[currentChar].data;

			for (int i = 0; i < characterTextures[currentChar].height; i++) {

				memcpy(
					&destPixels[(i + currentCharTop) * fontTexture_.width + currentCellX],
					&srcPixels[i * characterTextures[currentChar].width],
					characterTextures[currentChar].width
				);
			}

			currentChar++;
		}
	}

	ImageLoader::BufferTextureData(fontTexture_);

	delete[] fontTexture_.data;
	fontTexture_.data = nullptr;

	for (auto& it : characterTextures) {
		delete[] it.data;
		it.data = nullptr;
	}

	FT_Done_Face(face);
	FT_Done_FreeType(library);

	spaceSize_ = maxCellWidth / 2;
	newLine_ = maxBearing;
	lineHeight_ = maxCellHeight;

	for (int i = 0; i < 256; i++) {
		uvDimensions_[i].Height = (float)lineHeight_ / (float)fontTexture_.height;
	}

	fontName_ = fontName;
	initialized_ = true;

	letterSpacing_ = letterSpacing;
	lineSpacing_ = lineSpacing;
	addToSpaceLength_ = addToSpaceLength;
	fontScale_ = fontScale;

	return true;
}

void Evolve::Font::drawTextToRenderer(const char* text, const int topLeftX, const int topLeftY,
	const ColorRgba& color, TextureRenderer& textureRenderer) const {

	if (fontTexture_.id == 0) {
		EVOLVE_REPORT_ERROR("Didn't load any font yet.", drawTextToRenderer);
		return;
	}

	/*textureRenderer.draw(
		GlyphOrigin::BOTTOM_LEFT,
		RectDimension { 0,0,700,700 },
		UvDimension { 0.0f, 0.0f, 1.0f, 1.0f },
		fontTexture_.id, ColorRgba { 255,255,255,255 }
	);*/

	int drawX = topLeftX;
	int drawY = topLeftY;
	
	RectDimension currentDims;

	int i = 0;

	while (text[i] != '\0') {
		if (text[i] == ' ') {
			drawX += (int) ((spaceSize_ + addToSpaceLength_) * fontScale_);
		}
		else if (text[i] == '\n') {
			drawX = topLeftX;
			drawY -= (int) ((newLine_ + lineSpacing_) * fontScale_);
		}
		else {
			unsigned int ASCII = (unsigned char) text[i];

			currentDims.set(
				Origin::TOP_LEFT,
				drawX,
				drawY,
				(unsigned int) (characterWidths_[ASCII] * fontScale_),
				(unsigned int) (lineHeight_ * fontScale_)
			);

			textureRenderer.draw(
				currentDims,
				uvDimensions_[ASCII],
				fontTexture_.id,
				color
			);

			drawX += (int) ((characterWidths_[ASCII] + letterSpacing_) * fontScale_);
		}
		i++;
	}
}

unsigned int Evolve::Font::getLineWidth(const char* text) const {

	int width = 0;
	int i = 0;

	while (text[i] != '\0') {
		if (text[i] == '\n') {
			break;
		}
		else if (text[i] == ' ') {
			width += (int) ((spaceSize_ + addToSpaceLength_) * fontScale_);
		}
		else {
			width += (int) ((characterWidths_[(unsigned char)text[i]] + letterSpacing_) * fontScale_);
		}
	}
	return width;
}

unsigned int Evolve::Font::getLineHeight() const {
	return (unsigned int)(lineHeight_ * fontScale_); 
}

unsigned int Evolve::Font::getTextHeight(const char* text) const {

	int lines = 1;
	int i = 0;

	while (text[i] != '\0') {
		if (text[i] == '\n') {
			lines++;
		}
	}
	return (unsigned int) (lineHeight_ * fontScale_ * lines);
}

void Evolve::Font::deleteFont() {
	ImageLoader::DeleteTexture(fontTexture_);
}