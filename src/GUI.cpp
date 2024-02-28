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

#include "../include/Gui.h"

Evolve::Gui::Gui() {}

Evolve::Gui::~Gui() {
	freeGui();
}

bool Evolve::Gui::init(Font& font) {

	if (!font.isInitialized()) {
		EVOLVE_REPORT_ERROR("The font being added to Gui is not initailized.", init);
		return false;
	}

	addFont(font);

	m_arrowCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
	m_indexPointerCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);

	m_currentCursor = m_arrowCursor;	

	return true;
}

int Evolve::Gui::addFont(Font& font) {
	
	if (!font.isInitialized()) {
		EVOLVE_REPORT_ERROR("Font adding to Gui is not initailized.", init);
		return -1;
	}

	m_fonts.push_back(&font);
	return (int) (m_fonts.size() - 1);
}

int Evolve::Gui::addTextButton(const std::string& label, const unsigned int fontId, float labelScale,
	const ColorRgba& textColor, const ColorRgba& buttonColor,
	const GlyphOrigin& renderOrigin, const RectDimension& dimension, std::function<void()> buttonFunction) {

	if (fontId < 0 || fontId >= m_fonts.size()) {
		EVOLVE_REPORT_ERROR("Invalid font ID used.", addTextButton);
		return -1;
	}

	m_components.push_back(std::unique_ptr<Component>(
		new Button(label, fontId, labelScale, textColor, buttonColor, renderOrigin, dimension, buttonFunction)
	));
	
	return (int) (m_components.size() - 1);
}

int Evolve::Gui::addPlainText(const std::string& text, const unsigned int fontId, float scale,
	const ColorRgba& color, const glm::ivec2& topLeftPosition) {

	if (fontId < 0 || fontId >= m_fonts.size()) {
		EVOLVE_REPORT_ERROR("Invalid font ID used.", addTextButton);
		return -1;
	}

	m_components.push_back(std::unique_ptr<Component>(
		new PlainText(text, fontId, scale, color, RectDimension{ topLeftPosition.x, topLeftPosition.y, 0, 0 })
	));

	return (int) (m_components.size() - 1);
}

void Evolve::Gui::setComponentLabel(const int id, const std::string& text) {

	if (id < 0 || id >= m_components.size()) {
		EVOLVE_REPORT_ERROR("Invalid component ID used.", addTextButton);
	}

	m_components[id]->m_label = text;
}

void Evolve::Gui::updateGui(InputProcessor& inputProcessor, Camera& camera) {

	glm::ivec2 mouseCoords = camera.convertScreenCoordsToWorldCoords(inputProcessor.getMouseCoords());

	for (auto& comp : m_components) {
		if (comp->m_isVisible) {
			if (isMouseInsideComponent(mouseCoords, *comp)) {

				// if component is functional, change the cursor to index pointer
				if (comp->m_isFunctional) {
					if (m_currentCursor != m_indexPointerCursor) {
						m_currentCursor = m_indexPointerCursor;
						SDL_SetCursor(m_currentCursor);
					}
					
					// if mouse is clicked
					if (inputProcessor.isKeyDown(SDL_BUTTON_LEFT)) {
						switch (comp->m_type) {
						case Component::ComponentType::BUTTON:

							Button* button = (Button*)comp.get();

							button->m_buttonFunc();
						}
					}					
				}
				// is not functional, set normal cursor
				else {
					if (m_currentCursor != m_arrowCursor) {
						m_currentCursor = m_arrowCursor;
						SDL_SetCursor(m_currentCursor);
						break;
					}
				}
				// do not check other components if cursor is inside this component
				break;
			}
			// not inside component, set normal cursor
			else {
				if (m_currentCursor != m_arrowCursor) {
					m_currentCursor = m_arrowCursor;
					SDL_SetCursor(m_currentCursor);
				}
			}
		}
	}
}

void Evolve::Gui::freeGui() {
	if (m_arrowCursor != nullptr) {
		SDL_FreeCursor(m_arrowCursor);
		m_arrowCursor = nullptr;
	}

	if (m_indexPointerCursor != nullptr) {
		SDL_FreeCursor(m_indexPointerCursor);
		m_indexPointerCursor = nullptr;
	}
}

bool Evolve::Gui::isMouseInsideComponent(const glm::ivec2& mouseScreenCoords, Component& component) {

	int compWidth = component.m_dimension.width;
	int compHeight = component.m_dimension.height;

	int compBottomLeftX = component.m_centerX - compWidth / 2;
	int compBottomLeftY = component.m_centerY - compHeight / 2;

	if (
		(mouseScreenCoords.x >= compBottomLeftX && mouseScreenCoords.x <= compBottomLeftX + compWidth)
		&&
		(mouseScreenCoords.y >= compBottomLeftY && mouseScreenCoords.y <= compBottomLeftY + compHeight)
		) {
		return true;
	}

	return false;
}

Evolve::Gui::Component::Component() {}

Evolve::Gui::Component::~Component() {}

void Evolve::Gui::Component::findComponentCenter() {

	switch (m_renderOrigin) {
	case GlyphOrigin::BOTTOM_LEFT:

		m_centerX = m_dimension.x + (m_dimension.width / 2);
		m_centerY = m_dimension.y + (m_dimension.height / 2);
		break;

	case GlyphOrigin::BOTTOM_RIGHT:
		m_centerX = m_dimension.x - (m_dimension.width / 2);
		m_centerY = m_dimension.y + (m_dimension.height / 2);
		break;

	case GlyphOrigin::TOP_RIGHT:
		m_centerX = m_dimension.x - (m_dimension.width / 2);
		m_centerY = m_dimension.y - (m_dimension.height / 2);
		break;

	case GlyphOrigin::TOP_LEFT:
		m_centerX = m_dimension.x + (m_dimension.width / 2);
		m_centerY = m_dimension.y - (m_dimension.height / 2);
		break;

	case GlyphOrigin::CENTER:
		m_centerX = m_dimension.x;
		m_centerY = m_dimension.y;
		break;
	}
}

Evolve::Gui::Button::Button(const std::string& label, const unsigned int fontId, float labelScale,
	const ColorRgba& textColor, const ColorRgba& buttonColor, const GlyphOrigin& renderOrigin, 
	const RectDimension& dimension, std::function<void()> buttonFunction) :
	 
	m_buttonColor(buttonColor),
	m_buttonFunc(buttonFunction)
{
	m_label = label;
	m_type = ComponentType::BUTTON;
	m_renderOrigin = renderOrigin;
	m_dimension = dimension;
	m_labelScale = labelScale;
	m_primaryColor = textColor;
	m_fontId = fontId;

	m_isFunctional = true;
	m_isVisible = true;

	findComponentCenter();
}

Evolve::Gui::PlainText::PlainText(const std::string& text, const unsigned int fontId, float scale,
	const ColorRgba& color, const RectDimension& position) {
	
	m_label = text;
	m_type = ComponentType::PLAIN_TEXT;
	m_fontId = fontId;
	m_labelScale = scale;
	m_primaryColor = color;
	m_dimension = position;

	m_isFunctional = false;
	m_isVisible = true;
}
