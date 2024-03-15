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
#include "Vertex2D.h"
#include "Camera.h"
#include "RectDimension.h"

namespace Evolve {

	enum class ShapeSortType {
		BY_DEPTH_INCREMENTAL,
		BY_DEPTH_DECREMENTAL
	};

	class ShapeRenderer {
	public:
		ShapeRenderer() {};
		~ShapeRenderer() {};

		bool init(const std::string& pathToAssets);

		// the default shader will be used if no shader passed
		void begin();

		void drawTriangle(const glm::ivec2 originPos, const glm::ivec2 vertexTwoPos, const glm::ivec2 vertexThreePos, 
			const ColorRgba& verticesColor, const int depth = 0);

		void drawTriangle(const glm::ivec2 originPos, ColorRgba& originColor, 
			const glm::ivec2 vertexTwoPos, ColorRgba& vertexTwoColor,
			const glm::ivec2 vertexThreePos, ColorRgba& vertexThreeColor,
			int depth = 0);

		void drawRectangle(const glm::ivec2 originPos, const glm::ivec2 vertexTwoPos, 
			const glm::ivec2 vertexThreePos, const glm::ivec2 vertexFourPos, 
			ColorRgba& verticesColor, int depth = 0);

		void drawRectangle(const glm::ivec2 originPos, ColorRgba& originColor, 
			const glm::ivec2 vertexTwoPos, ColorRgba& vertexTwoColor,
			const glm::ivec2 vertexThreePos, ColorRgba& vertexThreeColor,
			const glm::ivec2 vertexFourPos, ColorRgba& vertexFourColor, 
			int depth = 0);

		void drawRectangle(const RectDimension& destRect,
			ColorRgba& verticesColor, int depth = 0);

		// this is very inefficient, it's better to use a circular texture using TextureRenderer
		void drawCircle(const glm::ivec2& centerPos, unsigned int radius, ColorRgba& color, int depth = 0);

		void end(const ShapeSortType& sortType = ShapeSortType::BY_DEPTH_INCREMENTAL);

		void renderShapes(Camera& camera, GlslProgram* shader = nullptr);

		void freeShapeRenderer();

	private:
		class Shape {
		public:
			friend class ShapeRenderer;
			friend class ShapeBatch;

			Shape(int depth, std::vector<Vertex2D> vertices, 
				const unsigned int numVertices, const unsigned int numIndices);

			~Shape();

		private:
			int m_depth = 0;

			std::vector<Vertex2D> m_vertices;

			unsigned int m_numVertices = 0, m_numIndices = 0;
		};

		class ShapeBatch {
		public:
			friend class ShapeRenderer;
			
			ShapeBatch(unsigned int offset, unsigned int numIndices);

		private:
			unsigned int m_offset;
			unsigned int m_numIndices;
			GLuint m_iboID = 0;
		};

		GlslProgram m_defaultShader;
		GlslProgram* m_currentShader = nullptr;

		bool m_inited = false;

		GLuint m_vaoID = 0, m_vboID = 0;
		std::vector<GLuint> m_iboIDs;

		GLuint m_totalVertices = 0, m_totalIndices = 0;

		//std::vector<Triangle> m_triangles;
		std::vector<Shape> m_shapes;
		std::vector<Shape*> m_shapePointers;
		std::vector<ShapeBatch> m_shapeBatches;

		void createVao();
		void setupShapeBatches();
		void addIndicesToBuffer(std::vector<GLuint>& indices, const int numIndices, 
			unsigned int& currentIndex, unsigned int& currentVertex);

		static bool compareByDepthIncremental(Shape* a, Shape* b);
		static bool compareByDepthDecremental(Shape* a, Shape* b);
	};
}