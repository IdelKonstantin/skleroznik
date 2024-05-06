#include "../../inc/main_window.h"
#include "../../inc/keys_names.h"
#include "../../inc/meteo_module.h"

#include <Keypad.h>
#include <Arduino.h>

extern Keypad keypad;
extern meteoModule meteo;

void UI::main_window::setup() {
    Serial.println("UI::main_window::setup");
}

void UI::main_window::drawHead() {
    Serial.println("UI::main_window::drawHead");
}

void UI::main_window::drawBody() {
    Serial.println("UI::main_window::drawBody");
}

void UI::main_window::drawTail() {
    Serial.println("UI::main_window::drawTail");
}

void UI::main_window::drawCanvas() {
    Serial.println("UI::main_window::drawCanvas");
}

void UI::main_window::worker() {
    
    Serial.println("UI::main_window::worker...");
    
    while(1) {
        
        auto key = keypad.getKey();
        auto meteoInfo = meteo.getMeasurements();

        Serial.println(meteoInfo.W, 1);

        if (key == LEFT_KEY) {
            Serial.println("UI::main_window::worker::LEFT");
        }

        if (key == RIGHT_KEY) {
            Serial.println("UI::main_window::worker::RIGHT");
        }

        if (key == UP_KEY) {
            
            Serial.println("UI::main_window::worker::UP");
        }    

        if (key == DOWN_KEY) {
            Serial.println("UI::main_window::worker::DOWN");
        }

        if (key == OK_KEY) {
            Serial.println("UI::main_window::worker::OK");
        }
    }
}