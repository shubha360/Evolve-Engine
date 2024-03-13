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

#include "../include/Evolve/Gui.h"

Evolve::Gui::Gui() {}

Evolve::Gui::~Gui() {
	freeGui();
}

bool Evolve::Gui::init() {
	m_arrowCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
	m_indexPointerCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);

	m_currentCursor = m_arrowCursor;	

	return true;
}

int Evolve::Gui::addFont(Font& font) {
	
	if (!font.isInitialized()) {
		EVOLVE_REPORT_ERROR("Font adding to Gui is not initailized.", addFont);
		return -1;
	}

	m_fonts.push_back(&font);
	return (int) (m_fonts.size() - 1);
}

int Evolve::Gui::addTextButton(const std::string& label, const unsigned int fontId, float labelScale,
	const ColorRgba& textColor, const ColorRgba& buttonColor,
	const RectDimension& dimension, std::function<void()> buttonFunction) {

	if (fontId < 0 || fontId >= m_fonts.size()) {
		EVOLVE_REPORT_ERROR("Invalid font ID used.", addTextButton);
		return -1;
	}

	m_components.emplace_back(
		new Button(label, fontId, labelScale, textColor, buttonColor, dimension, buttonFunction)
	);
	
	return (int) (m_components.size() - 1);
}

int Evolve::Gui::addPlainText(const std::string& text, const unsigned int fontId, float scale,
	const ColorRgba& color, const glm::ivec2& topLeftPosition) {

	if (fontId < 0 || fontId >= m_fonts.size()) {
		EVOLVE_REPORT_ERROR("Invalid font ID used.", addPlainText);
		return -1;
	}

	m_components.emplace_back(
		new PlainText(text, fontId, scale, color, topLeftPosition)
	);

	return (int) (m_components.size() - 1);
}

int Evolve::Gui::addBlinkingText(const std::string& text, const unsigned int fontId, float scale, 
	const ColorRgba& color, const glm::ivec2& topLeftPosition, 
	const float onDuration /*= 30.0f*/, const float offDuration /*= 30.0f*/)
{
	if (fontId < 0 || fontId >= m_fonts.size()) {
		EVOLVE_REPORT_ERROR("Invalid font ID used.", addBlinkingText);
		return -1;
	}

	m_components.emplace_back(
		new BlinkingText(text, fontId, scale, color, topLeftPosition, onDuration, offDuration)
	);

	return (int)(m_components.size() - 1);
}

int Evolve::Gui::addPanel(const RectDimension& dimension, const ColorRgba& color) {
	
	m_components.emplace_back(
		new Panel(dimension, color)
	);

	return (int)(m_components.size() - 1);
}

void Evolve::Gui::setComponentLabel(const int id, const std::string& text) {

	if (id < 0 || id >= m_components.size()) {
		EVOLVE_REPORT_ERROR("Invalid component ID used.", setComponentLabel);
	}

	m_components[id]->m_label = text;
}

void Evolve::Gui::setComponentPosition(const int id, const glm::ivec2& position) {
	if (id < 0 || id >= m_components.size()) {
		EVOLVE_REPORT_ERROR("Invalid component ID used.", setComponentPosition);
	}

	auto dim = m_components[id]->m_dimension;

	dim.set(dim.getOrigin(), position.x, position.y, dim.getWidth(), dim.getHeight());
}

int Evolve::Gui::getLabelWidth(const int id) {
	if (id < 0 || id >= m_components.size()) {
		EVOLVE_REPORT_ERROR("Invalid component ID used.", getLabelWidth);
	}
	
	return m_fonts[m_components[id]->m_fontId]->getLineWidth(m_components[id]->m_label);
}

int Evolve::Gui::getLabelHeight(const int id) {
	if (id < 0 || id >= m_components.size()) {
		EVOLVE_REPORT_ERROR("Invalid component ID used.", getLabelHeight);
	}
	
	return m_fonts[m_components[id]->m_fontId]->getLineHeight();
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

void Evolve::Gui::updateTime(const float deltaTime) {
	for (auto& comp : m_components) {
		comp->m_time += deltaTime;
	}
}

void Evolve::Gui::showComponent(const int id) {

	if (id < 0 || id >= m_components.size()) {
		EVOLVE_REPORT_ERROR("Invalid component ID used.", showComponent);
	}

	m_components[id]->m_isVisible = true;
	m_components[id]->m_time = 0.0f;
}

void Evolve::Gui::hideComponent(const int id) {
	
	if (id < 0 || id >= m_components.size()) {
		EVOLVE_REPORT_ERROR("Invalid component ID used.", hideComponent);
	}

	m_components[id]->m_isVisible = false;
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

	int compLeft = component.m_dimension.getLeft();
	int compRight = component.m_dimension.getRight();

	int compBottom = component.m_dimension.getBottom();
	int compTop = component.m_dimension.getTop();

	if (
		(mouseScreenCoords.x >= compLeft && mouseScreenCoords.x <= compRight)
		&&
		(mouseScreenCoords.y >= compBottom && mouseScreenCoords.y <= compTop)
		) {
		return true;
	}

	return false;
}

Evolve::Gui::Component::Component() {}

Evolve::Gui::Component::~Component() {}

Evolve::Gui::Button::Button(const std::string& label, const unsigned int fontId, float labelScale,
	const ColorRgba& textColor, const ColorRgba& buttonColor, 
	const RectDimension& dimension, std::function<void()> buttonFunction) :
	 
	m_buttonColor(buttonColor),
	m_buttonFunc(buttonFunction)
{
	m_label = label;
	m_type = ComponentType::BUTTON;
	m_dimension = dimension;
	m_labelScale = labelScale;
	m_primaryColor = textColor;
	m_fontId = fontId;

	m_isFunctional = true;
	m_isVisible = true;

	m_centerX = m_dimension.getCenterX();
	m_centerY = m_dimension.getCenterY();
}

Evolve::Gui::PlainText::PlainText(const std::string& text, const unsigned int fontId, float scale,
	const ColorRgba& color, const glm::ivec2& position) {
	
	m_label = text;
	m_type = ComponentType::PLAIN_TEXT;
	m_fontId = fontId;
	m_labelScale = scale;
	m_primaryColor = color;
	
	m_dimension.set(Origin::TOP_LEFT, position.x, position.y, 0, 0);

	m_isFunctional = false;
	m_isVisible = true;
}

Evolve::Gui::BlinkingText::BlinkingText(const std::string& text, const unsigned int fontId, float scale, 
	const ColorRgba& color, const glm::ivec2& position, 
	const float onDuration, const float offDuration) :
	
	m_onDuration(onDuration), m_offDuration(offDuration)
{
	m_label = text;
	m_type = ComponentType::BLINKING_TEXT;
	m_fontId = fontId;
	m_labelScale = scale;
	m_primaryColor = color;
	
	m_dimension.set(Origin::TOP_LEFT, position.x, position.y, 0, 0);

	m_isFunctional = false;
	m_isVisible = true;
}

Evolve::Gui::Panel::Panel(const RectDimension& dimension, const ColorRgba& color) {
	m_type = ComponentType::PANEL;
	m_primaryColor = color;
	m_dimension = dimension;

	m_isFunctional = false;
	m_isVisible = true;
}