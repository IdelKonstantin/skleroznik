// For a connection via I2C using the Arduino Wire include:
#include <Wire.h>               
#include "HT_SSD1306Wire.h"
#include "images.h"

class OLEDprinter {

private:
  SSD1306Wire  display{0x3c, 500000, SDA_OLED, SCL_OLED, GEOMETRY_128_64, RST_OLED}; // addr , freq , i2c group , resolution , rst

public:
  OLEDprinter() {};

  bool init() {
    return display.init();
  }

  void printMain() {

    display.clear();
    display.setFont(ArialMT_Plain_10);

    display.drawString(0, 0, "12-11-2024");
    display.drawString(0, 10, "12:04:34 UTC");
    display.drawString(72, 0, "55.78559 N");
    display.drawString(72, 10, "37.62629 E");
    display.drawString(0, 25, "kmh:");
    display.drawString(60, 25, "rpm:");
    display.drawString(0, 40, "bar:");
    display.drawString(60, 40, "mm:");
    //display.drawString(0, 50, "READY...");
    display.drawString(0, 50, ">> 20220804_043601.log");
    
    display.setFont(ArialMT_Plain_16);
    display.drawString(22, 20, "120");
    display.drawString(82, 20, "12000");
    display.drawString(22, 35, "9.6");
    display.drawString(82, 35, "320");
    display.display();
  }

  void printInitInfo() {

    const char* OK = "OK";
    const char* NA = "NA";
    const char* OKNA = "OK/NA";
    const char* Version = "1.0.0.1";

    display.clear();
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 0, "Firmware verion");
    display.drawString(0, 10, "Meteo sensor>");
    display.drawString(0, 20, "Accel/Gyro>");
    display.drawString(0, 30, "Suspension>");
    display.drawString(0, 40, "Breaks>");
    display.drawString(0, 50, "Ignition sensor>");

    display.drawString(90, 0, Version);
    display.drawString(90, 10, OK);
    display.drawString(90, 20, NA);
    display.drawString(90, 30, OKNA);
    display.drawString(90, 40, OKNA);
    display.drawString(90, 50, OK);  

    display.display();

    delay(2000);
    display.clear();
    display.drawString(0, 0, "Used space, KB: 30.1");
    display.drawString(0, 10, "Free space: 1300.1");
    display.drawString(0, 20, "Awailable time, min: 42");
    display.display();
  }

  void printWifi() {

    display.clear();
    display.drawXbm(0, 0, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 40, "SSID/PASS: TLM-440072");
    display.drawString(0, 50, "FTP: 192.168.2.1");
    display.display();
  }
};

OLEDprinter oled;

void setup() {

  Serial.begin(115200);

  if(!oled.init()) {
    Serial.println("Oled is not inited");
    while(1){};
  }
  Serial.println("Oled is inited");
  
  oled.printInitInfo();
  delay(10000);
  oled.printMain();
  delay(10000);
  oled.printWifi();
  delay(5000);
}

void loop() {
  //Serial.println(readVCC());
  delay(100);
}
