#include "meteo_module.h"

meteoModule meteo;

void setup()
{
    meteo.init();
    Serial.begin(115200);
}

void loop()
{
  bc::meteoData meteoInfo; 
  
  meteoInfo = meteo.getMeasurements();

//  Serial.println(meteoInfo.T);
//  Serial.println(meteoInfo.P);
//  Serial.println(meteoInfo.H);
  Serial.println(meteoInfo.W);
  
  delay(10);
}   
