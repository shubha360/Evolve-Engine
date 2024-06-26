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

#include "GlslProgram.h"
#include "Camera.h"
#include "RectDimension.h"
#include "UvDimension.h"
#include "Vertex2D.h"

namespace Evolve {

	enum class GlyphSortType {
		BY_TEXTURE_ID_INCREMENTAL,
		BY_TEXTURE_ID_DECREMENTAL,
		/*BY_DEPTH_INCREMENTAL,
		BY_DEPTH_DECREMENTAL*/
	};

	class TextureRenderer {
	public:
		TextureRenderer();
		~TextureRenderer();

		bool init(const std::string& pathToAssets);

		// the default shader will be used if no shader passed
		void begin();

		void draw(const RectDimension& destRect, const UvDimension& uvRect,
			GLuint textureID, const ColorRgba& color, int depth = 0);

		void end(const GlyphSortType& sortType = GlyphSortType::BY_TEXTURE_ID_INCREMENTAL);

		void renderTextures(Camera& camera, GlslProgram* shader = nullptr);

		void freeTextureRenderer();

	private:
		class Glyph {
		public:
			friend class TextureRenderer;
			friend class RenderBatch;

			Glyph(const RectDimension& destRect, const UvDimension& uvRect,
				GLuint textureID, const ColorRgba& color, int depth);

		private:
			GLuint textureID_ = 0;
			int depth_ = 0;

			Vertex2D vertices_[4] = {};
		};

		class RenderBatch {
		public:
			friend class TextureRenderer;
			RenderBatch(unsigned int offset, unsigned int numIndices, GLuint textureID);

		private:

			unsigned int offset_;
			unsigned int numIndices_;
			GLuint textureID_;
			GLuint iboID_ = 0;
		};

		GlslProgram defaultShader_;
		GlslProgram* currentShader_ = nullptr;

		bool inited_ = false;

		GLuint vaoID_ = 0, vboID_ = 0;
		std::vector<GLuint> iboIDs_;

		std::vector<Glyph> glyphs_;
		std::vector<Glyph*> glyphPointers_;
		std::vector<RenderBatch> renderBatches_;

		void createVao();
		void setupRenderBatches();
		void addIndicesToBuffer(std::vector<GLuint>& indices, unsigned int& currentIndex, unsigned int& currentVertex);

		static bool compareByTextureIdIncremental(Glyph* a, Glyph* b);
		static bool compareByTextureIdDecremental(Glyph* a, Glyph* b);
		/*static bool compareByDepthIncremental(Glyph* a, Glyph* b);
		static bool compareByDepthDecremental(Glyph* a, Glyph* b);*/
	};
}