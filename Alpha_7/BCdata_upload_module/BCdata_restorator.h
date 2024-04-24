#ifndef _BC_DATA_RESTORATOR_H_
#define _BC_DATA_RESTORATOR_H_

#define BULLETS_DATAFILE "/bullets.cfg"
#define RIFLES_DATAFILE "/rifles.cfg"
#define SETTINGS_DATAFILE "/settings.cfg"
#define INPUTS_DATAFILE "/inputs.cfg"
#define TARGET_DATAFILE "/target.cfg"
#define MILDOT_DATAFILE "/mildot.cfg"

#include <SPIFFS.h>

class dataRestorator {

private:
	static bool saveConfigFile(const std::pair<const char*, const char*>& config);

public:
	static bool formatAndRestore(); 
};

#endif /* _BC_DATA_RESTORATOR_H_ */