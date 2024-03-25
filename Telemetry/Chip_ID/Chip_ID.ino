#include <WiFi.h>
#include <WebServer.h> // include ESP32 library
#include "SPIFFS.h"

#define FIRMVARE_VERSION "1.0.0.1"

namespace telemetry {

  String getChipName() {
    
    const size_t buffLength{11};
    char ssid[buffLength];
    snprintf(ssid, buffLength, "TLM-%llX", ESP.getEfuseMac());
    return ssid;
  }
};

class fileWorker {

private:
  const bool m_memoryOK{false};
  const uint32_t m_bytesToKB{1024};
  const float m_KBperMinute{30};

  uint32_t checkFreeMemory() {
    return SPIFFS.totalBytes() - SPIFFS.usedBytes(); 
  }

public:
  fileWorker() : m_memoryOK(SPIFFS.begin(true)) {}

  bool isInited() const {
    return m_memoryOK;
  }

  void clearMemory() const {
    SPIFFS.format();
  }

  uint16_t averageTimeToWrite() {
    auto freeKBytes = checkFreeMemory()/m_bytesToKB;
    return freeKBytes/m_KBperMinute;
  }
};

class chip {

private:
  const String m_chipID;
  const String m_firmvareVersion{FIRMVARE_VERSION};
  
  chip() : m_chipID(telemetry::getChipName()) {}  

public:

  static chip& info() {

    static chip c;
    return c;
  }

  const String& chipID() const {
    return m_chipID;
  }

  const String& firmvareVersion() const {
    return m_firmvareVersion;
  }

  const uint32_t usedSpace() const {
    return SPIFFS.usedBytes();
  }

  const uint32_t freeSpace() const {
    return SPIFFS.totalBytes() - SPIFFS.usedBytes();
  }
};

///////////////////////////////////////////////////////////////////////

fileWorker fw;
WebServer server (80);

void base() // function to load default webpage
{ // and send HTML code to client

  const String data_ = R"(<!DOCTYPE html>
<html lang="ru">
<head>
    <meta charset="UTF-8">
    <title>Диагностика устройства</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: #f5f5f5;
            text-align: center;
        }
        .container {
            width: 50%;
            margin: 0 auto;
            padding: 20px;
            background-color: #fff;
            border-radius: 10px;
            box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
        }
        h1 {
            color: #333;
        }
        label {
            display: block;
            margin-top: 10px;
        }
        input {
            width: 100%;
            padding: 5px;
            margin-top: 5px;
            box-sizing: border-box;
        }
        button {
            padding: 10px 20px;
            background-color: #ff5e62;
            color: #fff;
            border: none;
            border-radius: 5px;
            cursor: pointer;
            margin-top: 20px;
        }
        button:hover {
            background-color: #e54448;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>Диагностика устройства</h1>
        <label for="deviceID">ID устройства (Номер):</label>
        <input type="text" id="deviceID">
        <label for="firmwareVersion">Версия прошивки:</label>
        <input type="text" id="firmwareVersion">
        <label for="usedMemory">Использованная память:</label>
        <input type="text" id="usedMemory">
        <label for="availableMemory">Доступная память:</label>
        <input type="text" id="availableMemory">
        <label for="fileSystemType">Тип файловой системы:</label>
        <input type="text" id="fileSystemType">
        <button id="formatButton">Форматировать</button>
    </div>
</body>
</html>)";

  
  server.send(200, "text/html", data_);
}

void setup() {

  Serial.begin(115200);
  Serial.println("++=====================++");
  Serial.println(fw.averageTimeToWrite());
  Serial.println(chip::info().chipID());
  Serial.println(chip::info().firmvareVersion());
  Serial.println(chip::info().usedSpace());
  Serial.println(chip::info().freeSpace());

  IPAddress local_ip(192,168,2,1);  // pre-defined IP address,
  IPAddress gateway(192,168,2,1); // gateway
  IPAddress subnet(255,255,255,0); 

  WiFi.mode(WIFI_AP); // Wi-Fi AP mode
  delay(1000); // setup AP mode
  WiFi.softAP(chip::info().chipID().c_str(), chip::info().chipID().c_str());
  WiFi.softAPConfig(local_ip, gateway , subnet);
  // predefined IP address
  server.begin(); // initialise server
  server.on("/info", base);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
}
