#ifndef _SETTINGS_KEEPER_H_
#define _SETTINGS_KEEPER_H_

#include <ArduinoJson.h>

namespace bc_data {

	struct bullet {

	};

	struct rifle {

	};
};

class configKeeper {

public:

	bc_data::bullet bullet;
	bc_data::rifle rifle;

};

#endif /* _SETTINGS_KEEPER_H_ */