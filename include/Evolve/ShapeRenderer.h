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

/* DISCLAIMER: This class is heavily inefficient. Use TextureRenderer instead. */

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

		void drawTriangle(const Position2D& originPos, const Position2D& vertexTwoPos, const Position2D& vertexThreePos,
			const ColorRgba& verticesColor, const int depth = 0);

		void drawTriangle(const Position2D& originPos, const ColorRgba& originColor,
			const Position2D& vertexTwoPos, const ColorRgba& vertexTwoColor,
			const Position2D& vertexThreePos, const ColorRgba& vertexThreeColor,
			int depth = 0);

		void drawRectangle(const Position2D& originPos, const Position2D& vertexTwoPos,
			const Position2D& vertexThreePos, const Position2D& vertexFourPos,
			const ColorRgba& verticesColor, int depth = 0);

		void drawRectangle(const Position2D& originPos, const ColorRgba& originColor,
			const Position2D& vertexTwoPos, const ColorRgba& vertexTwoColor,
			const Position2D& vertexThreePos, const ColorRgba& vertexThreeColor,
			const Position2D& vertexFourPos, const ColorRgba& vertexFourColor,
			int depth = 0);

		void drawRectangle(const RectDimension& destRect,
			const ColorRgba& verticesColor, int depth = 0);

		// this is very inefficient, it's better to use a circular texture using TextureRenderer
		void drawCircle(const Position2D& centerPos, unsigned int radius, const ColorRgba& color, int depth = 0);

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
			int depth_ = 0;

			std::vector<Vertex2D> vertices_;

			unsigned int numVertices_ = 0, numIndices_ = 0;
		};

		class ShapeBatch {
		public:
			friend class ShapeRenderer;
			
			ShapeBatch(unsigned int offset, unsigned int numIndices);

		private:
			unsigned int offset_;
			unsigned int numIndices_;
			GLuint iboID_ = 0;
		};

		GlslProgram defaultShader_;
		GlslProgram* currentShader_ = nullptr;

		bool inited_ = false;

		GLuint vaoID_ = 0, vboID_ = 0;
		std::vector<GLuint> iboIDs_;

		GLuint totalVertices_ = 0, totalIndices_ = 0;

		//std::vector<Triangle> m_triangles;
		std::vector<Shape> shapes_;
		std::vector<Shape*> shapePointers_;
		std::vector<ShapeBatch> shapeBatches_;

		void createVao();
		void setupShapeBatches();
		void addIndicesToBuffer(std::vector<GLuint>& indices, const int numIndices, 
			unsigned int& currentIndex, unsigned int& currentVertex);

		static bool compareByDepthIncremental(Shape* a, Shape* b);
		static bool compareByDepthDecremental(Shape* a, Shape* b);
	};
}