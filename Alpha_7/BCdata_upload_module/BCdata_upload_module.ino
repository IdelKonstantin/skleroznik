#include "BCdata_uploader.h"
#include "settings_keeper.h"

configKeeper cfgKeeper;

void setup()
{
  Serial.begin(115200);
  
  if(!cfgKeeper.init()) {
    Serial.println("No SPIFFS mounted");
    while(1);
  }

//  Serial.println("Formating...");
//  dataRestorator::formatAndRestore();

  if(!cfgKeeper.readConfigsAndSetting()) {
    Serial.println("Cant read configs");
    while(1);
  }
  
  dataUploader{}.handleRESTAPI();
}

void loop()
{
  
}   
