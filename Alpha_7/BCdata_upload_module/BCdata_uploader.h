#ifndef _BC_DATA_UPLOADER_H_
#define _BC_DATA_UPLOADER_H_

#include <esp_wifi.h>
#include <WiFi.h>
#include <WebServer.h>
#include "SPIFFS.h"

#define FIRMVARE_VERSION "1.0.0.1"

namespace bc_data {
	String getChipName(); 
};

class chip {

private:
	
	const String m_chipID;
	const String m_firmvareVersion{FIRMVARE_VERSION};

	chip();

public:

	static chip& info();
	const String& chipID() const;
	const String& firmvareVersion() const;
};

//////////////////////////////////////////////////////////////

class dataUploader {

private:

	bool m_canWork{true};
	WebServer m_server;

	void initServer();
	void processRESAPIRequests();

public:

	dataUploader();
	~dataUploader();

	void finishProcessing();
	void handleRESAPI();
};

#endif /* _BC_DATA_UPLOADER_H_ */