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

  Serial.print(meteoInfo.T, 0);
  Serial.print(" ");
  Serial.print(meteoInfo.P);
  Serial.print(" ");
  Serial.print(meteoInfo.H);
  Serial.print(" ");
  Serial.println(meteoInfo.W, 1);

  delay(100);
}
