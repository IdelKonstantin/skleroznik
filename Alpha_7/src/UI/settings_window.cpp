#include "../../inc/keys_names.h"
#include "../../inc/TFT_worker.h"
#include "../../inc/input_utils.h"
#include "../../inc/marker_director.h"
#include "../../inc/BC_windows.h"
#include "../../inc/settings_window.h"
#include "../../inc/about_window.h"
#include "../../inc/wifi_window.h"
#include "../../inc/bullet_db_window.h"

#include <vector>
#include <Keypad.h>
#include <Arduino.h>

extern Keypad keypad;
extern TFT_UNIT tft;
extern markerDirector markerPosition;

namespace UI {
    namespace settings_window {
        
        const uint16_t leftEdgeMargin{20};
        const uint16_t rightEdgeMargin{20};
        const uint16_t topMargin{54};
        const uint16_t fontVSize{18};
        const uint16_t fontHSize{12};
    
        const std::vector<String> menuNames {

            "Bullet database",
            "Rifle and Scope",
            "Mildot formula",
            "Range Card",
            "Settings",
            "Wireless upload",
            "About device"
        };
    }

    enum {

        BULLET_DB,
        RIFLE_DB,
        MILDOT,
        RANGECARD,
        SETTINGS,
        WIFI,
        ABOUT
    };
}

void UI::settings_window::setup() {
    
    tft.setRotation(2);
    tft.fillScreen(ILI9341_BLACK);

    //TODO: Русский шрифт!!! tft.setFont(&font_rus);

    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    tft.setTextSize(2);
}

void UI::settings_window::drawBody() {
    
    for(int i = 0; i < UI::settings_window::menuNames.size(); i++) {

        tft.setCursor(UI::settings_window::leftEdgeMargin, 
            UI::settings_window::topMargin + 
            i * UI::settings_window::fontVSize);

        tft.println(UI::settings_window::menuNames.at(i));
    }
}

void UI::settings_window::drawCanvas() {
    
    tft.fillRect(0, 296, 240, 32, ILI9341_RED);
    
    tft.setTextSize(2);
    tft.setTextColor(ILI9341_BLACK, ILI9341_RED);
    tft.setCursor(2, 300);
    tft.println(F("SELECT"));

    tft.setCursor(166, 300);
    tft.println(F("RETURN"));

    tft.setTextSize(2);
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
}

void UI::settings_window::drawMarker() {
    
    markerPosition.setMarkerData(
        0, 
        UI::settings_window::menuNames.size(), 
        UI::settings_window::fontVSize, 
        (HORIZONTAL_RESOLUTION - (UI::settings_window::rightEdgeMargin + 
            UI::settings_window::fontHSize)),
        UI::settings_window::topMargin
    );    
}

namespace UI {
    namespace settings_window {
    
        void printMenuLineRead() {
            
            tft.setTextColor(ILI9341_RED, ILI9341_BLACK);

            tft.setCursor(UI::settings_window::leftEdgeMargin, 
            UI::settings_window::topMargin + 
            markerPosition.getMarkerIndex() * UI::settings_window::fontVSize);
            
            tft.println(UI::settings_window::menuNames.at(markerPosition.getMarkerIndex()));
            delay(500);
        }

        void redrawWindowAndMarker(int16_t index) {
            markerPosition.setMarkerIndex(index);
            markerPosition.setMarkerMaxLines(UI::settings_window::menuNames.size());
            UI::settings_window::setup();
            UI::settings_window::drawBody();
            UI::settings_window::drawCanvas();
            markerPosition.drawMarker();
        }
    }
}

void UI::settings_window::worker() {

    while(1) {

        auto key = keypad.getKey();

        if (key == RIGHT_KEY) {
            break;

        } else if (key == LEFT_KEY || key == OK_KEY) {
            
            UI::settings_window::printMenuLineRead();

            switch(markerPosition.getMarkerIndex()) {

                case BULLET_DB:
                    UIwindow::builder::makeWindow()
                        .setSetup(UI::bullet_db_window::setup)
                        .setDrawBody(UI::bullet_db_window::drawBody)
                        .setDrawCanvas(UI::bullet_db_window::drawCanvas)
                        .setMarkers(UI::bullet_db_window::drawMarker)
                        .setWorker(UI::bullet_db_window::worker)
                        .build()
                    .start();
                    UI::settings_window::redrawWindowAndMarker(BULLET_DB);
                    break;

                case RIFLE_DB: 
                    //
                    UI::settings_window::redrawWindowAndMarker(RIFLE_DB);
                    break;

                case MILDOT:
                    //
                    UI::settings_window::redrawWindowAndMarker(MILDOT);
                    break;

                case RANGECARD:
                    //
                    UI::settings_window::redrawWindowAndMarker(RANGECARD);
                    break;

                case SETTINGS:
                    //
                    UI::settings_window::redrawWindowAndMarker(SETTINGS);
                    break;

                case WIFI:
                    UIwindow::builder::makeWindow()
                        .setSetup(UI::wifi_window::setup)
                        .setDrawBody(UI::wifi_window::drawBody)
                        .setDrawCanvas(UI::wifi_window::drawCanvas)
                        .setWorker(UI::wifi_window::worker)
                        .build()
                    .start();
                    UI::settings_window::redrawWindowAndMarker(WIFI);
                    break;

                case ABOUT:
                    UIwindow::builder::makeWindow()
                        .setSetup(UI::about_window::setup)
                        .setDrawBody(UI::about_window::drawBody)
                        .setDrawCanvas(UI::about_window::drawCanvas)
                        .setWorker(UI::about_window::worker)
                        .build()
                    .start();
                    UI::settings_window::redrawWindowAndMarker(ABOUT);
                    break;
            }

        } else if (key == UP_KEY) {
            markerPosition.upMarker();

        } else if (key == DOWN_KEY) {
            markerPosition. downMarker();
        }
    }
}