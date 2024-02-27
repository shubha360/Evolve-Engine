#pragma once

#include "IncludeLibs.h"

#include "RectDimension.h"
#include "UVDimension.h"
#include "Vertex2D.h"

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
	BY_DEPTH_INCREMENTAL,
	BY_DEPTH_DECREMENTAL,
};

class TextureRenderer {
public:
	TextureRenderer();
	~TextureRenderer();

	void begin();

	void draw(const GlyphOrigin renderOrigin,  const RectDimension& destRect, const UVDimension& uvRect, 
		GLuint textureID, const ColorRGBA& color, int depth = 1);

	void end(const GlyphSortType& sortType = GlyphSortType::BY_TEXTURE_ID_INCREMENTAL);

	void renderTextures();

	void freeTextureRenderer();	

private:
	class Glyph {
	public:
		friend class TextureRenderer;
		friend class RenderBatch;

		Glyph(const GlyphOrigin renderOrigin, const RectDimension& destRect, const UVDimension& uvRect,
			GLuint textureID, const ColorRGBA& color, int depth);

	private:
		GLuint m_textureID = 0;
		int m_depth = 1;

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

	GLuint m_vaoID = 0, m_vboID = 0;
	std::vector<GLuint> m_iboIDs;

	//std::vector<GLuint> m_vertexIndices;

	std::vector<Glyph> m_glyphs;
	std::vector<Glyph*> m_glyphPointers;
	std::vector<RenderBatch> m_renderBatches;

	void createVao();
	void setupRenderBatches();
	void addIndicesToBuffer(std::vector<GLuint>& indices, unsigned int& currentIndex, unsigned int& currentVertex);

	static bool compareByTextureIdIncremental(Glyph* a, Glyph* b);
	static bool compareByTextureIdDecremental(Glyph* a, Glyph* b);
	static bool compareByDepthIncremental(Glyph* a, Glyph* b);
	static bool compareByDepthDecremental(Glyph* a, Glyph* b);
};