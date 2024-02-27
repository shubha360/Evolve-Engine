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

#include "GLSLProgram.h"
#include "TextureRenderer.h"
#include "RectDimension.h"
#include "UVDimension.h"
#include "ColorRGBA.h"
#include "InputProcessor.h"
#include "Camera.h"
#include "Font.h"

class GUIRenderer;

class GUI {
public:
	friend class GUIRenderer;

	GUI();
	~GUI();

	bool init(Font& font);

	// returns the font id in this GUI
	int addFont(Font& font);

	// returns the id of the component
	// pass 0 to fontId to use the default font
	int addTextButton(const std::string& label, const unsigned int fontId, float labelScale,
		const ColorRGBA& textColor, const ColorRGBA& buttonColor,
		const GlyphOrigin& renderOrigin, const RectDimension& dimension, std::function<void()> buttonFunction);

	// pass 0 to fontId to use the default font
	int addPlainText(const std::string& text, const unsigned int fontId, float scale,
		const ColorRGBA& color, const glm::ivec2& topLeftPosition);

	void setComponentLabel(const int id, const std::string& text);

	void updateGUI(InputProcessor& inputProcessor, Camera& camera);

	void freeGUI();

private:
	class Component {
	public:
		friend class GUI;
		friend class GUIRenderer;

		enum class ComponentType { NONE, BUTTON, PLAIN_TEXT };

		Component();
		virtual ~Component();

		void showComponent() { m_isVisible = true; };
		void hideComponent() { m_isVisible = false; };

	protected:
		std::string m_label = "";
		GlyphOrigin m_renderOrigin = GlyphOrigin::BOTTOM_LEFT;
		ComponentType m_type = ComponentType::NONE;
		RectDimension m_dimension = {};
		float m_labelScale = 0;
		ColorRGBA m_primaryColor = {};
		unsigned int m_fontId = 0;

		int m_centerX = 0, m_centerY = 0;
		int m_labelTopLeftX = 0, m_labelTopLeftY = 0;

		bool m_isFunctional = true;
		bool m_isVisible = true;
		bool m_labelCoordinatesFound = false;

		void findComponentCenter();
	};

	class Button : public Component {
	public:
		friend class GUI;
		friend class GUIRenderer;

		Button(const std::string& label, const unsigned int fontId, float labelScale,
			const ColorRGBA& textColor, const ColorRGBA& buttonColor, const GlyphOrigin& renderOrigin, 
			const RectDimension& dimension, std::function<void()> buttonFunction);

	private:
		ColorRGBA m_buttonColor;
		std::function<void()> m_buttonFunc;
	};
	
	class PlainText : public Component {
	public:
		friend class GUI;
		friend class GUIRenderer;

		PlainText(const std::string& text, const unsigned int fontId, float scale,
			const ColorRGBA& color, const RectDimension& position);
	};


	std::vector<std::unique_ptr<Component>> m_components;

	std::vector<Font*> m_fonts;

	SDL_Cursor* m_arrowCursor = nullptr;
	SDL_Cursor* m_indexPointerCursor = nullptr;

	SDL_Cursor* m_currentCursor = nullptr;

	bool isMouseInsideComponent(const glm::ivec2& mouseScreenCoords, Component& component);
};