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
	arrowCursor_ = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
	indexPointerCursor_ = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);

	currentCursor_ = arrowCursor_;	

	components_.reserve(componentsMaxSize_);
	fonts_.reserve(fontsMaxSize_);

	return true;
}

size_t Evolve::Gui::addFont(Font& font) {

	if (!font.isInitialized()) {
		EVOLVE_REPORT_ERROR("Font adding to Gui is not initailized.", addFont);
		return -1;
	}

	fonts_.emplace_back(&font);
	size_t id = fonts_.size() - 1;

	if (fonts_.size() == fontsMaxSize_) {
		fontsMaxSize_ *= 2;
		fonts_.reserve(fontsMaxSize_);
	}
	return id;
}

size_t Evolve::Gui::addTextButton(const std::string& label, const size_t fontId, float labelScale,
	const ColorRgba& textColor, const ColorRgba& buttonColor,
	const RectDimension& dimension, std::function<void()> buttonFunction) {

	if (fontId < 0 || fontId >= fonts_.size()) {
		EVOLVE_REPORT_ERROR("Invalid font ID used.", addTextButton);
		return -1;
	}

	components_.emplace_back(
		std::make_unique<Button>(label, fontId, labelScale, textColor, buttonColor, dimension, buttonFunction)
	);
	
	size_t id = components_.size() - 1;

	if (components_.size() == componentsMaxSize_) {
		componentsMaxSize_ *= 2;
		components_.reserve(componentsMaxSize_);
	}

	return id;
}

size_t Evolve::Gui::addPlainText(const std::string& text, const size_t fontId, float scale,
	const ColorRgba& color, const Position2D& topLeftPosition) {

	if (fontId < 0 || fontId >= fonts_.size()) {
		EVOLVE_REPORT_ERROR("Invalid font ID used.", addPlainText);
		return -1;
	}

	components_.emplace_back(
		std::make_unique<PlainText>(text, fontId, scale, color, topLeftPosition)
	);

	size_t id = components_.size() - 1;

	if (components_.size() == componentsMaxSize_) {
		componentsMaxSize_ *= 2;
		components_.reserve(componentsMaxSize_);
	}

	return id;
}

size_t Evolve::Gui::addBlinkingText(const std::string& text, const size_t fontId, float scale,
	const ColorRgba& color, const Position2D& topLeftPosition,
	const float onDuration /*= 30.0f*/, const float offDuration /*= 30.0f*/)
{
	if (fontId < 0 || fontId >= fonts_.size()) {
		EVOLVE_REPORT_ERROR("Invalid font ID used.", addBlinkingText);
		return -1;
	}

	components_.emplace_back(
		std::make_unique<BlinkingText>(text, fontId, scale, color, topLeftPosition, onDuration, offDuration)
	);

	size_t id = components_.size() - 1;

	if (components_.size() == componentsMaxSize_) {
		componentsMaxSize_ *= 2;
		components_.reserve(componentsMaxSize_);
	}

	return id;
}

size_t Evolve::Gui::addPanel(const RectDimension& dimension, const ColorRgba& color) {
	
	components_.emplace_back(
		std::make_unique<Panel>(dimension, color)
	);

	size_t id = components_.size() - 1;

	if (components_.size() == componentsMaxSize_) {
		componentsMaxSize_ *= 2;
		components_.reserve(componentsMaxSize_);
	}

	return id;
}

void Evolve::Gui::setComponentLabel(const size_t id, const std::string& text) {

	if (id < 0 || id >= components_.size()) {
		EVOLVE_REPORT_ERROR("Invalid component ID used.", setComponentLabel);
	}

	components_[id]->label_ = text;
}

void Evolve::Gui::setComponentPosition(const size_t id, const Position2D& position) {
	if (id < 0 || id >= components_.size()) {
		EVOLVE_REPORT_ERROR("Invalid component ID used.", setComponentPosition);
	}

	auto& dim = components_[id]->dimension_;

	dim.set(dim.getOrigin(), position.X, position.Y, dim.getWidth(), dim.getHeight());
}

int Evolve::Gui::getLabelWidth(const size_t id) {
	if (id < 0 || id >= components_.size()) {
		EVOLVE_REPORT_ERROR("Invalid component ID used.", getLabelWidth);
	}
	
	return fonts_[components_[id]->fontId_]->getLineWidth(components_[id]->label_);
}

int Evolve::Gui::getLabelHeight(const size_t id) {
	if (id < 0 || id >= components_.size()) {
		EVOLVE_REPORT_ERROR("Invalid component ID used.", getLabelHeight);
	}
	
	return fonts_[components_[id]->fontId_]->getLineHeight();
}

void Evolve::Gui::updateGui(InputProcessor& inputProcessor, Camera& camera) {

	Position2D mouseCoords = camera.convertScreenCoordsToWorldCoords(inputProcessor.getMouseCoords());

	for (auto& comp : components_) {
		if (comp->isVisible_ && comp->isFunctional_) {
			
			if (isMouseInsideComponent(mouseCoords, *comp)) {

				// change cursor
				if (currentCursor_ != indexPointerCursor_) {
					currentCursor_ = indexPointerCursor_;
					SDL_SetCursor(currentCursor_);
				}

				// if mouse is clicked
				if (inputProcessor.isKeyDown(SDL_BUTTON_LEFT)) {
					switch (comp->type_) {
					case Component::ComponentType::BUTTON:

						Button* button = (Button*)comp.get();

						button->buttonFunc_();
					}
				}

				// do not check other components if cursor is inside this component
				break;
			}
			// not inside component, set normal cursor
			else {
				if (currentCursor_ != arrowCursor_) {
					currentCursor_ = arrowCursor_;
					SDL_SetCursor(currentCursor_);
				}
			}
		}
	}
}

void Evolve::Gui::updateTime(const float deltaTime) {
	for (auto& comp : components_) {
		comp->time_ += deltaTime;
	}
}

void Evolve::Gui::showComponent(const size_t id) {

	if (id < 0 || id >= components_.size()) {
		EVOLVE_REPORT_ERROR("Invalid component ID used.", showComponent);
	}

	components_[id]->isVisible_ = true;
	components_[id]->time_ = 0.0f;
}

void Evolve::Gui::hideComponent(const size_t id) {
	
	if (id < 0 || id >= components_.size()) {
		EVOLVE_REPORT_ERROR("Invalid component ID used.", hideComponent);
	}

	components_[id]->isVisible_ = false;
}

bool Evolve::Gui::isComponentVisible(const size_t id) {
	if (id < 0 || id >= components_.size()) {
		EVOLVE_REPORT_ERROR("Invalid component ID used.", hideComponent);
	}

	return components_[id]->isVisible_;
}

void Evolve::Gui::freeGui() {
	if (arrowCursor_ != nullptr) {
		SDL_FreeCursor(arrowCursor_);
		arrowCursor_ = nullptr;
	}

	if (indexPointerCursor_ != nullptr) {
		SDL_FreeCursor(indexPointerCursor_);
		indexPointerCursor_ = nullptr;
	}
}

bool Evolve::Gui::isMouseInsideComponent(const Position2D& mouseScreenCoords, Component& component) {

	int compLeft = component.dimension_.getLeft();
	int compRight = component.dimension_.getRight();

	int compBottom = component.dimension_.getBottom();
	int compTop = component.dimension_.getTop();

	if (
		(mouseScreenCoords.X >= compLeft && mouseScreenCoords.X <= compRight)
		&&
		(mouseScreenCoords.Y >= compBottom && mouseScreenCoords.Y <= compTop)
		) {
		return true;
	}

	return false;
}

Evolve::Gui::Component::Component() {}

Evolve::Gui::Component::~Component() {}

Evolve::Gui::Button::Button(const std::string& label, const size_t fontId, float labelScale,
	const ColorRgba& textColor, const ColorRgba& buttonColor, 
	const RectDimension& dimension, std::function<void()> buttonFunction) :
	 
	buttonColor_(buttonColor),
	buttonFunc_(buttonFunction)
{
	label_ = label;
	type_ = ComponentType::BUTTON;
	dimension_ = dimension;
	labelScale_ = labelScale;
	primaryColor_ = textColor;
	fontId_ = fontId;

	isFunctional_ = true;
	isVisible_ = true;

	centerX_ = dimension_.getCenterX();
	centerY_ = dimension_.getCenterY();
}

Evolve::Gui::PlainText::PlainText(const std::string& text, const size_t fontId, float scale,
	const ColorRgba& color, const Position2D& position) {
	
	label_ = text;
	type_ = ComponentType::PLAIN_TEXT;
	fontId_ = fontId;
	labelScale_ = scale;
	primaryColor_ = color;
	
	dimension_.set(Origin::TOP_LEFT, position.X, position.Y, 0, 0);

	isFunctional_ = false;
	isVisible_ = true;
}

Evolve::Gui::BlinkingText::BlinkingText(const std::string& text, const size_t fontId, float scale,
	const ColorRgba& color, const Position2D& position,
	const float onDuration, const float offDuration) :
	
	onDuration_(onDuration), offDuration_(offDuration)
{
	label_ = text;
	type_ = ComponentType::BLINKING_TEXT;
	fontId_ = fontId;
	labelScale_ = scale;
	primaryColor_ = color;
	
	dimension_.set(Origin::TOP_LEFT, position.X, position.Y, 0, 0);

	isFunctional_ = false;
	isVisible_ = true;
}

Evolve::Gui::Panel::Panel(const RectDimension& dimension, const ColorRgba& color) {
	type_ = ComponentType::PANEL;
	primaryColor_ = color;
	dimension_ = dimension;

	isFunctional_ = false;
	isVisible_ = true;
}