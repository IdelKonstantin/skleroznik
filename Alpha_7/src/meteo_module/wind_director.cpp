#include "../../inc/wind_director.h"


windDirector::windDirector() : m_windHour(m_hoursToDegs[0].first), 
m_windAngle(m_hoursToDegs[0].second), 
m_windIndexMax(m_hoursToDegs.size() - 1) {}

void windDirector::updateWindDirection() {

	auto windDirInfo = m_hoursToDegs.at(m_windDirIndex);
	m_windHour = windDirInfo.first;
	m_windAngle = windDirInfo.second;
}

void windDirector::drawClockLine(uint16_t color) {
	tft.drawLine(m_hourCenterX, m_hourCenterY, m_hourX, m_hourY, color);
	tft.fillCircle(203, 57, 4, ILI9341_WHITE);
}

void windDirector::drawClock() {
  
	tft.setTextSize(3);
	tft.setCursor(147, 31);
	tft.println(F("W"));
	tft.setCursor(147, 59);
	tft.println(F("D"));

	tft.drawRoundRect(167, 21, 73, 73, 3, ILI9341_WHITE);  

	tft.drawFastVLine(185, 21, 6, ILI9341_WHITE);
	tft.drawFastVLine(222, 21, 6, ILI9341_WHITE);
	tft.drawFastVLine(185, 88, 6, ILI9341_WHITE);
	tft.drawFastVLine(222, 88, 6, ILI9341_WHITE);

	tft.drawFastHLine(167, 39, 6, ILI9341_WHITE);
	tft.drawFastHLine(167, 76, 6, ILI9341_WHITE);
	tft.drawFastHLine(234, 39, 6, ILI9341_WHITE);
	tft.drawFastHLine(234, 76, 6, ILI9341_WHITE);
	
	tft.setTextSize(1);
	tft.setCursor(199, 23);
	tft.println(F("12"));

	tft.setCursor(231, 53);
	tft.println(F("3"));

	tft.setCursor(201, 83);
	tft.println(F("6"));

	tft.setCursor(170, 53);
	tft.println(F("9"));

	tft.setTextSize(2);
}

void windDirector::drawHourLine() {

	m_hourX = m_hourCenterX + m_hourLineLength * sin(m_windAngle * DEGS_TO_RADS);
	m_hourY = m_hourCenterY - m_hourLineLength * cos(m_windAngle * DEGS_TO_RADS);

	drawClockLine(ILI9341_WHITE);
}

void windDirector::increaseHour() {

	m_windDirIndex += 1;
	m_windDirIndex = m_windDirIndex > m_windIndexMax ? 0 : m_windDirIndex;		
	updateWindDirection();		
}

void windDirector::decreaseHour() {

	m_windDirIndex -= 1;
	m_windDirIndex = m_windDirIndex < 0 ? m_windIndexMax : m_windDirIndex;
	updateWindDirection();
}

void windDirector::increaseHourAndDrawLine() {

	drawClockLine(ILI9341_BLACK);
	increaseHour();
}

void windDirector::decreaseHourAndDrawLine() {
	
	drawClockLine(ILI9341_BLACK);
	decreaseHour();
}

float windDirector::getWindHour() const {

	return m_windHour;
}

uint16_t windDirector::getWindAngle() const {

	return m_windAngle;
}