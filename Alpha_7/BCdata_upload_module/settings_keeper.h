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

	struct deviceSettings {

		uint16_t backlIntencity;
		uint16_t backlFadeSec;
		uint16_t autoOffMin;
		float latitude;
		float magneticIncl;
		
		bool operator!=(const deviceSettings& settings);
	};

	//TODO: Дописать для всех структур, хранящих настройки
};

class configKeeper {

private:

	bool readDeviceSettings();

public:

	bc_data::selectedBullet bullet;
	bc_data::selectedRifle rifle;
	bc_data::deviceSettings settings;

	bool init() const;
	bool readConfigsAndSetting();
	bool readSelectedBullet(bc_data::selectedBullet& bullet);
	bool readSelectedRifle(bc_data::selectedRifle& rifle);
	bool readDeviceSettings(bc_data::deviceSettings& settings);

	void selectBulletWithIndex(size_t index) {

		//TODO...
	}

	void selectRifleWithIndex(size_t index) {

		//TODO...
	}
};

#endif /* _SETTINGS_KEEPER_H_ */