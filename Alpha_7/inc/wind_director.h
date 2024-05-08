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

	const uint16_t m_hourLineLength{25};
	const uint16_t m_hourCenterX{203};
	const uint16_t m_hourCenterY{57};
	const size_t m_windIndexMax;

	void updateWindDirection();
	void drawClockLine(uint16_t color);

public:
	windDirector();

	void drawClock();
	void drawHourLine();
	void increaseHour();
	void decreaseHour();
	void increaseHourAndDrawLine();
	void decreaseHourAndDrawLine();

	float getWindHour() const;
	uint16_t getWindAngle() const;
};

#endif /* _BC_WIND_DIRECTOR_H_ */