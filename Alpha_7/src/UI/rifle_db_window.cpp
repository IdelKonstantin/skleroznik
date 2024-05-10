#include "../../inc/keys_names.h"
#include "../../inc/TFT_worker.h"
#include "../../inc/marker_director.h"
#include "../../inc/BC_windows.h"
#include "../../inc/rifle_db_window.h"
#include "../../inc/rifle_window.h"
#include "../../inc/settings_keeper.h"

#include <vector>
#include <Keypad.h>
#include <Arduino.h>

#define RIFLE_MAX_NUM 10

extern Keypad keypad;
extern TFT_UNIT tft;
extern markerDirector markerPosition;
extern configKeeper cfgKeeper;

namespace UI {
    namespace rifle_db_window {
        
        const uint16_t leftEdgeMargin{20};
        const uint16_t rightEdgeMargin{20};
        const uint16_t topMargin{54};
        const uint16_t fontVSize{18};
        const uint16_t fontHSize{12};

        std::vector<String> rifleNames{RIFLE_MAX_NUM};
    }
}

void UI::rifle_db_window::setup() {
    
    tft.setRotation(2);
    tft.fillScreen(ILI9341_BLACK);
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
}

void UI::rifle_db_window::drawBody() {
    
    tft.setTextSize(3);
    tft.setCursor(66, 15);
    tft.println(F("RIFLES"));
    tft.setTextSize(2);

    rifleNames.clear();

    auto file = SPIFFS.open(RIFLES_DATAFILE, "r");
    StaticJsonDocument<RIFLE_BUFF_LENGTH> doc;
    deserializeJson(doc, file);
    file.close();

    auto rifleList = doc["rifles"];

    for(int i = 0; i < rifleList.size(); i++) {
        rifleNames.push_back((rifleList[i]["name"].as<String>()));
    }

    for(int i = 0; i < UI::rifle_db_window::rifleNames.size(); i++) {

        tft.setCursor(UI::rifle_db_window::leftEdgeMargin, 
            UI::rifle_db_window::topMargin + i * UI::rifle_db_window::fontVSize);
        tft.println(UI::rifle_db_window::rifleNames.at(i));
    }
}

void UI::rifle_db_window::drawCanvas() {
    
    tft.fillRect(0, 296, 240, 32, ILI9341_RED);
    
    tft.setTextSize(2);
    tft.setTextColor(ILI9341_BLACK, ILI9341_RED);
    tft.setCursor(2, 300);
    tft.println(F("SHOW"));

    tft.setCursor(166, 300);
    tft.println(F("RETURN"));

    tft.setTextSize(2);
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
}

void UI::rifle_db_window::drawMarker() {
    
    markerPosition.setMarkerData(
        cfgKeeper.rifle.index, 
        UI::rifle_db_window::rifleNames.size(), 
        UI::rifle_db_window::fontVSize, 
        (HORIZONTAL_RESOLUTION - (UI::rifle_db_window::rightEdgeMargin + 
            UI::rifle_db_window::fontHSize)),
        UI::rifle_db_window::topMargin
    );    
}

namespace UI {
    namespace rifle_db_window {
    
        void printRifleNameRead() {
            
            tft.setTextColor(ILI9341_RED, ILI9341_BLACK);

            tft.setCursor(UI::rifle_db_window::leftEdgeMargin, 
            UI::rifle_db_window::topMargin + 
            markerPosition.getMarkerIndex() * UI::rifle_db_window::fontVSize);
            
            tft.println(UI::rifle_db_window::rifleNames.at(markerPosition.getMarkerIndex()));
            delay(500);
        }

        void redrawWindow() {
            UI::rifle_db_window::setup();
            UI::rifle_db_window::drawBody();
            UI::rifle_db_window::drawCanvas();
            markerPosition.drawMarker();
        }
    }
}

void UI::rifle_db_window::worker() {

    while(1) {

        auto key = keypad.getKey();

        if (key == RIGHT_KEY) {
            break;

        } else if (key == OK_KEY) {
            UI::rifle_db_window::printRifleNameRead();
            cfgKeeper.selectRifleWithIndex(markerPosition.getMarkerIndex());
            break;

        } else if (key == LEFT_KEY) {
            UI::rifle_db_window::printRifleNameRead();
            UIwindow::builder::makeWindow()
                .setSetup(UI::rifle_window::setup)
                .setDrawBody(UI::rifle_window::drawBody)
                .setDrawCanvas(UI::rifle_window::drawCanvas)
                .setWorker(UI::rifle_window::worker)
                .build()
            .start();
            UI::rifle_db_window::redrawWindow();

        } else if (key == UP_KEY) {
            markerPosition.upMarker();

        } else if (key == DOWN_KEY) {
            markerPosition. downMarker();
        }
    }
}