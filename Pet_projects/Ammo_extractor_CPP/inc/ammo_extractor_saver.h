#pragma once

#include <iostream>
#include <fstream>
#include <memory>
#include <sstream>
#include <utility>
#include <cmath>
#include <cstdio>
#include <chrono>
#include <json/json.h>
#include <sqlite3.h>
#include <error_codes.h>
#include <bullet_data.h>

class ammoExtractor final {

private:

	std::string JSONpath{};
	std::string dummy{};
	std::string buffer{};
	std::stringstream ss{};

	std::vector<ammo::bulletData> allAmmos{};
	ammo::bulletData bulletDataUnit{};

	sqlite3 *db;
	char *zErrMsg{nullptr};

	auto getUnixTime();
	auto getRawJsonStrings();
	auto parseJSONStrings();
	auto saveToSqlite();
	static int callback(void *NotUsed, int argc, char **argv, char **azColName);

public:

	ammoExtractor() = delete;
	ammoExtractor(const std::string& JSONpath) : JSONpath(JSONpath) {};	
	ammo_error_t parseAndStore();
};