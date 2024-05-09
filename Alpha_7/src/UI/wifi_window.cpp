#include "../../inc/TFT_worker.h"
#include "../../inc/wifi_window.h"
#include "../../inc/device_info.h"
#include "../../inc/BCdata_uploader.h"


#include <Keypad.h>
#include <Arduino.h>

extern TFT_UNIT tft;

void UI::wifi_window::setup() {
    
    tft.setRotation(2);
    tft.fillScreen(ILI9341_BLACK);
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
}

void UI::wifi_window::drawBody() {
    
    tft.setTextSize(4);
    tft.setCursor(24, 36);
    tft.println(F("WIFI:"));

    tft.setTextSize(2);
    tft.setCursor(24, 90);
    tft.print(F("SSID:"));
    tft.println(chip::info().chipID());

    tft.setCursor(24, 108);
    tft.print(F("PASS:"));
    tft.println(chip::info().chipID());

    tft.setCursor(24, 126);
    tft.print(F("IP:192.168.2.1"));
}

void UI::wifi_window::drawCanvas() {
    
    tft.fillRect(0, 296, 240, 32, ILI9341_RED);
    
    tft.setTextSize(2);
    tft.setTextColor(ILI9341_BLACK, ILI9341_RED);

    tft.setCursor(166, 300);
    tft.println(F("RETURN"));

    tft.setTextSize(2);
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
}

void UI::wifi_window::worker() {
    dataUploader{}.handleRESTAPI();
}