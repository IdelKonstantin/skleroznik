#include <Keypad.h>

#include "./inc/some.h"

const byte ROWS = 5;
const byte COLS = 1;
char keys[ROWS][COLS] = {
  {'L'},
  {'R'},
  {'U'},
  {'D'},
  {'O'}
};

byte rowPins[ROWS] = {27, 16, 17, 25, 26};
byte colPins[COLS] = {14};

auto keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup() {
  // put your setup code here, to run once:
  dummy::init();
}

void loop() {
  // put your main code here, to run repeatedly:
  
  auto key = keypad.getKey();

  if(key != NO_KEY) {
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
}
