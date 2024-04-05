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
	depth_(depth), vertices_(vertices), numVertices_(numVertices), numIndices_(numIndices)
{}

Evolve::ShapeRenderer::Shape::~Shape() {}

bool Evolve::ShapeRenderer::init(const std::string& pathToAssets) {
	std::string vertShaderPath = pathToAssets + "/shaders/shape_shader.vert";
	std::string fragShaderPath = pathToAssets + "/shaders/shape_shader.frag";

	if (!defaultShader_.compileAndLinkShaders(
		vertShaderPath,
		fragShaderPath)) {
		EVOLVE_REPORT_ERROR("Failed to compile or link shape shader.", init);
		return false;
	}

	inited_ = true;
	return true;
}

void Evolve::ShapeRenderer::begin() {
	if (vaoID_ == 0) {
		createVao();
	}

	shapes_.clear();
	shapePointers_.clear();
	shapeBatches_.clear();

	totalVertices_ = 0;

	if (!iboIDs_.empty()) {
		glDeleteBuffers((GLsizei)iboIDs_.size(), iboIDs_.data());
		iboIDs_.clear();
	}
}

void Evolve::ShapeRenderer::drawTriangle(const Position2D& originPos, const Position2D& vertexTwoPos, 
	const Position2D& vertexThreePos, const ColorRgba& verticesColor, int depth /*= 0*/) {

	std::vector<Vertex2D> vertices(3);	

	vertices[0].setPosition(originPos);
	vertices[0].setColor(verticesColor);

	vertices[1].setPosition(vertexTwoPos);
	vertices[1].setColor(verticesColor);

	vertices[2].setPosition(vertexThreePos);
	vertices[2].setColor(verticesColor);

	shapes_.emplace_back(depth, vertices, 3, 3);
	
	totalVertices_ += shapes_.back().numVertices_;
	totalIndices_ += shapes_.back().numIndices_;
}

void Evolve::ShapeRenderer::drawTriangle(
	const Position2D& originPos, const ColorRgba& originColor,
	const Position2D& vertexTwoPos, const ColorRgba& vertexTwoColor,
	const Position2D& vertexThreePos, const ColorRgba& vertexThreeColor,
	int depth /*= 0*/) {

	std::vector<Vertex2D> vertices(3);

	vertices[0].setPosition(originPos);
	vertices[0].setColor(originColor);

	vertices[1].setPosition(vertexTwoPos);
	vertices[1].setColor(vertexTwoColor);

	vertices[2].setPosition(vertexThreePos);
	vertices[2].setColor(vertexThreeColor);

	shapes_.emplace_back(depth, vertices, 3, 3);

	totalVertices_ += shapes_.back().numVertices_;
	totalIndices_ += shapes_.back().numIndices_;
}

void Evolve::ShapeRenderer::drawRectangle(const Position2D& originPos, const Position2D& vertexTwoPos, 
	const Position2D& vertexThreePos, const Position2D& vertexFourPos, 
	const ColorRgba& verticesColor, int depth /*= 0*/) {

	std::vector<Vertex2D> vertices(4);

	vertices[0].setPosition(originPos);
	vertices[0].setColor(verticesColor);

	vertices[1].setPosition(vertexTwoPos);
	vertices[1].setColor(verticesColor);

	vertices[2].setPosition(vertexThreePos);
	vertices[2].setColor(verticesColor);

	vertices[3].setPosition(vertexFourPos);
	vertices[3].setColor(verticesColor);

	shapes_.emplace_back(depth, vertices, 4, 6);

	totalVertices_ += shapes_.back().numVertices_;
	totalIndices_ += shapes_.back().numIndices_;
}

void Evolve::ShapeRenderer::drawRectangle(const Position2D& originPos, const ColorRgba& originColor,
	const Position2D& vertexTwoPos, const ColorRgba& vertexTwoColor,
	const Position2D& vertexThreePos, const ColorRgba& vertexThreeColor,
	const Position2D& vertexFourPos, const ColorRgba& vertexFourColor,
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

	shapes_.emplace_back(depth, vertices, 4, 6);

	totalVertices_ += shapes_.back().numVertices_;
	totalIndices_ += shapes_.back().numIndices_;
}

void Evolve::ShapeRenderer::drawRectangle(const RectDimension& destRect,
	const ColorRgba& verticesColor, int depth /*= 0*/) {

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

	shapes_.emplace_back(depth, vertices, 4, 6);

	totalVertices_ += shapes_.back().numVertices_;
	totalIndices_ += shapes_.back().numIndices_;
}

void Evolve::ShapeRenderer::drawCircle(const Position2D& centerPos, unsigned int radius, 
	const ColorRgba& color, int depth) {
	
	static const int NUM_TRIANGLES = 60;

	Position2D vertexTwoPos { 0, 0 };
	bool makeTriangle = false;

	for (size_t i = 0; i <= NUM_TRIANGLES; i++) {

		double angle = (M_PI * 2.0f) * ((float)i / (float)NUM_TRIANGLES);

		if (!makeTriangle) {
			vertexTwoPos.X = (GLint) (centerPos.X + cos(angle) * radius);
			vertexTwoPos.X = (GLint) (centerPos.Y + sin(angle) * radius);

			makeTriangle = true;
		}
		else {
			Position2D vertexThreePos{
				(GLint) (centerPos.X + cos(angle) * radius),
				(GLint) (centerPos.Y + sin(angle) * radius)
			};

			drawTriangle(centerPos, vertexTwoPos, vertexThreePos, color, depth);

			makeTriangle = false;
			i--;
		}
	}
}

void Evolve::ShapeRenderer::end(const ShapeSortType& sortType /*= ShapeSortType::BY_DEPTH_INCREMENTAL*/) {
	if (!inited_) {
		EVOLVE_REPORT_ERROR("Texture renderer not initialized.", begin);
		return;
	}

	if (!shapes_.empty()) {
		shapePointers_.resize(shapes_.size());

		for (size_t i = 0; i < shapePointers_.size(); i++) {
			shapePointers_[i] = &shapes_[i];
		}

		switch (sortType) {

		case ShapeSortType::BY_DEPTH_INCREMENTAL:
			std::stable_sort(shapePointers_.begin(), shapePointers_.end(), compareByDepthIncremental);
			break;

		case ShapeSortType::BY_DEPTH_DECREMENTAL:
			std::stable_sort(shapePointers_.begin(), shapePointers_.end(), compareByDepthDecremental);
			break;
		}

		setupShapeBatches();
	}
}

void Evolve::ShapeRenderer::renderShapes(Camera& camera, GlslProgram* shader /*= nullptr*/) {
	if (!inited_) {
		EVOLVE_REPORT_ERROR("Shape renderer not initialized.", begin);
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

	if (!shapeBatches_.empty()) {
		glBindVertexArray(vaoID_);

		for (auto& batch : shapeBatches_) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batch.iboID_);
			glDrawElements(GL_TRIANGLES, batch.numIndices_, GL_UNSIGNED_INT, nullptr);
		}

		glBindVertexArray(0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
	}

	currentShader_->unuseProgram();
}

void Evolve::ShapeRenderer::freeShapeRenderer() {
	if (inited_) {
		defaultShader_.freeProgram();
	}

	if (!iboIDs_.empty()) {
		glDeleteBuffers((GLsizei)iboIDs_.size(), iboIDs_.data());
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

void Evolve::ShapeRenderer::createVao() {
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

	glVertexAttribPointer(0, 2, GL_INT, GL_FALSE, sizeof(Vertex2D), (void*) offsetof(Vertex2D, Position));
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex2D), (void*) offsetof(Vertex2D, Color));

	glBindVertexArray(0);
}

void Evolve::ShapeRenderer::setupShapeBatches() {
	if (!shapePointers_.empty()) {
		{
			// setup the vbo and buffer vertex data
			std::vector<Vertex2D> vertices;
			vertices.resize(totalVertices_);

			unsigned int currentVertex = 0;

			for (auto& shape : shapePointers_) {
				for (size_t vertex = 0; vertex < shape->vertices_.size(); vertex++) {
					vertices[currentVertex++] = shape->vertices_[vertex];
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
			vertexIndices.resize(totalIndices_);

			unsigned int curentIndex = 0;
			unsigned int currentVertex = 0;

			shapeBatches_.emplace_back(curentIndex, shapePointers_[0]->numIndices_);
			addIndicesToBuffer(vertexIndices, shapePointers_[0]->numIndices_, curentIndex, currentVertex);

			for (size_t i = 1; i < shapePointers_.size(); i++) {

				auto numIndices = shapePointers_[i]->numIndices_;

				if (shapePointers_[i]->depth_ == shapePointers_[i - 1]->depth_) {
					shapeBatches_.back().numIndices_ += numIndices;
				}
				else {
					shapeBatches_.emplace_back(curentIndex, numIndices);
				}

				addIndicesToBuffer(vertexIndices, numIndices, curentIndex, currentVertex);
			}

			iboIDs_.resize(shapeBatches_.size());

			glGenBuffers((GLsizei)iboIDs_.size(), iboIDs_.data());

			for (size_t i = 0; i < iboIDs_.size(); i++) {

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboIDs_[i]);

				glBufferData(GL_ELEMENT_ARRAY_BUFFER, shapeBatches_[i].numIndices_ * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);

				glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, shapeBatches_[i].numIndices_ * sizeof(GLuint), &vertexIndices[shapeBatches_[i].offset_]);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

				shapeBatches_[i].iboID_ = iboIDs_[i];
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
	return (a->depth_ < b->depth_);
}

bool Evolve::ShapeRenderer::compareByDepthDecremental(Shape* a, Shape* b) {
	return (a->depth_ > b->depth_);
}

Evolve::ShapeRenderer::ShapeBatch::ShapeBatch(unsigned int offset, unsigned int numIndices) :
	offset_(offset), numIndices_(numIndices)
{}
