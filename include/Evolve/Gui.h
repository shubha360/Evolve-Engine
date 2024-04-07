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
#include "TextureRenderer.h"
#include "RectDimension.h"
#include "UvDimension.h"
#include "ColorRgba.h"
#include "InputProcessor.h"
#include "Camera.h"
#include "Font.h"

namespace Evolve {

	class GuiRenderer;

	class Gui {
	public:
		friend class GuiRenderer;

		Gui();
		~Gui();

		bool init();

		// returns the font id in this Gui
		size_t addFont(Font& font);

		// returns the id of the component
		// pass 0 to fontId to use the default font
		size_t addTextButton(const char* label, const size_t fontId, float labelScale,
			const ColorRgba& textColor, const ColorRgba& buttonColor,
			const RectDimension& dimension, std::function<void()> buttonFunction);

		// returns the id of the component
		// pass 0 to fontId to use the default font
		size_t addPlainText(const char* text, const size_t fontId, float scale,
			const ColorRgba& color, const Position2D& topLeftPosition);

		// returns the id of the component
		// pass 0 to fontId to use the default font
		size_t addBlinkingText(const char* text, const size_t fontId, float scale,
			const ColorRgba& color, const Position2D& topLeftPosition,
			const float onDuration = 30.0f, const float offDuration = 30.0f);

		// returns the id of the component
		// pass 0 to fontId to use the default font
		size_t addPanel(const RectDimension& dimension, const ColorRgba& color);

		void setComponentLabel(const size_t id, const char* text);
		void setComponentPosition(const size_t id, const Position2D& position);

		int getLabelWidth(const size_t id);
		int getLabelHeight(const size_t id);

		void updateGui(InputProcessor& inputProcessor, Camera& camera);
		void updateTime(const float deltaTime);

		void showComponent(const size_t id);
		void hideComponent(const size_t id);

		bool isComponentVisible(const size_t id);

		void freeGui();

	private:
		class Component {
		public:
			friend class Gui;
			friend class GuiRenderer;

			enum class ComponentType { 
				NONE,
				BUTTON,
				PLAIN_TEXT,
				BLINKING_TEXT,
				PANEL
			};

			Component();
			virtual ~Component();

		protected:
			const char* label_ = nullptr;
			ComponentType type_ = ComponentType::NONE;
			RectDimension dimension_;
			float labelScale_ = 0;
			ColorRgba primaryColor_ = {};
			size_t fontId_ = 0;

			int centerX_ = 0, centerY_ = 0;
			int labelTopLeftX_ = 0, labelTopLeftY_ = 0;

			float time_ = 0.0f;

			bool isFunctional_ = true;
			bool isVisible_ = true;
			bool labelCoordinatesFound_ = false;
		};

		class Button : public Component {
		public:
			friend class Gui;
			friend class GuiRenderer;

			Button(const char* label, const size_t fontId, float labelScale,
				const ColorRgba& textColor, const ColorRgba& buttonColor,
				const RectDimension& dimension, std::function<void()> buttonFunction);

		private:
			ColorRgba buttonColor_;
			std::function<void()> buttonFunc_;
		};

		class PlainText : public Component {
		public:
			friend class Gui;
			friend class GuiRenderer;

			PlainText(const char* text, const size_t fontId, float scale,
				const ColorRgba& color, const Position2D& position);
		};

		class BlinkingText : public Component {
		public:
			friend class Gui;
			friend class GuiRenderer;

			BlinkingText(const char* text, const size_t fontId, float scale,
				const ColorRgba& color, const Position2D& position,
				const float onDuration, const float offDuration);

		private:
			float onDuration_ = 0.0f, offDuration_ = 0.0f;
		};

		class Panel : public Component {
		public:
			friend class Gui;
			friend class GuiRenderer;

			Panel(const RectDimension& dimension, const ColorRgba& color);
		};

		std::vector<std::unique_ptr<Component>> components_;
		size_t componentsMaxSize_ = 32;

		std::vector<Font*> fonts_;
		size_t fontsMaxSize_ = 8;

		SDL_Cursor* arrowCursor_ = nullptr;
		SDL_Cursor* indexPointerCursor_ = nullptr;

		SDL_Cursor* currentCursor_ = nullptr;

		bool isMouseInsideComponent(const Position2D& mouseScreenCoords, Component& component);
	};
}