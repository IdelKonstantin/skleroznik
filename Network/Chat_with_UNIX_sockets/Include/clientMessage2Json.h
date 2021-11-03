#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include <json/json.h>

class clientMessageToJsonString final { 

public:

	struct dataForSerialization {

		std::string clientID;
		std::string message;
	};

	clientMessageToJsonString() = default;
	std::string serializeToJson(dataForSerialization&& data);
	std::string deserializeToString(std::string&& readMessage);
};