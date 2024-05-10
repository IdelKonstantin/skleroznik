#include "../../inc/keys_names.h"
#include "../../inc/TFT_worker.h"
#include "../../inc/bullet_window.h"
#include "../../inc/marker_director.h"
#include "../../inc/settings_keeper.h"

#include <vector>
#include <Keypad.h>
#include <Arduino.h>

#define BULLET_INFO_MAX_NUM 12

extern Keypad keypad;
extern TFT_UNIT tft;
extern markerDirector markerPosition;

namespace UI {
    namespace bullet_window {
        
        const uint16_t leftEdgeMargin{20};
        const uint16_t rightEdgeMargin{20};
        const uint16_t topMargin{15};
        const uint16_t fontVSize{18};
        const uint16_t fontHSize{12};

        std::vector<String> bulletInfo{BULLET_INFO_MAX_NUM};
    }
}

void UI::bullet_window::setup() {
    
    tft.setRotation(2);
    tft.fillScreen(ILI9341_BLACK);
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    tft.setTextSize(2);
}

void UI::bullet_window::drawBody() {

    auto file = SPIFFS.open(BULLETS_DATAFILE, "r");
    StaticJsonDocument<BULLET_BUFF_LENGTH> doc;
    deserializeJson(doc, file);
    file.close();

    auto bullList = doc["bullets"];
    auto index = markerPosition.getMarkerIndex();

    bulletInfo.clear();

    String NameShort = String("Name> ") + bullList[index]["name"].as<String>();

    bulletInfo.push_back(NameShort.substring(0,17));
    bulletInfo.push_back(String("Drag Func.> ") + bullList[index]["DF"].as<String>());
    bulletInfo.push_back(String("BC= ") + bullList[index]["BC"].as<String>());
    bulletInfo.push_back(String("V0,m/s= ") + bullList[index]["MV"].as<String>());
    bulletInfo.push_back(String("Leng.,mm= ") + bullList[index]["length"].as<String>());
    bulletInfo.push_back(String("Weig.,gr= ") + bullList[index]["weight"].as<String>());
    bulletInfo.push_back(String("Diam.,mm= ") + bullList[index]["caliber"].as<String>());
    bulletInfo.push_back(String("Therm.sens,%= ") + bullList[index]["therm.sens."].as<String>());
    bulletInfo.push_back(String("Temper.V0,C= ") + bullList[index]["MV_temp."].as<String>());
    bulletInfo.push_back(String("CF at 0.9M= ") + bullList[index]["CF_M0.9"].as<String>());
    bulletInfo.push_back(String("CF at 1.0M= ") + bullList[index]["CF_M1.0"].as<String>());
    bulletInfo.push_back(String("CF at 1.1M= ") + bullList[index]["CF_M1.1"].as<String>());

    for(int i = 0; i < UI::bullet_window::bulletInfo.size(); i++) {

        tft.setCursor(UI::bullet_window::leftEdgeMargin, 
            UI::bullet_window::topMargin + i * UI::bullet_window::fontVSize);
        tft.println(UI::bullet_window::bulletInfo.at(i));
    }
}

void UI::bullet_window::drawCanvas() {
    
    tft.fillRect(0, 296, 240, 32, ILI9341_RED);
    
    tft.setTextSize(2);
    tft.setTextColor(ILI9341_BLACK, ILI9341_RED);

    tft.setCursor(166, 300);
    tft.println(F("RETURN"));

    tft.setTextSize(2);
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
}

void UI::bullet_window::worker() {

    while(1) {
        if (keypad.getKey() == RIGHT_KEY) {
            break;
        }
    }
}