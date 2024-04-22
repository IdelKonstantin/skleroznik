#include "BCdata_uploader.h"

void setup()
{
  SPIFFS.begin(false);
  Serial.begin(115200);
  Serial.println("Begin");
  dataUploader{}.handleRESTAPI();
  delay(120000);
  Serial.println("End");
}

void loop()
{
  
}   
