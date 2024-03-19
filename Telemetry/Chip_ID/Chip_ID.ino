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

void setup() {

  Serial.begin(115200);
  Serial.println("++=====================++");
  fw.clearMemory();
  Serial.println(fw.averageTimeToWrite());
  Serial.println(chip::info().chipID());
  Serial.println(chip::info().firmvareVersion());
  Serial.println(chip::info().usedSpace());
  Serial.println(chip::info().freeSpace());
}

void loop() {
  // put your main code here, to run repeatedly:

}
