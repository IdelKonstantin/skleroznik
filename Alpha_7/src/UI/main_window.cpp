#include "../../inc/main_window.h"
#include "../../inc/keys_names.h"
#include "../../inc/meteo_module.h"
#include "../../inc/TFT_worker.h"

#include <Keypad.h>
#include <Arduino.h>

extern Keypad keypad;
extern meteoModule meteo;
extern TFT_UNIT tft;

void UI::main_window::setup() {
    
    tft.setRotation(2);
    tft.fillScreen(ILI9341_BLACK);
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    tft.setTextSize(2);

    //Инициировать структуры решателя здесь
}

void UI::main_window::drawHead() {
    //T, P, H, W, Wdir, BAT
}

void UI::main_window::drawBody() {
    //BULL name, RIFLE name, SCOPE name STATIC
}

void UI::main_window::drawTail() {
    //DIST ANGLE STATIC
}

void UI::main_window::drawCanvas() {
    //tft.fillRect(2, 302, 236, 16, ILI9341_RED); STATIC
}

void UI::main_window::worker() {

    bool updateBC{true};
    
    while(1) {
        
        auto key = keypad.getKey();
        auto meteoInfo = meteo.getMeasurements();

        if (key == LEFT_KEY) {
            //BC
        }

        if (key == RIGHT_KEY) {
            //Data
        }

        if (key == UP_KEY) {
            //Поворот ветра по часовой update Wdir
        }    

        if (key == DOWN_KEY) {
            //Поворот ветра против часовой update Wdir
        }

        if (key == OK_KEY) {
            updateBC = !updateBC;
        }

        if(updateBC) {
            
            Serial.println(meteoInfo.W, 1);

            //Пересчитать поправки
            //Отрисовать на экране
        }

        //Обновить метеоданные для показа
    }
}