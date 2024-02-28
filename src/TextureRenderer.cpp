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

#include "../include/TextureRenderer.h"

TextureRenderer::Glyph::Glyph(const GlyphOrigin renderOrigin, const RectDimension& destRect, 
	const UvDimension& uvRect, GLuint textureID, 
	const ColorRgba& color, int depth) :
	
	m_textureID(textureID), m_depth(depth)
{
	
	this->m_textureID = textureID;

	int bottomLeftX = 0, bottomLeftY = 0;
	int width = destRect.width;
	int height = destRect.height;

	switch (renderOrigin) {
	case GlyphOrigin::BOTTOM_LEFT:
		bottomLeftX = destRect.x;
		bottomLeftY = destRect.y;
		break;

	case GlyphOrigin::BOTTOM_RIGHT:
		bottomLeftX = destRect.x - width;
		bottomLeftY = destRect.y;
		break;

	case GlyphOrigin::TOP_RIGHT:
		bottomLeftX = destRect.x - width;
		bottomLeftY = destRect.y - height;
		break;

	case GlyphOrigin::TOP_LEFT:
		bottomLeftX = destRect.x;
		bottomLeftY = destRect.y - height;
		break;

	case GlyphOrigin::CENTER:
		bottomLeftX = destRect.x - width / 2;
		bottomLeftY = destRect.y - height / 2;
		break;
	}

	// BOTTOM LEFT
	m_vertices[0].setPosition(bottomLeftX, bottomLeftY);
	m_vertices[0].setTextureCoords(uvRect.bottomLeftX, uvRect.bottomLeftY);
	m_vertices[0].color = color;

	// BOTTOM RIGHT
	m_vertices[1].setPosition(bottomLeftX + width, bottomLeftY);
	m_vertices[1].setTextureCoords(uvRect.bottomLeftX + uvRect.width, uvRect.bottomLeftY);
	m_vertices[1].color = color;

	// bottom RIGHT
	m_vertices[2].setPosition(bottomLeftX + width, bottomLeftY + height);
	m_vertices[2].setTextureCoords(uvRect.bottomLeftX + uvRect.width, uvRect.bottomLeftY + uvRect.height);
	m_vertices[2].color = color;

	// bottom LEFT
	m_vertices[3].setPosition(bottomLeftX, bottomLeftY + height);
	m_vertices[3].setTextureCoords(uvRect.bottomLeftX, uvRect.bottomLeftY + uvRect.height);
	m_vertices[3].color = color;
}

TextureRenderer::RenderBatch::RenderBatch(unsigned int offset, unsigned int numIndices, GLuint textureID) :
	m_offset(offset), m_numIndices(numIndices), m_textureID(textureID) {}

TextureRenderer::TextureRenderer() {}

TextureRenderer::~TextureRenderer() {
	freeTextureRenderer();
}

void TextureRenderer::begin() {

	if (m_vaoID == 0) {
		createVao();
	}

	m_glyphs.clear();
	m_glyphPointers.clear();
	m_renderBatches.clear();

	if (!m_iboIDs.empty()) {
		glDeleteBuffers((GLsizei) m_iboIDs.size(), m_iboIDs.data());
		m_iboIDs.clear();
	}
}

void TextureRenderer::draw(const GlyphOrigin renderOrigin, const RectDimension& destRect, const UvDimension& uvRect, 
	GLuint textureID, const ColorRgba& color, int depth /*= 1*/) {
	m_glyphs.emplace_back(renderOrigin, destRect, uvRect, textureID, color, depth);
}

void TextureRenderer::end(const GlyphSortType& sortType /*= GlyphSortType::BY_TEXTURE_ID_INCREMENTAL*/) {
	if (!m_glyphs.empty()) {
		m_glyphPointers.resize(m_glyphs.size());

		for (int i = 0; i < m_glyphPointers.size(); i++) {
			m_glyphPointers[i] = &m_glyphs[i];
		}

		switch (sortType) {
		
		case GlyphSortType::BY_TEXTURE_ID_INCREMENTAL:
			std::stable_sort(m_glyphPointers.begin(), m_glyphPointers.end(), compareByTextureIdIncremental);
			break;

		case GlyphSortType::BY_TEXTURE_ID_DECREMENTAL:
			std::stable_sort(m_glyphPointers.begin(), m_glyphPointers.end(), compareByTextureIdDecremental);
			break;

		case GlyphSortType::BY_DEPTH_INCREMENTAL:
			std::stable_sort(m_glyphPointers.begin(), m_glyphPointers.end(), compareByDepthIncremental);
			break;

		case GlyphSortType::BY_DEPTH_DECREMENTAL:
			std::stable_sort(m_glyphPointers.begin(), m_glyphPointers.end(), compareByDepthDecremental);
			break;
		}

		setupRenderBatches();
	}
}

void TextureRenderer::renderTextures() {
	if (!m_glyphPointers.empty()) {
		glBindVertexArray(m_vaoID);

		for (auto& batch : m_renderBatches) {
			glBindTexture(GL_TEXTURE_2D, batch.m_textureID);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batch.m_iboID);
			glDrawElements(GL_TRIANGLES, batch.m_numIndices, GL_UNSIGNED_INT, nullptr);
		}

		glBindVertexArray(0);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		//for (auto& id : m_iboIDs) {
		//	printf("%d ", id);
		//}
		//printf("\n\n");
	}
}

void TextureRenderer::freeTextureRenderer() {
	
	if (!m_iboIDs.empty()) {
		glDeleteBuffers((GLsizei) m_iboIDs.size(), m_iboIDs.data());
	}

	if (m_vboID != 0) {
		glDeleteBuffers(1, &m_vboID);
		m_vboID = 0;
	}
	
	if (m_vaoID != 0) {
		glDeleteVertexArrays(1, &m_vaoID);
		m_vaoID = 0;
	}	
}

void TextureRenderer::createVao() {
	if (m_vaoID == 0) {
		glGenVertexArrays(1, &m_vaoID);
	}

	if (m_vboID == 0) {
		glGenBuffers(1, &m_vboID);
	}

	glBindVertexArray(m_vaoID);

	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 2, GL_INT, GL_FALSE, sizeof(Vertex2D), (void*)offsetof(Vertex2D, position));
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex2D), (void*)offsetof(Vertex2D, color));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)offsetof(Vertex2D, textureCoords));

	glBindVertexArray(0);
}

void TextureRenderer::setupRenderBatches() {

	if (!m_glyphPointers.empty()) {

		{
			// setup the vbo and buffer vertex data
			std::vector<Vertex2D> vertices;
			vertices.resize(m_glyphPointers.size() * 4);

			unsigned int currentVertex = 0;

			for (auto& glyph : m_glyphPointers) {
				for (int vertex = 0; vertex < 4; vertex++) {
					vertices[currentVertex++] = glyph->m_vertices[vertex];
				}
			}

			glBindBuffer(GL_ARRAY_BUFFER, m_vboID);

			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex2D), nullptr, GL_DYNAMIC_DRAW);

			glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex2D), vertices.data());

			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}


		{
			// Setup render batches and the ibo
			std::vector<GLuint> vertexIndices;
			vertexIndices.resize(m_glyphPointers.size() * 6);

			unsigned int numIndices = 6;
			unsigned int curentIndex = 0;
			unsigned int currentVertex = 0;

			m_renderBatches.emplace_back(curentIndex, numIndices, m_glyphPointers[0]->m_textureID);
			addIndicesToBuffer(vertexIndices, curentIndex, currentVertex);

			for (int i = 1; i < m_glyphPointers.size(); i++) {

				if (m_glyphPointers[i]->m_textureID == m_glyphPointers[i - 1]->m_textureID) {
					m_renderBatches.back().m_numIndices += 6;
				}
				else {
					m_renderBatches.emplace_back(curentIndex, numIndices, m_glyphPointers[i]->m_textureID);
				}

				addIndicesToBuffer(vertexIndices, curentIndex, currentVertex);
			}

			m_iboIDs.resize(m_renderBatches.size());

			glGenBuffers((GLsizei) m_iboIDs.size(), m_iboIDs.data());

			for (int i = 0; i < m_iboIDs.size(); i++) {

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboIDs[i]);

				glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_renderBatches[i].m_numIndices * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);

				glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_renderBatches[i].m_numIndices * sizeof(GLuint), &vertexIndices[m_renderBatches[i].m_offset]);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

				m_renderBatches[i].m_iboID = m_iboIDs[i];
			}
		}
	}
}

void TextureRenderer::addIndicesToBuffer(std::vector<GLuint>& indices, 
	unsigned int& currentIndex, unsigned int& currentVertex) {
	
	// first triangle
	indices[currentIndex++] = currentVertex; // top left
	indices[currentIndex++] = currentVertex + 1; // top right
	indices[currentIndex++] = currentVertex + 2; // bottom right

	// second triangle
	indices[currentIndex++] = currentVertex; // top left
	indices[currentIndex++] = currentVertex + 3; // bottom left
	indices[currentIndex++] = currentVertex + 2; // bottom right

	currentVertex += 4;
}

bool TextureRenderer::compareByTextureIdIncremental(Glyph* a, Glyph* b) {
	return (a->m_textureID < b->m_textureID);
}

bool TextureRenderer::compareByTextureIdDecremental(Glyph* a, Glyph* b) {
	return (a->m_textureID > b->m_textureID);
}

bool TextureRenderer::compareByDepthIncremental(Glyph* a, Glyph* b) {
	return (a->m_depth < b->m_depth);
}

bool TextureRenderer::compareByDepthDecremental(Glyph* a, Glyph* b) {
	return (a->m_depth > b->m_depth);
}
