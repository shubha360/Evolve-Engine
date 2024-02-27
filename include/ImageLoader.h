#pragma once

#include "IncludeLibs.h"

#include "stb_image.h"
#include "TextureData.h"
#include "ErrorReporter.h"

class ImageLoader {
public:

	static void LoadTextureFromImage(const std::string& imagePath, TextureData& texture, 
		const unsigned int colorChannels);

	static void BufferTextureData(TextureData& texture);

	static void FreeTexture(TextureData& texture);
	static void DeleteTexture(TextureData& texture);

private:
	static std::map<std::string, TextureData> m_textureCache;
};
