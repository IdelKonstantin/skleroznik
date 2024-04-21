#include "BCdata_uploader.h"
#include "device_info.h"

dataUploader::dataUploader() {

	IPAddress local_ip(192,168,2,1);
	IPAddress gateway(192,168,2,1);
	IPAddress subnet(255,255,255,0); 

	WiFi.disconnect(false);
	WiFi.mode(WIFI_AP);
	WiFi.softAP(chip::info().chipID(), chip::info().chipID());
	WiFi.softAPConfig(local_ip, gateway , subnet);
	esp_wifi_set_ps(WIFI_PS_MAX_MODEM);
}

dataUploader::~dataUploader() {

	WiFi.disconnect(true);
	WiFi.mode(WIFI_OFF);
}

void dataUploader::finishProcessing() {

	m_canWork = false;
}

void dataUploader::handleRESTAPI() {

	initServer();
	processRESAPIRequests();
}

void dataUploader::initServer() {

	m_server.begin();

	m_server.on("/bullets_get", HTTP_GET, [this]() {

		//todo: прочесть конфиг с пулями из SPIFFS и выгрузить
		m_server.send(200, "application/json", "Bullets");
	});

	m_server.on("/rifles_get", HTTP_GET, [this]() {

		//todo: прочесть конфиг с винтовками из SPIFFS и выгрузить
		m_server.send(200, "application/json", "Rifles");
	});

	m_server.on("/bullets_post", HTTP_POST, [this]() {

		//todo: сохранить конфиг с пулями в SPIFFS
		//и обновить поля класса-харнителя выбранной пули
		String data = m_server.arg("data");
		m_server.send(200, "text/plain", "OK");
	});

	m_server.on("/rifles_post", HTTP_POST, [this]() {

		//todo: сохранить конфиг с винтовками в SPIFFS
		//и обновить поля класса-харнителя выбранной винтовки
		String data = m_server.arg("data");
		m_server.send(200, "text/plain", "OK");
	});	
}

void dataUploader::processRESTAPIRequests() {

	while(m_canWork) {
		
		m_server.handleClient();
	}

	m_server.stop();
}