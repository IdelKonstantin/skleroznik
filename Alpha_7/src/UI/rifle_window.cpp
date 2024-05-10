#include "../../inc/keys_names.h"
#include "../../inc/TFT_worker.h"
#include "../../inc/rifle_window.h"
#include "../../inc/marker_director.h"
#include "../../inc/settings_keeper.h"

#include <vector>
#include <Keypad.h>
#include <Arduino.h>

#define RIFLE_INFO_MAX_NUM 15

extern Keypad keypad;
extern TFT_UNIT tft;
extern markerDirector markerPosition;

namespace UI {
    namespace rifle_window {
        
        const uint16_t leftEdgeMargin{14};
        const uint16_t rightEdgeMargin{20};
        const uint16_t topMargin{15};
        const uint16_t fontVSize{18};
        const uint16_t fontHSize{12};

        std::vector<String> rifleInfo{RIFLE_INFO_MAX_NUM};
    }
}

void UI::rifle_window::setup() {
    
    tft.setRotation(2);
    tft.fillScreen(ILI9341_BLACK);
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    tft.setTextSize(2);
}

void UI::rifle_window::drawBody() {

    auto file = SPIFFS.open(RIFLES_DATAFILE, "r");
    StaticJsonDocument<RIFLE_BUFF_LENGTH> doc;
    deserializeJson(doc, file);
    file.close();

    auto riflesList = doc["rifles"];
    auto index = markerPosition.getMarkerIndex();

    rifleInfo.clear();

    String NameShort = String("Name> ") + riflesList[index]["name"].as<String>();

    rifleInfo.push_back(NameShort.substring(0,18));
    rifleInfo.push_back(String("Scope Hgt.,sm= ") + riflesList[index]["scope_hight"].as<String>());
    rifleInfo.push_back(String("Twist,mm= ") + riflesList[index]["twist"].as<String>());
    rifleInfo.push_back(String("Twist Dir.> ") + riflesList[index]["twist_dir."].as<String>());
    rifleInfo.push_back(String("Scope units> ") + riflesList[index]["scope_units"].as<String>());
    rifleInfo.push_back(String("Vert. click= ") + riflesList[index]["vert.click"].as<String>());
    rifleInfo.push_back(String("Horiz.click= ") + riflesList[index]["horiz.click"].as<String>());
    rifleInfo.push_back(String("Zeroing,m= ") + riflesList[index]["zero_dist."].as<String>());
    rifleInfo.push_back(String("Zero athm.> ") + riflesList[index]["zeroing"].as<String>());
    rifleInfo.push_back(String("Zero T.,C= ") + riflesList[index]["zero_T"].as<String>());
    rifleInfo.push_back(String("Zero P.,hPa= ") + riflesList[index]["zero_P"].as<String>());

    rifleInfo.push_back(String("V/Drift,sm= ") + riflesList[index]["zero_vert.drift"].as<String>());
    rifleInfo.push_back(String("V/Dr.Dir.> ") + riflesList[index]["zero_vert.drify.dir."].as<String>());
    rifleInfo.push_back(String("H/Drift,sm= ") + riflesList[index]["zero_horiz.drift"].as<String>());
    rifleInfo.push_back(String("H/Dr.Dir.> ") + riflesList[index]["zero_horiz.drift.dir"].as<String>());

    for(int i = 0; i < UI::rifle_window::rifleInfo.size(); i++) {

        tft.setCursor(UI::rifle_window::leftEdgeMargin, 
            UI::rifle_window::topMargin + i * UI::rifle_window::fontVSize);
        tft.println(UI::rifle_window::rifleInfo.at(i));
    }
}

void UI::rifle_window::drawCanvas() {
    
    tft.fillRect(0, 296, 240, 32, ILI9341_RED);
    
    tft.setTextSize(2);
    tft.setTextColor(ILI9341_BLACK, ILI9341_RED);

    tft.setCursor(166, 300);
    tft.println(F("RETURN"));

    tft.setTextSize(2);
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
}

void UI::rifle_window::worker() {

    while(1) {
        if (keypad.getKey() == RIGHT_KEY) {
            break;
        }
    }
}