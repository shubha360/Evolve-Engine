#pragma once

#include "IncludeLibs.h"

#include "GlslProgram.h"
#include "Vertex2D.h"
#include "Camera.h"
#include "RectDimension.h"

namespace Evolve {

	enum class ShapeOrigin {
		BOTTOM_LEFT,
		BOTTOM_RIGHT,
		TOP_RIGHT,
		TOP_LEFT,
		CENTER
	};

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
		void begin(Camera& camera, GlslProgram* shader = nullptr);

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

		void drawRectangle(const ShapeOrigin origin, const RectDimension& destRect,
			ColorRgba& verticesColor, int depth = 0);

		void end(const ShapeSortType& sortType = ShapeSortType::BY_DEPTH_INCREMENTAL);

		void renderShapes();

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