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

#include "../include/Evolve/TextureRenderer.h"

Evolve::TextureRenderer::Glyph::Glyph(const RectDimension& destRect,
	const UvDimension& uvRect, GLuint textureID, 
	const ColorRgba& color, int depth) :
	
	textureID_(textureID), depth_(depth)
{
	this->textureID_ = textureID;

	// BOTTOM LEFT
	vertices_[0].setPosition(destRect.getLeft(), destRect.getBottom());
	vertices_[0].setTextureCoords(uvRect.BottomLeftX, uvRect.BottomLeftY);
	vertices_[0].setColor(color);

	// BOTTOM RIGHT
	vertices_[1].setPosition(destRect.getRight(), destRect.getBottom());
	vertices_[1].setTextureCoords(uvRect.BottomLeftX + uvRect.Width, uvRect.BottomLeftY);
	vertices_[1].setColor(color);

	// TOP RIGHT
	vertices_[2].setPosition(destRect.getRight(), destRect.getTop());
	vertices_[2].setTextureCoords(uvRect.BottomLeftX + uvRect.Width, uvRect.BottomLeftY + uvRect.Height);
	vertices_[2].setColor(color);

	// TOP LEFT
	vertices_[3].setPosition(destRect.getLeft(), destRect.getTop());
	vertices_[3].setTextureCoords(uvRect.BottomLeftX, uvRect.BottomLeftY + uvRect.Height);
	vertices_[3].setColor(color);
}

Evolve::TextureRenderer::RenderBatch::RenderBatch(unsigned int offset, unsigned int numIndices, GLuint textureID) :
	offset_(offset), numIndices_(numIndices), textureID_(textureID) {}

Evolve::TextureRenderer::TextureRenderer() {}

Evolve::TextureRenderer::~TextureRenderer() {
	freeTextureRenderer();
}

bool Evolve::TextureRenderer::init(const std::string& pathToAssets) {
	
	std::string vertShaderPath = pathToAssets + "/shaders/texture_shader.vert";
	std::string fragShaderPath = pathToAssets + "/shaders/texture_shader.frag";

	if (!defaultShader_.compileAndLinkShaders(
		vertShaderPath,
		fragShaderPath)) {
		EVOLVE_REPORT_ERROR("Failed to compile or link texture shader.", init);
		return false;
	}

	inited_ = true;
	return true;
}

void Evolve::TextureRenderer::begin() {

	if (!inited_) {
		EVOLVE_REPORT_ERROR("Texture renderer not initialized.", begin);
		return;
	}

	if (vaoID_ == 0) {
		createVao();
	}

	glyphs_.clear();
	glyphPointers_.clear();
	renderBatches_.clear();

	if (!iboIDs_.empty()) {
		glDeleteBuffers((GLsizei) iboIDs_.size(), iboIDs_.data());
		iboIDs_.clear();
	}
}

void Evolve::TextureRenderer::draw(const RectDimension& destRect, const UvDimension& uvRect,
	GLuint textureID, const ColorRgba& color, int depth /*= 1*/) {

	if (!inited_) {
		EVOLVE_REPORT_ERROR("Texture renderer not initialized.", begin);
		return;
	}

	glyphs_.emplace_back(destRect, uvRect, textureID, color, depth);
}

void Evolve::TextureRenderer::end(const GlyphSortType& sortType /*= GlyphSortType::BY_TEXTURE_ID_INCREMENTAL*/) {

	if (!inited_) {
		EVOLVE_REPORT_ERROR("Texture renderer not initialized.", begin);
		return;
	}

	if (!glyphs_.empty()) {
		glyphPointers_.resize(glyphs_.size());

		for (size_t i = 0; i < glyphPointers_.size(); i++) {
			glyphPointers_[i] = &glyphs_[i];
		}

		switch (sortType) {
		
		case GlyphSortType::BY_TEXTURE_ID_INCREMENTAL:
			std::stable_sort(glyphPointers_.begin(), glyphPointers_.end(), compareByTextureIdIncremental);
			break;

		case GlyphSortType::BY_TEXTURE_ID_DECREMENTAL:
			std::stable_sort(glyphPointers_.begin(), glyphPointers_.end(), compareByTextureIdDecremental);
			break;

		/*case GlyphSortType::BY_DEPTH_INCREMENTAL:
			std::stable_sort(glyphPointers_.begin(), glyphPointers_.end(), compareByDepthIncremental);
			break;

		case GlyphSortType::BY_DEPTH_DECREMENTAL:
			std::stable_sort(glyphPointers_.begin(), glyphPointers_.end(), compareByDepthDecremental);
			break;*/
		}

		setupRenderBatches();
	}
}

void Evolve::TextureRenderer::renderTextures(Camera& camera, GlslProgram* shader /*= nullptr*/) {

	if (!inited_) {
		EVOLVE_REPORT_ERROR("Texture renderer not initialized.", begin);
		return;
	}

	if (shader != nullptr) {
		currentShader_ = shader;
	}
	else {
		currentShader_ = &defaultShader_;
	}

	currentShader_->useProgram();

	camera.sendMatrixDataToShader(*currentShader_);

	glActiveTexture(GL_TEXTURE0);
	GLint samplerLoc = currentShader_->getUniformLocation("u_imageSampler");
	glUniform1i(samplerLoc, 0);

	if (!renderBatches_.empty()) {
		glBindVertexArray(vaoID_);

		for (auto& batch : renderBatches_) {
			glBindTexture(GL_TEXTURE_2D, batch.textureID_);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batch.iboID_);
			glDrawElements(GL_TRIANGLES, batch.numIndices_, GL_UNSIGNED_INT, nullptr);
		}

		glBindVertexArray(0);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
	}

	currentShader_->unuseProgram();
}

void Evolve::TextureRenderer::freeTextureRenderer() {
	
	if (inited_) {
		defaultShader_.freeProgram();
	}

	if (!iboIDs_.empty()) {
		glDeleteBuffers((GLsizei) iboIDs_.size(), iboIDs_.data());
	}

	if (vboID_ != 0) {
		glDeleteBuffers(1, &vboID_);
		vboID_ = 0;
	}
	
	if (vaoID_ != 0) {
		glDeleteVertexArrays(1, &vaoID_);
		vaoID_ = 0;
	}
}

void Evolve::TextureRenderer::createVao() {
	if (vaoID_ == 0) {
		glGenVertexArrays(1, &vaoID_);
	}

	if (vboID_ == 0) {
		glGenBuffers(1, &vboID_);
	}

	glBindVertexArray(vaoID_);

	glBindBuffer(GL_ARRAY_BUFFER, vboID_);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 2, GL_INT, GL_FALSE, sizeof(Vertex2D), (void*) offsetof(Vertex2D, Position));
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex2D), (void*) offsetof(Vertex2D, Color));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*) offsetof(Vertex2D, TextureCoords));

	glBindVertexArray(0);
}

void Evolve::TextureRenderer::setupRenderBatches() {

	if (!glyphPointers_.empty()) {

		{
			// setup the vbo and buffer vertex data
			std::vector<Vertex2D> vertices;
			vertices.resize(glyphPointers_.size() * 4);

			unsigned int currentVertex = 0;

			for (auto& glyph : glyphPointers_) {
				for (int vertex = 0; vertex < 4; vertex++) {
					vertices[currentVertex++] = glyph->vertices_[vertex];
				}
			}

			glBindBuffer(GL_ARRAY_BUFFER, vboID_);

			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex2D), nullptr, GL_DYNAMIC_DRAW);

			glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex2D), vertices.data());

			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}


		{
			// Setup render batches and the ibo
			std::vector<GLuint> vertexIndices;
			vertexIndices.resize(glyphPointers_.size() * 6);

			unsigned int numIndices = 6;
			unsigned int curentIndex = 0;
			unsigned int currentVertex = 0;

			renderBatches_.emplace_back(curentIndex, numIndices, glyphPointers_[0]->textureID_);
			addIndicesToBuffer(vertexIndices, curentIndex, currentVertex);

			for (size_t i = 1; i < glyphPointers_.size(); i++) {

				if (glyphPointers_[i]->textureID_ == glyphPointers_[i - 1]->textureID_) {
					renderBatches_.back().numIndices_ += 6;
				}
				else {
					renderBatches_.emplace_back(curentIndex, numIndices, glyphPointers_[i]->textureID_);
				}

				addIndicesToBuffer(vertexIndices, curentIndex, currentVertex);
			}

			iboIDs_.resize(renderBatches_.size());

			glGenBuffers((GLsizei) iboIDs_.size(), iboIDs_.data());

			for (size_t i = 0; i < iboIDs_.size(); i++) {

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboIDs_[i]);

				glBufferData(GL_ELEMENT_ARRAY_BUFFER, renderBatches_[i].numIndices_ * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);

				glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, renderBatches_[i].numIndices_ * sizeof(GLuint), &vertexIndices[renderBatches_[i].offset_]);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

				renderBatches_[i].iboID_ = iboIDs_[i];
			}
		}
	}
}

void Evolve::TextureRenderer::addIndicesToBuffer(std::vector<GLuint>& indices,
	unsigned int& currentIndex, unsigned int& currentVertex) {
	
	// first triangle
	indices[currentIndex++] = currentVertex; // bottom left
	indices[currentIndex++] = currentVertex + 1; // bottom right
	indices[currentIndex++] = currentVertex + 2; // top right

	// second triangle
	indices[currentIndex++] = currentVertex; // bottom left
	indices[currentIndex++] = currentVertex + 3; // top left
	indices[currentIndex++] = currentVertex + 2; // top right

	currentVertex += 4;
}

bool Evolve::TextureRenderer::compareByTextureIdIncremental(Glyph* a, Glyph* b) {
	return (a->textureID_ < b->textureID_);
}

bool Evolve::TextureRenderer::compareByTextureIdDecremental(Glyph* a, Glyph* b) {
	return (a->textureID_ > b->textureID_);
}

//bool Evolve::TextureRenderer::compareByDepthIncremental(Glyph* a, Glyph* b) {
//	return (a->depth_ < b->depth_);
//}
//
//bool Evolve::TextureRenderer::compareByDepthDecremental(Glyph* a, Glyph* b) {
//	return (a->depth_ > b->depth_);
//}