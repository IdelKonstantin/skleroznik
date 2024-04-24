#ifndef _SETTINGS_KEEPER_H_
#define _SETTINGS_KEEPER_H_

#include <SPIFFS.h>
#include <ArduinoJson.h>
#include <cstdint>

#include "BCdata_restorator.h"

#define RIFLE_BUFF_LENGTH 4096
#define BULLET_BUFF_LENGTH 4096
#define SETTINGS_BUFF_LENGTH 512
#define INPUTS_BUFF_LENGTH 256
#define TARGET_BUFF_LENGTH 256
#define MILDOT_BUFF_LENGTH 127

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

	struct deviceInputs {

		bool koriolis{false};
		bool termoCorr{false};
		bool rangeCard{false};
		bool aeroJump{false};

		bool operator!=(const deviceInputs& inputs);
	};

	struct targetInfo {

		uint16_t distance;
		uint8_t terrainAngle;
		float speedMILs;
		uint16_t azimuth;

		bool operator!=(const targetInfo& target);
	};

	struct mildotCalculator {

		float targetSizeMeters;
		float targetSizeMils;

		bool operator!=(const mildotCalculator& mildot);
	};
};

class configKeeper {

private:

	template<std::size_t N> 
	std::pair<bool, StaticJsonDocument<N>> readAndDeserializeJSON(const char* path);

	template<std::size_t N> 
	void saveSerializedJSON(const StaticJsonDocument<N>& doc, const char* path);

public:

	bc_data::selectedBullet bullet;
	bc_data::selectedRifle rifle;
	bc_data::deviceSettings settings;
	bc_data::deviceInputs inputs;
	bc_data::targetInfo target;
	bc_data::mildotCalculator mildot;

	bool init() const;
	bool readConfigsAndSetting();
	bool readSelectedBullet(bc_data::selectedBullet& bullet);
	bool readSelectedRifle(bc_data::selectedRifle& rifle);

	bool readDeviceSettings(bc_data::deviceSettings& settings);
	bool readDeviceInputs(bc_data::deviceInputs& inputs);
	bool readTargetInfo(bc_data::targetInfo& target);
	bool readMildotInputs(bc_data::mildotCalculator& mildot);

	void compareAndSaveDeviceSettings(const bc_data::deviceSettings& settings_);
	void compareAndSaveDeviceInputs(bc_data::deviceInputs& inputs_);
	void compareAndSaveTargetInfo(bc_data::targetInfo& target_);
	void compareAndSaveMildotInputs(bc_data::mildotCalculator& mildot_);

	//TODO: 
	void selectBulletWithIndex(size_t index);
	void selectRifleWithIndex(size_t index);
};

#endif /* _SETTINGS_KEEPER_H_ */