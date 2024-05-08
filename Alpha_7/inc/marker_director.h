#ifndef _BC_MARKER_DIRECTOR_H_
#define _BC_MARKER_DIRECTOR_H_

#include "TFT_worker.h"

extern TFT_UNIT tft;

#define HORIZONTAL_RESOLUTION 240

class markerDirector {

private:

	int16_t m_markerIndex{0};
	uint16_t m_maxLines{0};
	uint16_t m_fontVSize;
	uint16_t m_rightOffset;
	uint16_t m_topMargin;

	void eraseMarker() const;
	void drawMarker() const;

public:
	markerDirector();

	void setMarkerData(int16_t index, uint16_t maxLines, uint16_t fontVSize, uint16_t rightOffset, uint16_t topMargin);
	void downMarker();
	void upMarker();
	uint16_t getMarkerIndex() const;
};

#endif /* _BC_MARKER_DIRECTOR_H_ */