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

#include "../include/GUIRenderer.h"

GUIRenderer::GUIRenderer() {}

GUIRenderer::~GUIRenderer() {
	freeGUIRenderer();
}

bool GUIRenderer::init(const std::string& pathToAssets) {

	std::string vertShaderPath = pathToAssets + "/gui/shaders/gui_shader.vert";
	std::string fragShaderPath = pathToAssets + "/gui/shaders/gui_shader.frag";
	
	std::string buttonImagePath = pathToAssets + "/gui/images/rounded_rect_button512x256.png";

	if (!m_glslProgram.compileAndLinkShaders(
		vertShaderPath,
		fragShaderPath)) {
		REPORT_ERROR("Failed to compile or link GUI shader.", init);
		return false;
	}

	ImageLoader::LoadTextureFromImage(
		buttonImagePath, m_roundedRectButtonTexture, 4
	);
	ImageLoader::BufferTextureData(m_roundedRectButtonTexture);
	ImageLoader::FreeTexture(m_roundedRectButtonTexture);

	return true;
}

void GUIRenderer::renderGUI(GUI& gui, Camera& camera) {
	
	m_glslProgram.useProgram();

	glActiveTexture(GL_TEXTURE0);
	GLint samplerLoc = m_glslProgram.getUniformLocation("u_imageSampler");
	glUniform1i(samplerLoc, 0);

	camera.sendMatrixDataToShader(m_glslProgram);

	m_renderer.begin();

	for (auto& comp : gui.m_components) {

		if (comp->m_isVisible) {
			
			if (comp->m_type == GUI::Component::ComponentType::BUTTON) {

				GUI::Button* button = (GUI::Button*)comp.get();
				Font* font = gui.m_fonts[button->m_fontId];

				m_renderer.draw(
					button->m_renderOrigin,
					button->m_dimension,
					UVDimension{ 0.0f, 0.0f, 1.0f, 1.0f },
					m_roundedRectButtonTexture.id,
					button->m_buttonColor
				);

				font->setFontScale(button->m_labelScale);

				if (!button->m_labelCoordinatesFound) {
					getLabelCoordinates(button->m_labelTopLeftX, button->m_labelTopLeftY,
						button->m_label, button->m_centerX, button->m_centerY, *font);

					button->m_labelCoordinatesFound = true;
				}

				font->drawTextToRenderer(button->m_label, button->m_labelTopLeftX,
					button->m_labelTopLeftY, button->m_primaryColor, m_renderer);
			}

			else if (comp->m_type == GUI::Component::ComponentType::PLAIN_TEXT) {

				GUI::PlainText* plainText = (GUI::PlainText*)comp.get();
				Font* font = gui.m_fonts[plainText->m_fontId];

				font->setFontScale(plainText->m_labelScale);

				font->drawTextToRenderer(plainText->m_label, plainText->m_dimension.x,
					plainText->m_dimension.y, plainText->m_primaryColor, m_renderer);
			}
		}
	}

	m_renderer.end(GlyphSortType::BY_TEXTURE_ID_DECREMENTAL);

	m_renderer.renderTextures();
}

void GUIRenderer::freeGUIRenderer() {
	ImageLoader::DeleteTexture(m_roundedRectButtonTexture);
	//m_font.deleteFont();

	m_renderer.freeTextureRenderer();
	m_glslProgram.freeProgram();
}

void GUIRenderer::getLabelCoordinates(int& x, int& y, const std::string& label, 
	const int componentCenterX, const int componentCenterY, Font& font) {
	
	unsigned int labelWidth = font.getLineWidth(label);
	unsigned int labelHeight = font.getLineHeight();

	x = componentCenterX - labelWidth / 2;
	y = componentCenterY + labelHeight / 2;
}

