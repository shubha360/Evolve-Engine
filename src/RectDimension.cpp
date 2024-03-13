#include "../include/Evolve/RectDimension.h"

Evolve::RectDimension::RectDimension() {}

Evolve::RectDimension::RectDimension(Origin origin, int x, int y, unsigned int width, unsigned int height) {
	set(origin, x, y, width, height);
}

Evolve::RectDimension::~RectDimension() {}

void Evolve::RectDimension::set(Origin origin, int x, int y, unsigned int width, unsigned int height) {
	m_origin = origin;
	m_left = x;
	m_bottom = y;
	m_width = width;
	m_height = height;

	findBottomLeft(origin);
}

void Evolve::RectDimension::findBottomLeft(Origin origin) {
	switch (origin) {
	case Origin::BOTTOM_LEFT:
		m_left = m_left;
		m_bottom = m_bottom;
		break;

	case Origin::BOTTOM_RIGHT:
		m_left = m_left - m_width;
		m_bottom = m_bottom;
		break;

	case Origin::TOP_RIGHT:
		m_left = m_left - m_width;
		m_bottom = m_bottom - m_height;
		break;

	case Origin::TOP_LEFT:
		m_left = m_left;
		m_bottom = m_bottom - m_height;
		break;

	case Origin::CENTER:
		m_left = m_left - m_width / 2;
		m_bottom = m_bottom - m_height / 2;
		break;
	}
}


