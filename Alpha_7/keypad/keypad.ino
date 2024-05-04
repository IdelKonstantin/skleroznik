#include "key_worker.h"

Keypad keypad(makeKeymap(key::keys), key::rowPins, key::colPins, KEY_ROWS, KEY_COLS);

void setup() {

  Serial.begin(115200);
  delay(1000);
  Serial.println("Press any key...");
}

void loop() {
  // put your main code here, to run repeatedly:

  auto key = keypad.getKey();

  if (key == 'L') {
    Serial.println("LEFT");
  }
  else if (key == 'R'){
   Serial.println("RIGHT");
  }
  else if (key == 'U'){
   Serial.println("UPAPAP");
  }
  else if (key == 'D'){
   Serial.println("DOWN");
  }
  else if (key == 'O'){
   Serial.println("OK");
  }    
}
