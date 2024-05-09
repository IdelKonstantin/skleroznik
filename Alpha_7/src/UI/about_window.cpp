#include "../../inc/keys_names.h"
#include "../../inc/TFT_worker.h"
#include "../../inc/about_window.h"
#include "../../inc/device_info.h"
#include "../../inc/trajectory_solver_routines.h"

#include <Keypad.h>
#include <Arduino.h>

extern Keypad keypad;
extern TFT_UNIT tft;

void UI::about_window::setup() {
    
    tft.setRotation(2);
    tft.fillScreen(ILI9341_BLACK);
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    tft.setTextSize(2);
}

void UI::about_window::drawBody() {
    
    tft.setCursor(66, 36);
    tft.println(F("Ballistic"));
    tft.setCursor(60, 54);
    tft.println(F("Calculator"));
    tft.setCursor(78, 72);
    tft.println(chip::info().deviceName());

    tft.setCursor(42, 108);
    tft.print(F("ID:"));
    tft.println(chip::info().chipID());
    tft.setCursor(54, 126);
    tft.print(F("F/W:"));
    tft.println(chip::info().firmwareVersion());
    tft.setCursor(54, 144);
    tft.print(F("LIB:"));
    tft.println(LIB_VERSION);
    tft.setCursor(60, 162);
    tft.println(chip::info().releaseDate());

    tft.drawRect(38, 30, 164, 156, ILI9341_WHITE);

    tft.setCursor(6, 258);
    tft.println(chip::info().webSite());
}

void UI::about_window::drawCanvas() {
    
    tft.fillRect(0, 296, 240, 32, ILI9341_RED);
    
    tft.setTextSize(2);
    tft.setTextColor(ILI9341_BLACK, ILI9341_RED);

    tft.setCursor(166, 300);
    tft.println(F("RETURN"));

    tft.setTextSize(2);
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
}

void UI::about_window::worker() {

    while(1) {
        if (keypad.getKey() == RIGHT_KEY) {
            break;
        }
    }
}