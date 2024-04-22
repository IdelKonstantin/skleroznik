#include "BCdata_uploader.h"
#include "settings_keeper.h"

void setup()
{
  Serial.begin(115200);
  configKeeper cfgk;

  if(!cfgk.init()) {
    Serial.println("No SPIFFS mounted");
    while(1);
  }

  if(!cfgk.readConfigsAndSetting()) {
    Serial.println("Cant read configs");
    while(1);
  }
  
  dataUploader{}.handleRESTAPI();
}

void loop()
{
  
}   
