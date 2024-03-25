#include <Wire.h>  
#include "HT_SSD1306Wire.h"
#include "driver/adc.h"

// class OLEPprinter {

// private:

//   SSD1306Wire  display{0x3c, 500000, SDA_OLED, SCL_OLED, GEOMETRY_128_64, RST_OLED};

// public:

//   OLEPprinter() {

//     pinMode(Vext,OUTPUT);
//     digitalWrite(Vext, LOW);
//     delay(100);

//     display.init();
//     display.clear();

//     display.setContrast(255);
//     display.setTextAlignment(TEXT_ALIGN_LEFT);
//     display.setFont(ArialMT_Plain_10);
//   };

//   void printSmth() {

//     display.drawString(0, 0, "Hello world");
//     display.display();
//   }
// };

// OLEPprinter printer;

// void setup() {
//   printer.printSmth();
// }

// void loop() { }

SSD1306Wire  display{0x3c, 500000, SDA_OLED, SCL_OLED, GEOMETRY_128_64, RST_OLED};

void VextON() {

  pinMode(Vext,OUTPUT);
  digitalWrite(Vext, LOW); 
}

void setup() {

  Serial.begin(115200);
  Serial.println();
  Serial.println();
  VextON();
  delay(100);

  // Initialising the UI will init the display too.
  display.init();

  display.setFont(ArialMT_Plain_10);  
}


void loop() {

  display.clear();
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.drawString(10, 128, String(millis()));
  display.display();
}
