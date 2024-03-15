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
		size_t addTextButton(const std::string& label, const size_t fontId, float labelScale,
			const ColorRgba& textColor, const ColorRgba& buttonColor,
			const RectDimension& dimension, std::function<void()> buttonFunction);

		// returns the id of the component
		// pass 0 to fontId to use the default font
		size_t addPlainText(const std::string& text, const size_t fontId, float scale,
			const ColorRgba& color, const glm::ivec2& topLeftPosition);

		// returns the id of the component
		// pass 0 to fontId to use the default font
		size_t addBlinkingText(const std::string& text, const size_t fontId, float scale,
			const ColorRgba& color, const glm::ivec2& topLeftPosition, 
			const float onDuration = 30.0f, const float offDuration = 30.0f);

		// returns the id of the component
		// pass 0 to fontId to use the default font
		size_t addPanel(const RectDimension& dimension, const ColorRgba& color);

		void setComponentLabel(const size_t id, const std::string& text);
		void setComponentPosition(const size_t id, const glm::ivec2& position);

		int getLabelWidth(const size_t id);
		int getLabelHeight(const size_t id);

		void updateGui(InputProcessor& inputProcessor, Camera& camera);
		void updateTime(const float deltaTime);

		void showComponent(const size_t id);
		void hideComponent(const size_t id);

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
			std::string m_label = "";
			ComponentType m_type = ComponentType::NONE;
			RectDimension m_dimension;
			float m_labelScale = 0;
			ColorRgba m_primaryColor = {};
			size_t m_fontId = 0;

			int m_centerX = 0, m_centerY = 0;
			int m_labelTopLeftX = 0, m_labelTopLeftY = 0;

			float m_time = 0.0f;

			bool m_isFunctional = true;
			bool m_isVisible = true;
			bool m_labelCoordinatesFound = false;
		};

		class Button : public Component {
		public:
			friend class Gui;
			friend class GuiRenderer;

			Button(const std::string& label, const size_t fontId, float labelScale,
				const ColorRgba& textColor, const ColorRgba& buttonColor,
				const RectDimension& dimension, std::function<void()> buttonFunction);

		private:
			ColorRgba m_buttonColor;
			std::function<void()> m_buttonFunc;
		};

		class PlainText : public Component {
		public:
			friend class Gui;
			friend class GuiRenderer;

			PlainText(const std::string& text, const size_t fontId, float scale,
				const ColorRgba& color, const glm::ivec2& position);
		};

		class BlinkingText : public Component {
		public:
			friend class Gui;
			friend class GuiRenderer;

			BlinkingText(const std::string& text, const size_t fontId, float scale,
				const ColorRgba& color, const glm::ivec2& position, 
				const float onDuration, const float offDuration);

		private:
			float m_onDuration = 0.0f, m_offDuration = 0.0f;
		};

		class Panel : public Component {
		public:
			friend class Gui;
			friend class GuiRenderer;

			Panel(const RectDimension& dimension, const ColorRgba& color);
		};

		std::vector<std::unique_ptr<Component>> m_components;

		std::vector<Font*> m_fonts;

		SDL_Cursor* m_arrowCursor = nullptr;
		SDL_Cursor* m_indexPointerCursor = nullptr;

		SDL_Cursor* m_currentCursor = nullptr;

		bool isMouseInsideComponent(const glm::ivec2& mouseScreenCoords, Component& component);
	};
}