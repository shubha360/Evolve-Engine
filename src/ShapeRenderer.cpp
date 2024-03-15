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

#include "../include/Evolve/ShapeRenderer.h"

Evolve::ShapeRenderer::Shape::Shape(int depth, std::vector<Vertex2D> vertices, 
	const unsigned int numVertices, const unsigned int numIndices) :
	m_depth(depth), m_vertices(vertices), m_numVertices(numVertices), m_numIndices(numIndices)
{}

Evolve::ShapeRenderer::Shape::~Shape() {}

bool Evolve::ShapeRenderer::init(const std::string& pathToAssets) {
	std::string vertShaderPath = pathToAssets + "/shaders/shape_shader.vert";
	std::string fragShaderPath = pathToAssets + "/shaders/shape_shader.frag";

	if (!m_defaultShader.compileAndLinkShaders(
		vertShaderPath,
		fragShaderPath)) {
		EVOLVE_REPORT_ERROR("Failed to compile or link shape shader.", init);
		return false;
	}

	m_inited = true;
	return true;
}

void Evolve::ShapeRenderer::begin() {
	if (m_vaoID == 0) {
		createVao();
	}

	m_shapes.clear();
	m_shapePointers.clear();
	m_shapeBatches.clear();

	m_totalVertices = 0;

	if (!m_iboIDs.empty()) {
		glDeleteBuffers((GLsizei)m_iboIDs.size(), m_iboIDs.data());
		m_iboIDs.clear();
	}
}

void Evolve::ShapeRenderer::drawTriangle(const glm::ivec2 originPos, const glm::ivec2 vertexTwoPos, 
	const glm::ivec2 vertexThreePos, const ColorRgba& verticesColor, int depth /*= 0*/) {

	std::vector<Vertex2D> vertices(3);	

	vertices[0].setPosition(originPos);
	vertices[0].setColor(verticesColor);

	vertices[1].setPosition(vertexTwoPos);
	vertices[1].setColor(verticesColor);

	vertices[2].setPosition(vertexThreePos);
	vertices[2].setColor(verticesColor);

	m_shapes.emplace_back(depth, vertices, 3, 3);
	
	m_totalVertices += m_shapes.back().m_numVertices;
	m_totalIndices += m_shapes.back().m_numIndices;
}

void Evolve::ShapeRenderer::drawTriangle(
	const glm::ivec2 originPos, ColorRgba& originColor,
	const glm::ivec2 vertexTwoPos, ColorRgba& vertexTwoColor,
	const glm::ivec2 vertexThreePos, ColorRgba& vertexThreeColor, 
	int depth /*= 0*/) {

	std::vector<Vertex2D> vertices(3);

	vertices[0].setPosition(originPos);
	vertices[0].setColor(originColor);

	vertices[1].setPosition(vertexTwoPos);
	vertices[1].setColor(vertexTwoColor);

	vertices[2].setPosition(vertexThreePos);
	vertices[2].setColor(vertexThreeColor);

	m_shapes.emplace_back(depth, vertices, 3, 3);

	m_totalVertices += m_shapes.back().m_numVertices;
	m_totalIndices += m_shapes.back().m_numIndices;
}

void Evolve::ShapeRenderer::drawRectangle(const glm::ivec2 originPos, const glm::ivec2 vertexTwoPos, 
	const glm::ivec2 vertexThreePos, const glm::ivec2 vertexFourPos, 
	ColorRgba& verticesColor, int depth /*= 0*/) {

	std::vector<Vertex2D> vertices(4);

	vertices[0].setPosition(originPos);
	vertices[0].setColor(verticesColor);

	vertices[1].setPosition(vertexTwoPos);
	vertices[1].setColor(verticesColor);

	vertices[2].setPosition(vertexThreePos);
	vertices[2].setColor(verticesColor);

	vertices[3].setPosition(vertexFourPos);
	vertices[3].setColor(verticesColor);

	m_shapes.emplace_back(depth, vertices, 4, 6);

	m_totalVertices += m_shapes.back().m_numVertices;
	m_totalIndices += m_shapes.back().m_numIndices;
}

void Evolve::ShapeRenderer::drawRectangle(const glm::ivec2 originPos, ColorRgba& originColor, 
	const glm::ivec2 vertexTwoPos, ColorRgba& vertexTwoColor, 
	const glm::ivec2 vertexThreePos, ColorRgba& vertexThreeColor, 
	const glm::ivec2 vertexFourPos, ColorRgba& vertexFourColor, 
	int depth /*= 0*/) {

	std::vector<Vertex2D> vertices(4);

	vertices[0].setPosition(originPos);
	vertices[0].setColor(originColor);

	vertices[1].setPosition(vertexTwoPos);
	vertices[1].setColor(vertexTwoColor);

	vertices[2].setPosition(vertexThreePos);
	vertices[2].setColor(vertexThreeColor);

	vertices[3].setPosition(vertexFourPos);
	vertices[3].setColor(vertexFourColor);

	m_shapes.emplace_back(depth, vertices, 4, 6);

	m_totalVertices += m_shapes.back().m_numVertices;
	m_totalIndices += m_shapes.back().m_numIndices;
}

void Evolve::ShapeRenderer::drawRectangle(const RectDimension& destRect,
	ColorRgba& verticesColor, int depth /*= 0*/) {

	std::vector<Vertex2D> vertices(4);
	
	// bottom left
	vertices[0].setPosition(destRect.getLeft(), destRect.getBottom());
	vertices[0].setColor(verticesColor);

	// bottom right
	vertices[1].setPosition(destRect.getRight(), destRect.getBottom());
	vertices[1].setColor(verticesColor);

	// top right
	vertices[2].setPosition(destRect.getRight(), destRect.getTop());
	vertices[2].setColor(verticesColor);

	// top left
	vertices[3].setPosition(destRect.getLeft(), destRect.getTop());
	vertices[3].setColor(verticesColor);

	m_shapes.emplace_back(depth, vertices, 4, 6);

	m_totalVertices += m_shapes.back().m_numVertices;
	m_totalIndices += m_shapes.back().m_numIndices;
}

void Evolve::ShapeRenderer::drawCircle(const glm::ivec2& centerPos, unsigned int radius, ColorRgba& color, int depth) {
	static const int NUM_TRIANGLES = 60;

	glm::ivec2 vertexTwoPos(0, 0);
	bool makeTriangle = false;

	for (size_t i = 0; i <= NUM_TRIANGLES; i++) {

		double angle = (M_PI * 2.0f) * ((float)i / (float)NUM_TRIANGLES);

		if (!makeTriangle) {
			vertexTwoPos.x = (int) (centerPos.x + cos(angle) * radius);
			vertexTwoPos.y = (int) (centerPos.y + sin(angle) * radius);

			makeTriangle = true;
		}
		else {
			glm::ivec2 vertexThreePos(
				centerPos.x + cos(angle) * radius,
				centerPos.y + sin(angle) * radius
			);

			drawTriangle(centerPos, vertexTwoPos, vertexThreePos, color, depth);

			makeTriangle = false;
			i--;
		}
	}
}

void Evolve::ShapeRenderer::end(const ShapeSortType& sortType /*= ShapeSortType::BY_DEPTH_INCREMENTAL*/) {
	if (!m_inited) {
		EVOLVE_REPORT_ERROR("Texture renderer not initialized.", begin);
		return;
	}

	if (!m_shapes.empty()) {
		m_shapePointers.resize(m_shapes.size());

		for (size_t i = 0; i < m_shapePointers.size(); i++) {
			m_shapePointers[i] = &m_shapes[i];
		}

		switch (sortType) {

		case ShapeSortType::BY_DEPTH_INCREMENTAL:
			std::stable_sort(m_shapePointers.begin(), m_shapePointers.end(), compareByDepthIncremental);
			break;

		case ShapeSortType::BY_DEPTH_DECREMENTAL:
			std::stable_sort(m_shapePointers.begin(), m_shapePointers.end(), compareByDepthDecremental);
			break;
		}

		setupShapeBatches();
	}
}

void Evolve::ShapeRenderer::renderShapes(Camera& camera, GlslProgram* shader /*= nullptr*/) {
	if (!m_inited) {
		EVOLVE_REPORT_ERROR("Shape renderer not initialized.", begin);
		return;
	}

	if (shader != nullptr) {
		m_currentShader = shader;
	}
	else {
		m_currentShader = &m_defaultShader;
	}

	m_currentShader->useProgram();

	camera.sendMatrixDataToShader(*m_currentShader);

	if (!m_shapeBatches.empty()) {
		glBindVertexArray(m_vaoID);

		for (auto& batch : m_shapeBatches) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batch.m_iboID);
			glDrawElements(GL_TRIANGLES, batch.m_numIndices, GL_UNSIGNED_INT, nullptr);
		}

		glBindVertexArray(0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
	}

	m_currentShader->unuseProgram();
}

void Evolve::ShapeRenderer::freeShapeRenderer() {
	if (m_inited) {
		m_defaultShader.freeProgram();
	}

	if (!m_iboIDs.empty()) {
		glDeleteBuffers((GLsizei)m_iboIDs.size(), m_iboIDs.data());
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

void Evolve::ShapeRenderer::createVao() {
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

	glVertexAttribPointer(0, 2, GL_INT, GL_FALSE, sizeof(Vertex2D), (void*)offsetof(Vertex2D, position));
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex2D), (void*)offsetof(Vertex2D, color));

	glBindVertexArray(0);
}

void Evolve::ShapeRenderer::setupShapeBatches() {
	if (!m_shapePointers.empty()) {
		{
			// setup the vbo and buffer vertex data
			std::vector<Vertex2D> vertices;
			vertices.resize(m_totalVertices);

			unsigned int currentVertex = 0;

			for (auto& shape : m_shapePointers) {
				for (size_t vertex = 0; vertex < shape->m_vertices.size(); vertex++) {
					vertices[currentVertex++] = shape->m_vertices[vertex];
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
			vertexIndices.resize(m_totalIndices);

			unsigned int curentIndex = 0;
			unsigned int currentVertex = 0;

			m_shapeBatches.emplace_back(curentIndex, m_shapePointers[0]->m_numIndices);
			addIndicesToBuffer(vertexIndices, m_shapePointers[0]->m_numIndices, curentIndex, currentVertex);

			for (size_t i = 1; i < m_shapePointers.size(); i++) {

				auto numIndices = m_shapePointers[i]->m_numIndices;

				if (m_shapePointers[i]->m_depth == m_shapePointers[i - 1]->m_depth) {
					m_shapeBatches.back().m_numIndices += numIndices;
				}
				else {
					m_shapeBatches.emplace_back(curentIndex, numIndices);
				}

				addIndicesToBuffer(vertexIndices, numIndices, curentIndex, currentVertex);
			}

			m_iboIDs.resize(m_shapeBatches.size());

			glGenBuffers((GLsizei)m_iboIDs.size(), m_iboIDs.data());

			for (size_t i = 0; i < m_iboIDs.size(); i++) {

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboIDs[i]);

				glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_shapeBatches[i].m_numIndices * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);

				glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_shapeBatches[i].m_numIndices * sizeof(GLuint), &vertexIndices[m_shapeBatches[i].m_offset]);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

				m_shapeBatches[i].m_iboID = m_iboIDs[i];
			}
		}
	}
}

void Evolve::ShapeRenderer::addIndicesToBuffer(std::vector<GLuint>& indices, const int numIndices, 
	unsigned int& currentIndex, unsigned int& currentVertex) {

	// triangle
	if (numIndices == 3) {
		indices[currentIndex++] = currentVertex;
		indices[currentIndex++] = currentVertex + 1; 
		indices[currentIndex++] = currentVertex + 2;

		currentVertex += 3;
	}
	
	// quad
	else if (numIndices == 6) {
	
		// first triangle
		indices[currentIndex++] = currentVertex; // botom left
		indices[currentIndex++] = currentVertex + 1; // bottom right
		indices[currentIndex++] = currentVertex + 2; // top right

		// second triangle
		indices[currentIndex++] = currentVertex; // bottom left
		indices[currentIndex++] = currentVertex + 3; // top left
		indices[currentIndex++] = currentVertex + 2; // top right

		currentVertex += 4;
	}

	// circle
	else {
		for (int i = 0; i < numIndices; i++) {
			indices[currentIndex++] = currentVertex + i;
		}

		currentVertex += numIndices;
	}
}

bool Evolve::ShapeRenderer::compareByDepthIncremental(Shape* a, Shape* b) {
	return (a->m_depth < b->m_depth);
}

bool Evolve::ShapeRenderer::compareByDepthDecremental(Shape* a, Shape* b) {
	return (a->m_depth > b->m_depth);
}

Evolve::ShapeRenderer::ShapeBatch::ShapeBatch(unsigned int offset, unsigned int numIndices) :
	m_offset(offset), m_numIndices(numIndices)
{}
