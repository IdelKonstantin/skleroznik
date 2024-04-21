#include "BCdata_uploader.h"

void setup()
{
  SPIFFS.begin(false);
  dataUploader{}.handleRESAPI();
  delay(120000);
}

void loop()
{
  
}   
