#include "../include/GUI.h"

GUI::GUI() {}

GUI::~GUI() {
	freeGUI();
}

bool GUI::init(Font& font) {

	if (!font.isInitialized()) {
		REPORT_ERROR("The font being added to GUI is not initailized.", init);
		return false;
	}

	addFont(font);

	m_arrowCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
	m_indexPointerCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);

	m_currentCursor = m_arrowCursor;	

	return true;
}

int GUI::addFont(Font& font) {
	
	if (!font.isInitialized()) {
		REPORT_ERROR("Font adding to GUI is not initailized.", init);
		return -1;
	}

	m_fonts.push_back(&font);
	return (int) (m_fonts.size() - 1);
}

int GUI::addTextButton(const std::string& label, const unsigned int fontId, float labelScale,
	const ColorRGBA& textColor, const ColorRGBA& buttonColor,
	const GlyphOrigin& renderOrigin, const RectDimension& dimension, std::function<void()> buttonFunction) {

	if (fontId < 0 || fontId >= m_fonts.size()) {
		REPORT_ERROR("Invalid font ID used.", addTextButton);
		return -1;
	}

	m_components.push_back(std::unique_ptr<Component>(
		new Button(label, fontId, labelScale, textColor, buttonColor, renderOrigin, dimension, buttonFunction)
	));
	
	return (int) (m_components.size() - 1);
}

int GUI::addPlainText(const std::string& text, const unsigned int fontId, float scale,
	const ColorRGBA& color, const glm::ivec2& topLeftPosition) {

	if (fontId < 0 || fontId >= m_fonts.size()) {
		REPORT_ERROR("Invalid font ID used.", addTextButton);
		return -1;
	}

	m_components.push_back(std::unique_ptr<Component>(
		new PlainText(text, fontId, scale, color, RectDimension{ topLeftPosition.x, topLeftPosition.y, 0, 0 })
	));

	return (int) (m_components.size() - 1);
}

void GUI::setComponentLabel(const int id, const std::string& text) {

	if (id < 0 || id >= m_components.size()) {
		REPORT_ERROR("Invalid component ID used.", addTextButton);
	}

	m_components[id]->m_label = text;
}

void GUI::updateGUI(InputProcessor& inputProcessor, Camera& camera) {

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

void GUI::freeGUI() {
	if (m_arrowCursor != nullptr) {
		SDL_FreeCursor(m_arrowCursor);
		m_arrowCursor = nullptr;
	}

	if (m_indexPointerCursor != nullptr) {
		SDL_FreeCursor(m_indexPointerCursor);
		m_indexPointerCursor = nullptr;
	}
}

bool GUI::isMouseInsideComponent(const glm::ivec2& mouseScreenCoords, Component& component) {

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

GUI::Component::Component() {}

GUI::Component::~Component() {}

void GUI::Component::findComponentCenter() {

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

GUI::Button::Button(const std::string& label, const unsigned int fontId, float labelScale,
	const ColorRGBA& textColor, const ColorRGBA& buttonColor, const GlyphOrigin& renderOrigin, 
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

GUI::PlainText::PlainText(const std::string& text, const unsigned int fontId, float scale, 
	const ColorRGBA& color, const RectDimension& position) {
	
	m_label = text;
	m_type = ComponentType::PLAIN_TEXT;
	m_fontId = fontId;
	m_labelScale = scale;
	m_primaryColor = color;
	m_dimension = position;

	m_isFunctional = false;
	m_isVisible = true;
}
