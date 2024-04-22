#ifndef _SETTINGS_KEEPER_H_
#define _SETTINGS_KEEPER_H_

#include <SPIFFS.h>
#include <ArduinoJson.h>
#include <cstdint>

namespace bc_data {

	struct selectedBullet {

		uint16_t index;
		String name;
		uint16_t DF;
		float BC;
		uint16_t MV;
		float length;
		uint16_t weight;
		float caliber;
		int16_t MVtemp;
		float thermSens;
		float CFM0_9;
		float CFM1_0;
		float CFM1_1;

		bool operator!=(const selectedBullet& bullet);
	};

	struct selectedRifle {

		uint16_t index;
		String name;
		float scopeHight;
		uint16_t zeroDist;
		float zeroVertDrift;
		bool zeroVertDriftDir;
		float zeroHorizDrift;
		bool zeroHorizDriftDir;
		uint16_t scopeUnits;
		float vertClick;
		float horizClick;
		uint16_t twist;
		bool twistDir;
		bool zeroingAt;
		int16_t zeroT;
		uint16_t zeroP;

		bool operator!=(const selectedRifle& rifle);
	};

	//TODO: Дописать для всех структур, хранящих настройки
};

class configKeeper {

public:

	bc_data::selectedBullet bullet;
	bc_data::selectedRifle rifle;

	bool init() const;
	bool readConfigsAndSetting();
	bool readSelectedBullet(bc_data::selectedBullet& bullet);
	bool readSelectedRifle(bc_data::selectedRifle& rifle);

	void selectBulletWithIndex(size_t index) {

		//TODO...
	}

	void selectRifleWithIndex(size_t index) {

		//TODO...
	}
};

#endif /* _SETTINGS_KEEPER_H_ */