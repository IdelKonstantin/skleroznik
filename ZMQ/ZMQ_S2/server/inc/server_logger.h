#pragma once

#include <chrono>
#include <fstream>
#include <sstream>
#include <iostream>
#include <boost/date_time/posix_time/posix_time_io.hpp>

using log_type_t = uint8_t;

namespace logType {

	const log_type_t REQUEST = 0;
	const log_type_t RESPONSE = 1;
}

class serverLogger {
	
	std::string path{"./"};
	std::string logLine{};
	std::string logTimeMark{};
	std::stringstream ss{};
	
	void addTimepointToPath();
	const std::string&  buildLogTimeMark();
	time_t getCurrentTimepoint();
	std::string getdateAndTime();

public:
	serverLogger();
	void logoutData(const std::string& serviceMessage, const log_type_t logType);
};
