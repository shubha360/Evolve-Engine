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

#include "../include/Evolve/GuiRenderer.h"

Evolve::GuiRenderer::GuiRenderer() {}

Evolve::GuiRenderer::~GuiRenderer() {
	freeGuiRenderer();
}

bool Evolve::GuiRenderer::init(const std::string& pathToAssets) {

	if (!shapeRenderer_.init(pathToAssets)) {
		EVOLVE_REPORT_ERROR("Failed to initialize gui shape renderer.", init);
		return false;
	}

	if (!textureRenderer_.init(pathToAssets)) {
		EVOLVE_REPORT_ERROR("Failed to initialize gui font renderer.", init);
		return false;
	}

	return true;
}

void Evolve::GuiRenderer::renderGui(Gui& gui, Camera& camera) {

	textureRenderer_.begin();
	shapeRenderer_.begin();	

	for (auto& comp : gui.components_) {

		if (comp->isVisible_) {
			
			// BUTTON
			if (comp->type_ == Gui::Component::ComponentType::BUTTON) {

				Gui::Button* button = (Gui::Button*)comp.get();
				Font* font = gui.fonts_[button->fontId_];

				shapeRenderer_.drawRectangle(
					button->dimension_,
					button->buttonColor_
				);

				font->setFontScale(button->labelScale_);

				if (!button->labelCoordinatesFound_) {
					getLabelCoordinates(button->labelTopLeftX_, button->labelTopLeftY_,
						button->label_, button->centerX_, button->centerY_, *font);

					button->labelCoordinatesFound_ = true;
				}

				font->drawTextToRenderer(button->label_, button->labelTopLeftX_,
					button->labelTopLeftY_, button->primaryColor_, textureRenderer_);
			}

			// PLAIN TEXT
			else if (comp->type_ == Gui::Component::ComponentType::PLAIN_TEXT) {

				Gui::PlainText* plainText = (Gui::PlainText*)comp.get();
				Font* font = gui.fonts_[plainText->fontId_];

				font->setFontScale(plainText->labelScale_);

				font->drawTextToRenderer(plainText->label_, plainText->dimension_.getLeft(),
					plainText->dimension_.getTop(), plainText->primaryColor_, textureRenderer_);
			}

			// BLINKING_TEXT
			else if (comp->type_ == Gui::Component::ComponentType::BLINKING_TEXT) {

				Gui::BlinkingText* blinkingText = (Gui::BlinkingText*)comp.get();

				if (blinkingText->time_ <= blinkingText->onDuration_) {

					Font* font = gui.fonts_[blinkingText->fontId_];

					font->setFontScale(blinkingText->labelScale_);

					font->drawTextToRenderer(blinkingText->label_, blinkingText->dimension_.getLeft(),
						blinkingText->dimension_.getTop(), blinkingText->primaryColor_, textureRenderer_);
				}
				else {
					if (blinkingText->time_ > blinkingText->onDuration_ + blinkingText->offDuration_) {
						blinkingText->time_ = 0.0f;
					}
				}
			}

			// PANEL
			else if (comp->type_ == Gui::Component::ComponentType::PANEL) {
				shapeRenderer_.drawRectangle(
					comp->dimension_,
					comp->primaryColor_
				);
			}
		}
	}

	textureRenderer_.end();
	shapeRenderer_.end();

	shapeRenderer_.renderShapes(camera);
	textureRenderer_.renderTextures(camera);
}

void Evolve::GuiRenderer::freeGuiRenderer() {
	shapeRenderer_.freeShapeRenderer();
	textureRenderer_.freeTextureRenderer();
}

void Evolve::GuiRenderer::getLabelCoordinates(int& X, int& y, const std::string& label,
	const int componentCenterX, const int componentCenterY, Font& font) {
	
	unsigned int labelWidth = font.getLineWidth(label);
	unsigned int labelHeight = font.getLineHeight();

	X = componentCenterX - labelWidth / 2;
	y = componentCenterY + labelHeight / 2;
}

