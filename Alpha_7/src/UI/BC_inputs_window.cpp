#include "../../inc/keys_names.h"
#include "../../inc/TFT_worker.h"
#include "../../inc/input_utils.h"
#include "../../inc/marker_director.h"
#include "../../inc/BC_windows.h"
#include "../../inc/BC_input_window.h"
#include "../../inc/settings_keeper.h"

#include <vector>
#include <Keypad.h>
#include <Arduino.h>

extern Keypad keypad;
extern TFT_UNIT tft;
extern markerDirector markerPosition;
extern configKeeper cfgKeeper;

namespace UI {
    namespace BC_input_window {
        
        const uint16_t leftEdgeMargin{8};
        const uint16_t rightEdgeMargin{8};
        const uint16_t topMargin{54};
        const uint16_t fontVSize{18};
        const uint16_t fontHSize{12};

        const std::vector<String> inputNames {

            "Temper.,C=",
            "Press.,hPa=",
            "Humid.,%=",
            "Distance,m=",
            "Inclin.,deg=",
            "Windage,m/s=",
            "Wind dir,h>",
            "Speed,mil/s=",
            "Azimuth,deg=",
            "Rangecard>",
            "Koriolis>",
            "Windjump>",
            "Therm.corr.>"
        };
    }    
}

void UI::BC_input_window::setup() {
    
    tft.setRotation(2);
    tft.fillScreen(ILI9341_BLACK);

    //TODO: Русский шрифт!!! tft.setFont(&font_rus);

    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    tft.setTextSize(2);
}

void UI::BC_input_window::drawBody() {
    
    String topText;
    tft.setCursor(UI::BC_input_window::leftEdgeMargin, 2);
    topText = "Bullet>";
    topText += cfgKeeper.bullet.name;
    tft.println(topText);

    tft.setCursor(UI::BC_input_window::leftEdgeMargin, 2 + UI::BC_input_window::fontVSize);
    topText = "Rifle>";
    topText += cfgKeeper.rifle.name;
    tft.println(topText);

    tft.drawLine(2, 42, 238, 42, ILI9341_WHITE);
    tft.drawLine(2, 44, 238, 44, ILI9341_WHITE);

    for(int i = 0; i < UI::BC_input_window::inputNames.size(); i++) {

        tft.setCursor(UI::BC_input_window::leftEdgeMargin, 
            UI::BC_input_window::topMargin + 
            i * UI::BC_input_window::fontVSize);

        tft.println(UI::BC_input_window::inputNames.at(i));
    }
}

void UI::BC_input_window::drawCanvas() {
    
    tft.fillRect(0, 296, 240, 32, ILI9341_RED);
    
    tft.setTextSize(2);
    tft.setTextColor(ILI9341_BLACK, ILI9341_RED);
    tft.setCursor(2, 300);
    tft.println(F("RUN"));

    tft.setCursor(166, 300);
    tft.println(F("RETURN"));

    tft.setTextSize(2);
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
}

void UI::BC_input_window::drawMarker() {
    
    markerPosition.setMarkerData(
        3, 
        UI::BC_input_window::inputNames.size(), 
        UI::BC_input_window::fontVSize, 
        (HORIZONTAL_RESOLUTION - (UI::BC_input_window::rightEdgeMargin + 
            UI::BC_input_window::fontHSize)),
        UI::BC_input_window::topMargin
    );    
}

void UI::BC_input_window::worker() {

    while(1) {

        auto key = keypad.getKey();

        if (key == RIGHT_KEY) {
            break;

        } else if (key == LEFT_KEY) {
        

        } else if (key == OK_KEY) {


        } else if (key == UP_KEY) {
            markerPosition.upMarker();

        } else if (key == DOWN_KEY) {
            markerPosition. downMarker();
        }
    }
}