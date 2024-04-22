#ifndef _BC_DATA_UPLOADER_H_
#define _BC_DATA_UPLOADER_H_

#include <esp_wifi.h>
#include <WiFi.h>
#include <WebServer.h>
#include "SPIFFS.h"

#define BULLETS_DATAFILE "/bullets.cfg"
#define RIFLES_DATAFILE "/rifles.cfg"

//Установить ArduinoJson (by Benoit Blanchon) в менеджере библиотек IDE

class dataUploader {

private:

	bool m_canWork{true};
	WebServer m_server;

	void initServer();
	void processRESTAPIRequests();

public:

	dataUploader();
	~dataUploader();

	void finishProcessing();
	void handleRESTAPI();
};

#endif /* _BC_DATA_UPLOADER_H_ */