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

	if (!textureRenderer_.init(pathToAssets)) {
		EVOLVE_REPORT_ERROR("Failed to initialize gui font renderer.", init);
		return false;
	}

	ImageLoader::LoadTextureFromImage(pathToAssets + "/images/button_bg.png", buttonBgTexture_, 4);
	ImageLoader::BufferTextureData(buttonBgTexture_);
	ImageLoader::FreeTexture(buttonBgTexture_);

	ImageLoader::LoadTextureFromImage(pathToAssets + "/images/panel.png", panelTexture_, 4);
	ImageLoader::BufferTextureData(panelTexture_);
	ImageLoader::FreeTexture(panelTexture_);

	return true;
}

void Evolve::GuiRenderer::renderGui(Gui& gui, Camera& camera) {

	textureRenderer_.begin();

	static UvDimension uv { 0.0f, 0.0f, 1.0f, 1.0f };

	for (auto& comp : gui.components_) {

		if (comp->isVisible_) {
			
			// BUTTON
			if (comp->type_ == Gui::Component::ComponentType::BUTTON) {

				Gui::Button* button = (Gui::Button*)comp.get();
				Font* font = gui.fonts_[button->fontId_];

				textureRenderer_.draw(
					button->dimension_,
					uv,
					buttonBgTexture_.id,
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
				textureRenderer_.draw(
					comp->dimension_,
					uv,
					panelTexture_.id,
					comp->primaryColor_
				);
			}
		}
	}

	textureRenderer_.end(GlyphSortType::BY_TEXTURE_ID_DECREMENTAL);

	textureRenderer_.renderTextures(camera);
}

void Evolve::GuiRenderer::freeGuiRenderer() {
	textureRenderer_.freeTextureRenderer();

	ImageLoader::DeleteTexture(buttonBgTexture_);
	ImageLoader::DeleteTexture(panelTexture_);
}

void Evolve::GuiRenderer::getLabelCoordinates(int& X, int& y, const char* label,
	const int componentCenterX, const int componentCenterY, Font& font) {
	
	unsigned int labelWidth = font.getLineWidth(label);
	unsigned int labelHeight = font.getLineHeight();

	X = componentCenterX - labelWidth / 2;
	y = componentCenterY + labelHeight / 2;
}

