#ifndef _BC_WIND_DIRECTOR_H_
#define _BC_WIND_DIRECTOR_H_

#include <vector>
#include <cmath>
#include "TFT_worker.h"

#define DEGS_TO_RADS 0.017453

extern TFT_UNIT tft;

class windDirector {

private:
	
	const std::vector<std::pair<float, uint16_t>> m_hoursToDegs {
		{12, 0},
		{1, 30},
		{1.5, 45},
		{2, 60},
		{3, 90},
		{4, 120},
		{4.5, 135},
		{5, 150},
		{6, 180},
		{7, 210},
		{7.5, 225},
		{8, 240},
		{9, 270},
		{10, 300},
		{10.5, 315},
		{11, 330}
	};

	int16_t m_windDirIndex{0};
	float m_windHour;
	uint16_t m_windAngle;

	uint16_t m_hourX, m_hourY;

	const uint16_t m_hourLineLength{30};
	const uint16_t m_hourCenterX{203};
	const uint16_t m_hourCenterY{57};

	void updateWindDirection() {

		auto windDirInfo = m_hoursToDegs.at(m_windDirIndex);
		m_windHour = windDirInfo.first;
		m_windAngle = windDirInfo.second;
	}

public:
	windDirector() : m_windHour(m_hoursToDegs[0].first), m_windAngle(m_hoursToDegs[0].second) {}

	void drawClock() {

		tft.drawRoundRect(167, 21, 73, 73, 3, ILI9341_WHITE);
		tft.fillCircle(203, 57, 4, ILI9341_WHITE);    
		tft.setTextSize(3);
		tft.setCursor(147, 31);
		tft.println(F("W"));
		tft.setCursor(147, 59);
		tft.println(F("D"));
		tft.setTextSize(2);
	}

	void drawHourLine() {

		m_hourX = m_hourCenterX + m_hourLineLength * sin(m_windAngle * DEGS_TO_RADS);
		m_hourY = m_hourCenterY - m_hourLineLength * cos(m_windAngle * DEGS_TO_RADS);

		tft.drawLine(hourCenterX, hourCenterY, hourX, hourY, ILI9341_WHITE);
	}

	void increaseHour() {

		m_windDirIndex += 1;
		m_windDirIndex = m_windDirIndex > m_hoursToDegs.size() ? 0 : m_windDirIndex;		

		updateWindDirection();		
	}

	void decreaseHour() {

		m_windDirIndex -= 1;
		m_windDirIndex = m_windDirIndex < 0 ? m_hoursToDegs.size() : m_windDirIndex;

		updateWindDirection();
	}

	void increaseHourAndDrawLine() {

		tft.drawLine(hourCenterX, hourCenterY, hourX, hourY, ILI9341_BLACK);
		tft.fillCircle(203, 57, 4, ILI9341_WHITE);

		increaseHour();
	}

	void decreaseHourAndDrawLine() {
		
		tft.drawLine(hourCenterX, hourCenterY, hourX, hourY, ILI9341_BLACK); 
		tft.fillCircle(203, 57, 4, ILI9341_WHITE);

		decreaseHour();
	}
};

#endif /* _BC_WIND_DIRECTOR_H_ */