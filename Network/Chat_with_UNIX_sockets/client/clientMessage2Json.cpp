#include "clientMessage2Json.h"

std::string clientMessageToJsonString::serializeToJson(clientMessageToJsonString::dataForSerialization&& data) {
	
	Json::Value root;

	root["ID"] = data.clientID;
	root["Message"] = data.message;

	Json::FastWriter fastwriter;
	return fastwriter.write(root);
}


std::string clientMessageToJsonString::deserializeToString(std::string&& readMessage) {

	Json::Value root(readMessage);
    JSONCPP_STRING err;
    Json::CharReaderBuilder builder;

    const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());

    if (!reader->parse(readMessage.c_str(), readMessage.c_str() + readMessage.length(), &root, &err)) {
 
        return std::string("Error in incoming message. Reason UNKNOWN"); 
    }

    std::string senderID{};
    std::string messageBody{};

    try {

    	senderID = root["ID"].asString();
    	messageBody = root["Message"].asString();
    }
    catch(const std::exception& ex) {
       
        return std::string("Error in incoming message. Reason: ") + std::string(ex.what());
    }

	return std::string{"[" + senderID + "]: " + messageBody};
}