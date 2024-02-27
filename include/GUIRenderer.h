#pragma once

#include "GLSLProgram.h"
#include "Font.h"
#include "ErrorReporter.h"
#include "TextureRenderer.h"
#include "ImageLoader.h"
#include "Camera.h"
#include "GUI.h"

class GUIRenderer {
public:
	GUIRenderer();
	~GUIRenderer();

	bool init();

	void renderGUI(GUI& gui, Camera& camera);

	void freeGUIRenderer();

private:
	GLSLProgram m_glslProgram;
	TextureRenderer m_renderer;
	//Font m_font;

	TextureData m_roundedRectButtonTexture;

	void getLabelCoordinates(int& x, int& y, const std::string& label,
		const int componentCenterX, const int componentCenterY, Font& font);
};