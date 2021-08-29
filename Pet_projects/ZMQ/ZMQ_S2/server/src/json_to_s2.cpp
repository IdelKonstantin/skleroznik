#include "json_to_s2.h"

json_error_t jsonToS2helper::parseJsonRequest(const std::string& data) {

    Json::Value root(data);
    JSONCPP_STRING err;
    Json::CharReaderBuilder builder;

    const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());

    if (!reader->parse(data.c_str(), data.c_str() + data.length(), &root, &err)) {

        this->errorCode = json_err::WRONG_JSON;    
        return this->errorCode;  
    }

    try {

        this->parseForMeteoData(root);
        this->parseForBulletData(root);
        this->parseForRifleData(root);
        this->parseForScopeData(root);
        this->parseForInputs(root);
        this->parseForOptions(root);
    }
    catch(...) {
    
        this->errorCode = json_err::PARSING_FAILURE;    
        return this->errorCode; 
    }

    return json_err::DONE;
}

void jsonToS2helper::parseForMeteoData(const Json::Value& root) {

    this->meteo.T = static_cast<decltype(this->meteo.T)>(root["Meteo"]["T"].asInt());
    this->meteo.P = static_cast<decltype(this->meteo.P)>(root["Meteo"]["P"].asUInt());
    this->meteo.H = static_cast<decltype(this->meteo.H)>(root["Meteo"]["H"].asUInt());
    this->meteo.windSpeed = static_cast<decltype(this->meteo.windSpeed)>(root["Meteo"]["windSpeed"].asDouble());
    this->meteo.windDir = static_cast<decltype(this->meteo.windDir)>(root["Meteo"]["windDir"].asUInt());
}

void jsonToS2helper::parseForBulletData(const Json::Value& root) {

	std::strcpy(this->bullet.bulletName, root["Bullet"]["bulletName"].asString().c_str());
	this->bullet.dragFunction = static_cast<decltype(this->bullet.dragFunction)>(root["Bullet"]["dragFunction"].asUInt());
	this->bullet.BC = static_cast<decltype(this->bullet.BC)>(root["Bullet"]["BC"].asDouble());
	this->bullet.DSF_0_9 = static_cast<decltype(this->bullet.DSF_0_9)>(root["Bullet"]["DSF_0_9"].asDouble());
	this->bullet.DSF_1_0 = static_cast<decltype(this->bullet.DSF_1_0)>(root["Bullet"]["DSF_1_0"].asDouble());
	this->bullet.DSF_1_1 = static_cast<decltype(this->bullet.DSF_1_1)>(root["Bullet"]["DSF_1_1"].asDouble());
	this->bullet.V0 = static_cast<decltype(this->bullet.V0)>(root["Bullet"]["V0"].asUInt());
	this->bullet.length = static_cast<decltype(this->bullet.length)>(root["Bullet"]["length"].asDouble());
	this->bullet.mass = static_cast<decltype(this->bullet.mass)>(root["Bullet"]["mass"].asUInt());
	this->bullet.calinber = static_cast<decltype(this->bullet.calinber)>(root["Bullet"]["calinber"].asDouble());
	this->bullet.V0temp = static_cast<decltype(this->bullet.V0temp)>(root["Bullet"]["V0temp"].asInt());		
	this->bullet.thermalSens = static_cast<decltype(this->bullet.thermalSens)>(root["Bullet"]["thermalSens"].asDouble());
}

void jsonToS2helper::parseForRifleData(const Json::Value& root) {

	std::strcpy(this->rifle.rifleName, root["rifle"]["rifleName"].asString().c_str());
	this->rifle.zeroDistance = static_cast<decltype(this->rifle.zeroDistance)>(root["Rifle"]["zeroDistance"].asUInt());
	this->rifle.scopeHight = static_cast<decltype(this->rifle.scopeHight)>(root["Rifle"]["scopeHight"].asDouble());
	this->rifle.twist = static_cast<decltype(this->rifle.twist)>(root["Rifle"]["twist"].asDouble());
	this->rifle.twistDir = static_cast<decltype(this->rifle.twistDir)>(root["Rifle"]["twistDir"].asUInt());
	this->rifle.zeroAtm = static_cast<decltype(this->rifle.zeroAtm)>(root["Rifle"]["zeroAtm"].asUInt());
	this->rifle.zeroTemp = static_cast<decltype(this->rifle.zeroTemp)>(root["Rifle"]["zeroTemp"].asInt());
	this->rifle.zeroPress = static_cast<decltype(this->rifle.zeroPress)>(root["Rifle"]["zeroPress"].asUInt());
	this->rifle.vertDrift = static_cast<decltype(this->rifle.vertDrift)>(root["Rifle"]["vertDrift"].asDouble());
	this->rifle.vertDrDir = static_cast<decltype(this->rifle.vertDrDir)>(root["Rifle"]["vertDrDir"].asUInt());		
	this->rifle.horizDrift = static_cast<decltype(this->rifle.horizDrift)>(root["Rifle"]["horizDrift"].asDouble());
	this->rifle.horizDrDir = static_cast<decltype(this->rifle.horizDrDir)>(root["Rifle"]["horizDrDir"].asUInt());
}

void jsonToS2helper::parseForScopeData(const Json::Value& root) {

	std::strcpy(this->scope.scopeName, root["Scope"]["scopeName"].asString().c_str());
	this->scope.angleUnits = static_cast<decltype(this->scope.angleUnits)>(root["Scope"]["angleUnits"].asUInt());
	this->scope.clickVert = static_cast<decltype(this->scope.clickVert)>(root["Scope"]["clickVert"].asDouble());
	this->scope.clickHoriz = static_cast<decltype(this->scope.clickHoriz)>(root["Scope"]["clickHoriz"].asDouble());
	this->scope.reticlePattern = static_cast<decltype(this->scope.reticlePattern)>(root["Scope"]["reticlePattern"].asUInt());
}

void jsonToS2helper::parseForInputs(const Json::Value& root) {
	
	this->inputs.shotDistance = static_cast<decltype(this->inputs.shotDistance)>(root["Inputs"]["shotDistance"].asUInt());
	this->inputs.terrainAndle = static_cast<decltype(this->inputs.terrainAndle)>(root["Inputs"]["terrainAndle"].asUInt());
	this->inputs.targetSpeedInMILs = static_cast<decltype(this->inputs.targetSpeedInMILs)>(root["Inputs"]["targetSpeedInMILs"].asDouble());
	this->inputs.targetAzimuth = static_cast<decltype(this->inputs.targetAzimuth)>(root["Inputs"]["targetAzimuth"].asInt());
	this->inputs.latitude = static_cast<decltype(this->inputs.latitude)>(root["Inputs"]["latitude"].asDouble());
	this->inputs.magneticIncl = static_cast<decltype(this->inputs.magneticIncl)>(root["Inputs"]["magneticIncl"].asDouble());
}

void jsonToS2helper::parseForOptions(const Json::Value& root) {

	this->options.Koriolis = static_cast<decltype(this->options.Koriolis)>(root["Options"]["Koriolis"].asUInt());
	this->options.BallisticTable = static_cast<decltype(this->options.BallisticTable)>(root["Options"]["BallisticTable"].asUInt());
	this->options.ThermalCorrection = static_cast<decltype(this->options.ThermalCorrection)>(root["Options"]["ThermalCorrection"].asUInt());
}

void jsonToS2helper::serializeResults(char* buffer) {

	Json::Value root;

	root["Vertical"].append(std::string("cm: " + std::to_string(this->results.vertSm)));
	root["Vertical"].append(std::string("angle: " + std::to_string(this->results.vertAngleUnits)));
	root["Vertical"].append(std::string("clicks: " + std::to_string(this->results.vertClicks)));

	root["Horizontal"].append(std::string("cm: " + std::to_string(this->results.horizSm)));
	root["Horizontal"].append(std::string("angle: " + std::to_string(this->results.horizAngleUnits)));
	root["Horizontal"].append(std::string("clicks: " + std::to_string(this->results.horizClicks)));

	root["Derivation"].append(std::string("cm: " + std::to_string(this->results.derivSm)));
	root["Derivation"].append(std::string("angle: " + std::to_string(this->results.derivAngleUnits)));
	root["Derivation"].append(std::string("clicks: " + std::to_string(this->results.derivClicks)));

	root["Time"].append(std::string("sec: " + std::to_string(this->results.flightTime)));
	root["Cinetic Energy"].append(std::string("J: " + std::to_string(this->results.cineticEnergy)));
	root["Mach Number"].append(std::string(std::to_string(this->results.MachNumber)));

	root["Calib.dist.(at 0.9 Mach)"].append("m: " + std::string(std::to_string(this->results.deepSubsonic)));
	root["Calib.dist.(at 1.0 Mach)"].append("m: " + std::string(std::to_string(this->results.subsonicDist)));
	root["Calib.dist.(at 1.1 Mach)"].append("m: " + std::string(std::to_string(this->results.transsonicDist)));

	root["Miller's FGS"].append(std::string(std::to_string(this->results.FGS)));

	Json::FastWriter fastwriter;

	std::memset(buffer, '\0', MAX_BUFFER_SIZE);
	std::strcpy(buffer, fastwriter.write(root).c_str());
}

void jsonToS2helper::getErrorMessage(char* buffer) {

	switch(this->errorCode) {

		case json_err::WRONG_JSON:
		std::strcpy(buffer, this->errorWrongJson);
		break;
		
		case json_err::PARSING_FAILURE:
		std::strcpy(buffer, this->parsingFailure);
		break;
	}
}

const struct Meteo* jsonToS2helper::getMeteo() {

	return &(this->meteo);
}

const struct Bullet* jsonToS2helper::getBullet() {

	return &(this->bullet);
}

const struct Rifle* jsonToS2helper::getRifle() {

	return &(this->rifle);
}

const struct Scope* jsonToS2helper::getScope() {

	return (&this->scope);
}

const struct Inputs* jsonToS2helper::getInputs() {

	return &(this->inputs);
}

const struct Options* jsonToS2helper::getOptions() {

	return &(this->options);
}

struct Results* jsonToS2helper::getResults() {

	return &(this->results);
}