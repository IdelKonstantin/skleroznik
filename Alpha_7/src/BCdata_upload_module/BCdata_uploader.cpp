#include "../../inc/BCdata_uploader.h"
#include "../../inc/device_info.h"
#include "../../inc/settings_keeper.h"

extern configKeeper cfgKeeper;

dataUploader::dataUploader() {

	IPAddress local_ip(192,168,2,1);
	IPAddress gateway(192,168,2,1);
	IPAddress subnet(255,255,255,0);

	WiFi.disconnect(false);
	WiFi.mode(WIFI_AP);
	WiFi.softAP(chip::info().chipID().c_str(), chip::info().chipID().c_str());
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
	processRESTAPIRequests();
}

void dataUploader::initServer() {

	ElegantOTA.setAuth("s2admin", "s2admin");
	ElegantOTA.begin(&m_server);
	m_server.begin();

/****************** GETS ******************/

	m_server.on("/bullets_get", HTTP_GET, [this]() {

		if(SPIFFS.exists(BULLETS_DATAFILE)) {

			auto file = SPIFFS.open(BULLETS_DATAFILE, "r");
			
			if(file) {
				m_server.streamFile(file, "application/json");
				file.close();
			} else {
				m_server.send(500, "text/plain", "Failed to open bullets config file");
			}
		} else {
			m_server.send(404, "text/plain", "Bullets config file not found in SPIFFS");
		}
	});

	m_server.on("/rifles_get", HTTP_GET, [this]() {

		if(SPIFFS.exists(RIFLES_DATAFILE)) {

			auto file = SPIFFS.open(RIFLES_DATAFILE, "r");
			
			if(file) {
				m_server.streamFile(file, "application/json");
				file.close();
			} else {
				m_server.send(500, "text/plain", "Failed to open rifles config file");
			}
		} else {
			m_server.send(404, "text/plain", "Rifles config file not found in SPIFFS");
		}
	});

	m_server.on("/device_info", HTTP_GET, [this]() {

		StaticJsonDocument<256> info;
		info["DeviceName"] = chip::info().deviceName();
		info["FirmwareVersion"] = chip::info().firmwareVersion();
		info["ReleaseDate"] = chip::info().releaseDate();
		info["Website"] = chip::info().webSite();

		String responce;
		serializeJsonPretty(info, responce);

		m_server.send(200, "application/json", responce);
	});

	m_server.on("/settings_get", HTTP_GET, [this]() {

		if(SPIFFS.exists(SETTINGS_DATAFILE)) {

			auto file = SPIFFS.open(SETTINGS_DATAFILE, "r");
			
			if(file) {
				m_server.streamFile(file, "application/json");
				file.close();
			} else {
				m_server.send(500, "text/plain", "Failed to open settings config file");
			}
		} else {
			m_server.send(404, "text/plain", "Settings config file not found in SPIFFS");
		}
	});

	m_server.on("/hard_reset", HTTP_GET, [this]() {

		if(!dataRestorator::formatAndRestore()) {
			m_server.send(500, "text/plain", "Failed to reset to defaults");
		}
		else {
			m_server.send(200, "text/plain", "Device has been reseted/restarted");
			delay(1000);
			ESP.restart();		
		}
	});	

	m_server.on("/hard_reset", HTTP_GET, [this]() {

		if(!dataRestorator::formatAndRestore()) {
			m_server.send(500, "text/plain", "Failed to reset to defaults");
		}
		else {
			m_server.send(200, "text/plain", "Device has been reseted/restarted");
			delay(1000);
			ESP.restart();		
		}
	});	

	m_server.on("/bt_get", HTTP_GET, [this]() {

		if(SPIFFS.exists(BALLISTIC_TABLE)) {

			auto file = SPIFFS.open(BALLISTIC_TABLE, "r");
			
			if(file) {
				m_server.streamFile(file, "application/json");
				file.close();
			} else {
				m_server.send(500, "text/plain", "Failed to open range card file");
			}
		} else {
			m_server.send(404, "text/plain", "No range card yet created");
		}
	});	


/****************** POSTS ******************/

	m_server.on("/bullets_post", HTTP_POST, [this]() {

		auto data = m_server.arg(0);

		if(data.length() != 0) {

			auto file = SPIFFS.open(BULLETS_DATAFILE, "w");

			if(file) {
				file.print(data);
				file.close();

				bc_data::selectedBullet newBullet;
				cfgKeeper.readSelectedBullet(newBullet);

				if(newBullet != cfgKeeper.bullet) {
					cfgKeeper.bullet = newBullet;
				}

				m_server.send(200, "text/plain", "OK");

			} else {
				m_server.send(500, "text/plain", "Failed to open bullets config file for writing");
			}

		} else {
			m_server.send(400, "text/plain", "No bullets data received");
		}
	});

	m_server.on("/rifles_post", HTTP_POST, [this]() {

		auto data = m_server.arg(0);

		if(data.length() != 0) {

			auto file = SPIFFS.open(RIFLES_DATAFILE, "w");

			if(file) {
				file.print(data);
				file.close();

				bc_data::selectedRifle newRifle;
				cfgKeeper.readSelectedRifle(newRifle);

				if(newRifle != cfgKeeper.rifle) {
					cfgKeeper.rifle = newRifle;
				}

				m_server.send(200, "text/plain", "OK");

			} else {
				m_server.send(500, "text/plain", "Failed to open rifles config file for writing");
			}

		} else {
			m_server.send(400, "text/plain", "No bullets data received");
		}
	});

	m_server.on("/settings_post", HTTP_POST, [this]() {

		auto data = m_server.arg(0);

		if(data.length() != 0) {

			auto file = SPIFFS.open(SETTINGS_DATAFILE, "w");

			if(file) {
				file.print(data);
				file.close();

				bc_data::deviceSettings newSettings;
				cfgKeeper.readDeviceSettings(newSettings);

				if(newSettings != cfgKeeper.settings) {
					cfgKeeper.settings = newSettings;
				}

				m_server.send(200, "text/plain", "OK");

			} else {
				m_server.send(500, "text/plain", "Failed to open settings config file for writing");
			}

		} else {
			m_server.send(400, "text/plain", "No settings data received");
		}
	});	
}

void dataUploader::processRESTAPIRequests() {

	while(m_canWork) {	
		m_server.handleClient();
		ElegantOTA.loop();
	}

	m_server.stop();
}