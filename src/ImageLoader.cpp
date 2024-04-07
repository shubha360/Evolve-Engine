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

#include "../include/Evolve/ImageLoader.h"

std::map<std::string, Evolve::TextureData> Evolve::ImageLoader::textureCache_;

void Evolve::ImageLoader::LoadTextureFromImage(const std::string& imagePath, TextureData& texture,
    const unsigned int colorChannels) {

    if (colorChannels != 1 && colorChannels != 4) {
        std::string errStr = "Invalid Color channel " + std::to_string(colorChannels) + ".";

        EVOLVE_REPORT_ERROR(errStr.c_str(), LoadTextureFromImage);
        return;
    }

    auto it = textureCache_.find(imagePath);

    if (it != textureCache_.end()) { // Found in the cache
        texture = it->second;
        //printf("Loaded from cache.\n\n");
    }
    else { // Not found in the cache

        FreeTexture(texture);
        DeleteTexture(texture);

        texture.path = imagePath;
        texture.bitsPerPixel = colorChannels;

        texture.data =
            stbi_load(
                imagePath.c_str(),
                &texture.width,
                &texture.height,
                &texture.bitsPerPixelInFile,
                texture.bitsPerPixel
            );

        if (texture.data == nullptr) {
            std::string errStr = "Failed to load image at " + imagePath;

            EVOLVE_REPORT_ERROR(errStr.c_str(), LoadTextureFromImage);

            texture.path = "";
            texture.bitsPerPixel = 0;
        }
        else {
            textureCache_.insert(make_pair(imagePath, texture));
            //printf("Loaded from file.\n\n");
        }
    }    
}

void Evolve::ImageLoader::BufferTextureData(TextureData& texture) {

    if (texture.id != 0) {
        EVOLVE_REPORT_ERROR("Texture data is already buffered.", BufferTextureData);
        return;
    }

    if (texture.data == nullptr) {
        EVOLVE_REPORT_ERROR("Texture has no pixel data.", BufferTextureData);
        return;
    }

    glGenTextures(1, &texture.id);

    glBindTexture(GL_TEXTURE_2D, texture.id);

    GLint internalFormat = 0;
    GLenum pixelFormat = 0;

    switch (texture.bitsPerPixel) {
    case 1:
        internalFormat = GL_RED;
        pixelFormat = GL_RED;
        break;

    case 4:
        internalFormat = GL_RGBA8;
        pixelFormat = GL_RGBA;
        break;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, texture.width, texture.height,
        0, pixelFormat, GL_UNSIGNED_BYTE, &texture.data[0]);


    if (internalFormat == GL_RED && pixelFormat == GL_RED) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_RED);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Evolve::ImageLoader::FreeTexture(TextureData& texture) {
    if (texture.data != nullptr) {
        stbi_image_free(texture.data);
        texture.data = nullptr;
    }
}

void Evolve::ImageLoader::DeleteTexture(TextureData& texture) {
    if (texture.id != 0) {
        glDeleteTextures(1, &texture.id);
        texture.id = 0;
    }
}
