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

	enum class GlyphOrigin {
		BOTTOM_LEFT,
		BOTTOM_RIGHT,
		TOP_RIGHT,
		TOP_LEFT,
		CENTER
	};

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
		void begin(Camera& camera, GlslProgram* shader = nullptr);

		void draw(const GlyphOrigin renderOrigin, const RectDimension& destRect, const UvDimension& uvRect,
			GLuint textureID, const ColorRgba& color, int depth = 0);

		void end(const GlyphSortType& sortType = GlyphSortType::BY_TEXTURE_ID_INCREMENTAL);

		void renderTextures();

		void freeTextureRenderer();

	private:
		class Glyph {
		public:
			friend class TextureRenderer;
			friend class RenderBatch;

			Glyph(const GlyphOrigin renderOrigin, const RectDimension& destRect, const UvDimension& uvRect,
				GLuint textureID, const ColorRgba& color, int depth);

		private:
			GLuint m_textureID = 0;
			int m_depth = 0;

			Vertex2D m_vertices[4] = {};
		};

		class RenderBatch {
		public:
			friend class TextureRenderer;
			RenderBatch(unsigned int offset, unsigned int numIndices, GLuint textureID);

		private:

			unsigned int m_offset;
			unsigned int m_numIndices;
			GLuint m_textureID;
			GLuint m_iboID = 0;
		};

		GlslProgram m_defaultShader;
		GlslProgram* m_currentShader = nullptr;

		bool m_inited = false;

		GLuint m_vaoID = 0, m_vboID = 0;
		std::vector<GLuint> m_iboIDs;

		std::vector<Glyph> m_glyphs;
		std::vector<Glyph*> m_glyphPointers;
		std::vector<RenderBatch> m_renderBatches;

		void createVao();
		void setupRenderBatches();
		void addIndicesToBuffer(std::vector<GLuint>& indices, unsigned int& currentIndex, unsigned int& currentVertex);

		static bool compareByTextureIdIncremental(Glyph* a, Glyph* b);
		static bool compareByTextureIdDecremental(Glyph* a, Glyph* b);
		/*static bool compareByDepthIncremental(Glyph* a, Glyph* b);
		static bool compareByDepthDecremental(Glyph* a, Glyph* b);*/
	};
}