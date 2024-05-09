#include "../../inc/marker_director.h"

markerDirector::markerDirector() {};

void markerDirector::eraseMarker() const {
	
	tft.setTextColor(ILI9341_BLACK, ILI9341_BLACK);
	tft.setCursor(m_rightOffset, m_topMargin + m_markerIndex * m_fontVSize);
	tft.println(F("<"));
	tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
}

void markerDirector::drawMarker() const {

	tft.setCursor(m_rightOffset, m_topMargin + m_markerIndex * m_fontVSize);
	tft.println(F("<"));		
}

void markerDirector::setMarkerData(int16_t index, uint16_t maxLines, uint16_t fontVSize, uint16_t rightOffset, uint16_t topMargin) {

	m_markerIndex = index;
	m_maxLines = maxLines;
	m_fontVSize = fontVSize;
	m_rightOffset = rightOffset;
	m_topMargin = topMargin;
	drawMarker();
}

void markerDirector::downMarker() {

	eraseMarker();
	m_markerIndex += 1;
	m_markerIndex = m_markerIndex > (m_maxLines - 1) ? 0 : m_markerIndex;
	drawMarker();
}

void markerDirector::upMarker() {

	eraseMarker();
	m_markerIndex -= 1;
	m_markerIndex = m_markerIndex < 0 ? (m_maxLines - 1) : m_markerIndex;
	drawMarker();
}

uint16_t markerDirector::getMarkerIndex() const {
	return m_markerIndex;
}

void markerDirector::setMarkerIndex(int16_t index) {
	m_markerIndex = index;
}

void markerDirector::setMarkerMaxLines(uint16_t maxLines) {
	m_maxLines = maxLines;
}