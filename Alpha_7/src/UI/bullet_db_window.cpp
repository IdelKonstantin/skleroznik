#include "../../inc/keys_names.h"
#include "../../inc/TFT_worker.h"
#include "../../inc/marker_director.h"
#include "../../inc/BC_windows.h"
#include "../../inc/bullet_db_window.h"
#include "../../inc/bullet_window.h"
#include "../../inc/settings_keeper.h"

#include <vector>
#include <Keypad.h>
#include <Arduino.h>

#define BULLET_MAX_NUM 10

extern Keypad keypad;
extern TFT_UNIT tft;
extern markerDirector markerPosition;
extern configKeeper cfgKeeper;

namespace UI {
    namespace bullet_db_window {
        
        const uint16_t leftEdgeMargin{20};
        const uint16_t rightEdgeMargin{20};
        const uint16_t topMargin{54};
        const uint16_t fontVSize{18};
        const uint16_t fontHSize{12};

        std::vector<String> bulletNames{BULLET_MAX_NUM};
    }
}

void UI::bullet_db_window::setup() {
    
    tft.setRotation(2);
    tft.fillScreen(ILI9341_BLACK);
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
}

void UI::bullet_db_window::drawBody() {
    
    tft.setTextSize(3);
    tft.setCursor(57, 15);
    tft.println(F("BULLETS"));
    tft.setTextSize(2);

    bulletNames.clear();

    auto file = SPIFFS.open(BULLETS_DATAFILE, "r");
    StaticJsonDocument<BULLET_BUFF_LENGTH> doc;
    deserializeJson(doc, file);
    file.close();

    auto bullList = doc["bullets"];

    for(int i = 0; i < bullList.size(); i++) {
        bulletNames.push_back((bullList[i]["name"].as<String>()));
    }

    for(int i = 0; i < UI::bullet_db_window::bulletNames.size(); i++) {

        tft.setCursor(UI::bullet_db_window::leftEdgeMargin, 
            UI::bullet_db_window::topMargin + i * UI::bullet_db_window::fontVSize);
        tft.println(UI::bullet_db_window::bulletNames.at(i));
    }
}

void UI::bullet_db_window::drawCanvas() {
    
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

void UI::bullet_db_window::drawMarker() {
    
    markerPosition.setMarkerData(
        cfgKeeper.bullet.index, 
        UI::bullet_db_window::bulletNames.size(), 
        UI::bullet_db_window::fontVSize, 
        (HORIZONTAL_RESOLUTION - (UI::bullet_db_window::rightEdgeMargin + 
            UI::bullet_db_window::fontHSize)),
        UI::bullet_db_window::topMargin
    );    
}

namespace UI {
    namespace bullet_db_window {
    
        void printBulletNameRead() {
            
            tft.setTextColor(ILI9341_RED, ILI9341_BLACK);

            tft.setCursor(UI::bullet_db_window::leftEdgeMargin, 
            UI::bullet_db_window::topMargin + 
            markerPosition.getMarkerIndex() * UI::bullet_db_window::fontVSize);
            
            tft.println(UI::bullet_db_window::bulletNames.at(markerPosition.getMarkerIndex()));
            delay(500);
        }

        void redrawWindow() {
            UI::bullet_db_window::setup();
            UI::bullet_db_window::drawBody();
            UI::bullet_db_window::drawCanvas();
            markerPosition.drawMarker();
        }
    }
}

void UI::bullet_db_window::worker() {

    while(1) {

        auto key = keypad.getKey();

        if (key == RIGHT_KEY) {
            break;

        } else if (key == OK_KEY) {
            UI::bullet_db_window::printBulletNameRead();
            cfgKeeper.selectBulletWithIndex(markerPosition.getMarkerIndex());
            break;

        } else if (key == LEFT_KEY) {
            UI::bullet_db_window::printBulletNameRead();    
            UIwindow::builder::makeWindow()
                .setSetup(UI::bullet_window::setup)
                .setDrawBody(UI::bullet_window::drawBody)
                .setDrawCanvas(UI::bullet_window::drawCanvas)
                .setWorker(UI::bullet_window::worker)
                .build()
            .start();
            UI::bullet_db_window::redrawWindow();

        } else if (key == UP_KEY) {
            markerPosition.upMarker();

        } else if (key == DOWN_KEY) {
            markerPosition. downMarker();
        }
    }
}