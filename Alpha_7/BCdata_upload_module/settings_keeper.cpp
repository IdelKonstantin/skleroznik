#include "settings_keeper.h"
#include "BCdata_uploader.h"

extern configKeeper cfgKeeper;

bool bc_data::selectedBullet::operator!=(const selectedBullet& bullet) {

	return name != bullet.name || 
		DF != bullet.DF ||
		BC != bullet.BC ||
		MV != bullet.MV ||
		length != bullet.length ||
		weight != bullet.weight ||
		caliber != bullet.caliber ||
		MVtemp != bullet.MVtemp ||
		thermSens != bullet.thermSens ||
		CFM0_9 != bullet.CFM0_9 ||
		CFM1_0 != bullet.CFM1_0 ||
		CFM1_1 != bullet.CFM1_1;
}

bool bc_data::selectedRifle::operator!=(const selectedRifle& rifle) {

	return name != rifle.name || 
		scopeHight != rifle.scopeHight ||
		zeroDist != rifle.zeroDist ||
		zeroVertDrift != rifle.zeroVertDrift ||
		zeroVertDriftDir != rifle.zeroVertDriftDir ||
		zeroHorizDrift != rifle.zeroHorizDrift ||
		zeroHorizDriftDir != rifle.zeroHorizDriftDir ||
		scopeUnits != rifle.scopeUnits ||
		vertClick != rifle.vertClick ||
		horizClick != rifle.horizClick ||
		twist != rifle.twist ||
		twistDir != rifle.twistDir ||
		zeroingAt != rifle.zeroingAt ||
		zeroT != rifle.zeroT ||
		zeroP != rifle.zeroP;
}

bool bc_data::deviceSettings::operator!=(const deviceSettings& settings) {

	return backlIntencity != settings.backlIntencity ||
		backlFadeSec != settings.backlFadeSec ||
		autoOffMin != settings.autoOffMin ||
		latitude != settings.latitude ||
		magneticIncl != settings.magneticIncl;
}

bool bc_data::deviceInputs::operator!=(const deviceInputs& inputs) {

	return koriolis != inputs.koriolis ||
		termoCorr != inputs.termoCorr ||
		rangeCard != inputs.rangeCard ||
		aeroJump != inputs.aeroJump;
}

bool bc_data::targetInfo::operator!=(const targetInfo& target) {

	return distance != target.distance || 
		terrainAngle != target.terrainAngle ||
		speedMILs != target.speedMILs ||
		azimuth != target.azimuth;
}

bool bc_data::mildotCalculator::operator!=(const mildotCalculator& mildot) {


	return targetSizeMeters != mildot.targetSizeMeters ||
		targetSizeMils != mildot.targetSizeMils;
}

/////////////////////////////////////////////////////////////////////////

bool configKeeper::init() const {

	return SPIFFS.begin(false);
}

bool configKeeper::readConfigsAndSetting() {

	if(!readDeviceSettings(settings)) {
		return false;
	}

	if(!readSelectedBullet(bullet)) {
		return false;
	}

	if(!readSelectedRifle(rifle)) {
		return false;
	}

	if(!readDeviceInputs(inputs)) {
		return false;
	}

	if(!readTargetInfo(target)) {
		return false;
	}

	return readMildotInputs(mildot);
}

template<std::size_t N>
std::pair<bool, StaticJsonDocument<N>> configKeeper::readAndDeserializeJSON(const char* path) {

	if(!SPIFFS.exists(path)) {
		return std::make_pair(false, StaticJsonDocument<N>{});
	}

	auto file = SPIFFS.open(path, "r");
		
	if(!file) {
		return std::make_pair(false, StaticJsonDocument<N>{});
	}

	StaticJsonDocument<N> doc;
	auto error = deserializeJson(doc, file);
	
	if (error) {

		file.close();
		return {false, doc};
	}

	file.close();
	return {true, doc};
}

bool configKeeper::readDeviceInputs(bc_data::deviceInputs& inputs) { 

	bool success;
	StaticJsonDocument<INPUTS_BUFF_LENGTH> doc;

	std::tie(success, doc) = readAndDeserializeJSON<INPUTS_BUFF_LENGTH>(INPUTS_DATAFILE);

	if(!success) {
		return false;
	}

	inputs.koriolis = doc["koriolis"].as<bool>();
	inputs.termoCorr = doc["termocorr."].as<bool>();
	inputs.rangeCard = doc["range_card"].as<bool>();
	inputs.aeroJump = doc["aero_jump"].as<bool>();
	
	return true;
}

bool configKeeper::readTargetInfo(bc_data::targetInfo& target) {

	bool success;
	StaticJsonDocument<TARGET_BUFF_LENGTH> doc;

	std::tie(success, doc) = readAndDeserializeJSON<TARGET_BUFF_LENGTH>(TARGET_DATAFILE);

	if(!success) {
		return false;
	}

	target.distance = doc["distance"].as<uint16_t>();
	target.terrainAngle = doc["terrain_angle"].as<uint8_t>();
	target.speedMILs = doc["speed_mils"].as<float>();
	target.azimuth = doc["azimuth"].as<uint16_t>();

	return true;
}

bool configKeeper::readMildotInputs(bc_data::mildotCalculator& mildot) {

	bool success;
	StaticJsonDocument<MILDOT_BUFF_LENGTH> doc;

	std::tie(success, doc) = readAndDeserializeJSON<MILDOT_BUFF_LENGTH>(MILDOT_DATAFILE);

	if(!success) {
		return false;
	}

	mildot.targetSizeMeters = doc["size"].as<float>();
	mildot.targetSizeMils = doc["mils"].as<float>();

	return true;
}

bool configKeeper::readDeviceSettings(bc_data::deviceSettings& settings) {

	bool success;
	StaticJsonDocument<SETTINGS_BUFF_LENGTH> doc;

	std::tie(success, doc) = readAndDeserializeJSON<SETTINGS_BUFF_LENGTH>(SETTINGS_DATAFILE);

	if(!success) {
		return false;
	}

	settings.backlIntencity = doc["bckl.intens."].as<uint16_t>();
	settings.backlFadeSec = doc["bckl.fade_sec"].as<uint16_t>();
	settings.autoOffMin = doc["autooff_min"].as<uint16_t>();
	settings.latitude = doc["latitude"].as<float>();
	settings.magneticIncl = doc["magnetic.incl."].as<float>();

	return true;
}

bool configKeeper::readSelectedBullet(bc_data::selectedBullet& bullet) {

	bool success;
	StaticJsonDocument<BULLET_BUFF_LENGTH> doc;

	std::tie(success, doc) = readAndDeserializeJSON<BULLET_BUFF_LENGTH>(BULLETS_DATAFILE);

	if(!success) {
		return false;
	}

	auto index = doc["current_bullet_index"].as<uint16_t>();
	auto root = doc["bullets"][index];

	bullet.index = index;
	bullet.name = root["name"].as<String>();
	bullet.DF = root["DF"].as<String>() == "G1" ? 1 : 7;
	bullet.BC = root["BC"].as<float>();
	bullet.MV = root["MV"].as<uint16_t>();
	bullet.length = root["length"].as<float>();
	bullet.weight = root["weight"].as<uint16_t>();
	bullet.caliber = root["caliber"].as<float>();
	bullet.MVtemp = root["MV_temp."].as<int16_t>();
	bullet.thermSens = root["therm.sens."].as<float>();
	bullet.CFM0_9 = root["CF_M0.9"].as<float>();
	bullet.CFM1_0 = root["CF_M1.0"].as<float>();
	bullet.CFM1_1 = root["CF_M1.1"].as<float>();

	return true;
}

bool configKeeper::readSelectedRifle(bc_data::selectedRifle& rifle) {

	bool success;
	StaticJsonDocument<RIFLE_BUFF_LENGTH> doc;

	std::tie(success, doc) = readAndDeserializeJSON<RIFLE_BUFF_LENGTH>(RIFLES_DATAFILE);

	if(!success) {
		return false;
	}

	auto index = doc["current_rifle_index"].as<uint16_t>();
	auto root = doc["rifles"][index];

	rifle.index = index;

	rifle.name = root["name"].as<String>();
	rifle.scopeHight = root["scope_hight"].as<float>();
	rifle.zeroDist = root["zero_dist."].as<uint16_t>();
	rifle.zeroVertDrift = root["zero_vert.drift"].as<float>();
	rifle.zeroVertDriftDir = root["zero_vert.drify.dir."].as<String>() != "up";
	rifle.zeroHorizDrift = root["zero_horiz.drift"].as<float>();
	rifle.zeroHorizDriftDir = root["zero_horiz.drift.dir"].as<String>() != "left";
	rifle.scopeUnits = root["scope_units"].as<String>() == "MOA" ? 1 : 2;
	rifle.vertClick = root["vert.click"].as<float>();
	rifle.horizClick = root["horiz.click"].as<float>();
	rifle.twist = root["twist"].as<uint16_t>();
	rifle.twistDir = root["twist_dir."].as<String>() != "right";
	rifle.zeroingAt = root["zeroing"].as<String>() != "here";
	rifle.zeroT = root["zero_T"].as<int16_t>();
	rifle.zeroP = root["zero_P"].as<uint16_t>();

	return true;
}

template<std::size_t N> 
void configKeeper::saveSerializedJSON(const StaticJsonDocument<N>& doc, const char* path) {

	auto file = SPIFFS.open(path, "w");

	if(file) {

		serializeJsonPretty(doc, file);
		file.close();			
	}
}

void configKeeper::compareAndSaveDeviceSettings(const bc_data::deviceSettings& settings_) {

	if(settings != settings_) {
		
		settings = settings_;

		StaticJsonDocument<SETTINGS_BUFF_LENGTH> doc;

		doc["bckl.intens."] = settings.backlIntencity;
		doc["bckl.fade_sec"] = settings.backlFadeSec;
		doc["autooff_min"] = settings.autoOffMin;
		doc["latitude"] = settings.latitude;
		doc["magnetic.incl."] = settings.magneticIncl;

		saveSerializedJSON<SETTINGS_BUFF_LENGTH>(doc, SETTINGS_DATAFILE);
	}
}

void configKeeper::compareAndSaveDeviceInputs(const bc_data::deviceInputs& inputs_) {

	if(inputs != inputs_) {
		
		inputs = inputs_;

		StaticJsonDocument<INPUTS_BUFF_LENGTH> doc;

		doc["koriolis"] = inputs.koriolis;
		doc["termocorr."] = inputs.termoCorr;
		doc["range_card"] = inputs.rangeCard;
		doc["aero_jump"] = inputs.aeroJump;

		saveSerializedJSON<INPUTS_BUFF_LENGTH>(doc, INPUTS_DATAFILE);
	}
}

void configKeeper::compareAndSaveTargetInfo(const bc_data::targetInfo& target_) {

	if(target != target_) {
		
		target = target_;

		StaticJsonDocument<TARGET_BUFF_LENGTH> doc;

		doc["distance"] = target.distance;
		doc["terrain_angle"] = target.terrainAngle;
		doc["speed_mils"] = target.speedMILs;
		doc["azimuth"] = target.azimuth;

		saveSerializedJSON<TARGET_BUFF_LENGTH>(doc, TARGET_DATAFILE);
	}
}

void configKeeper::compareAndSaveMildotInputs(const bc_data::mildotCalculator& mildot_) {

	if(mildot != mildot_) {
		
		mildot = mildot_;

		StaticJsonDocument<MILDOT_BUFF_LENGTH> doc;

		doc["size"] = mildot.targetSizeMeters;
		doc["mils"] = mildot.targetSizeMils;

		saveSerializedJSON<MILDOT_BUFF_LENGTH>(doc, MILDOT_DATAFILE);
	}
}

bool configKeeper::processFileWithSelectedIndex(const char* path, const char* indexTag, size_t index) {

	auto file = SPIFFS.open(path, "r");

	if(!file) {
		return false;
	}

	StaticJsonDocument<BIG_BUFF_LENGTH> doc;

	auto error = deserializeJson(doc, file);
	file.close();
	
	if (error) {
		return false;
	}

	doc[indexTag] = index;

	file = SPIFFS.open(path, "w");

	if(!file) {
		return false;
	}

	String configData;

	serializeJsonPretty(doc, configData);

	file.print(configData);
	file.close();

	return true;
}

void configKeeper::selectBulletWithIndex(size_t index) {

	if(processFileWithSelectedIndex(BULLETS_DATAFILE, "current_bullet_index", index)) {
		
		bc_data::selectedBullet newBullet;
		cfgKeeper.readSelectedBullet(newBullet);

		if(newBullet != cfgKeeper.bullet) {
			cfgKeeper.bullet = newBullet;
		}		
	}
}

void configKeeper::selectRifleWithIndex(size_t index) {

	if(processFileWithSelectedIndex(RIFLES_DATAFILE, "current_rifle_index", index)) {
		
		bc_data::selectedRifle newRifle;
		cfgKeeper.readSelectedRifle(newRifle);

		if(newRifle != cfgKeeper.rifle) {
			cfgKeeper.rifle = newRifle;
		}	
	}
}