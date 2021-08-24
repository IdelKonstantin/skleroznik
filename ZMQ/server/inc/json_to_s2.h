#pragma once

#include <cstring>
#include <cstdint>
#include <memory>
#include <vector>
#include <json/json.h>
#include <trajectory_solver_API.h>

#define MAX_BUFFER_SIZE 0xFFF

using json_error_t = uint8_t;

namespace json_err {

	const json_error_t DONE = 0;
	const json_error_t WRONG_JSON = 1;	
	const json_error_t PARSING_FAILURE = 2;
}

class jsonToS2helper final { 

private:

	Meteo meteo{};
	Bullet bullet{};
	Rifle rifle{};
	Scope scope{};
	Inputs inputs{};
	Options options{};
	Results results{};

	const char* errorWrongJson = "{\"Error\": {\"errorDescription\": \"Wrong JSON\"}}";
	const char* parsingFailure = "{\"Error\": {\"errorDescription\": \"JSON parsing failure\"}}";

	json_error_t errorCode{json_err::DONE};

	void parseForMeteoData(const Json::Value& root);
	void parseForBulletData(const Json::Value& root);
	void parseForRifleData(const Json::Value& root);
	void parseForScopeData(const Json::Value& root);
	void parseForInputs(const Json::Value& root);
	void parseForOptions(const Json::Value& root);

public:

	jsonToS2helper() = default;
	json_error_t parseJsonRequest(const std::string& data);

	const struct Meteo* getMeteo();
	const struct Bullet* getBullet();
	const struct Rifle* getRifle();
	const struct Scope* getScope();
	const struct Inputs* getInputs();
	const struct Options* getOptions();
	struct Results* getResults();

	void getErrorMessage(char* buffer);
	void serializeResults(char* buffer);		
};
