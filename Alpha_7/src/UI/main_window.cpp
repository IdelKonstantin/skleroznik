#include "../../inc/main_window.h"
#include "../../inc/keys_names.h"
#include "../../inc/meteo_module.h"
#include "../../inc/TFT_worker.h"
#include "../../inc/settings_keeper.h"
#include "../../inc/input_utils.h"
#include "../../inc/trajectory_solver_API.h"

#include <Keypad.h>
#include <Arduino.h>

extern Keypad keypad;
extern meteoModule meteo;
extern TFT_UNIT tft;
extern configKeeper cfgKeeper;

extern Bullet g_bullet;
extern Rifle g_rifle;
extern Scope g_scope;
extern Inputs g_inputs;
extern Options g_options;
extern Results g_results;

#define DUMMY_NAME ""
#define DUMMY_DATA 0
#define DUMMY_PTR nullptr

void UI::main_window::setup() {
    
    tft.setRotation(2);
    tft.fillScreen(ILI9341_BLACK);

    //TODO: Русский шрифт!!! tft.setFont(&font_rus);

    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    tft.setTextSize(2);

    g_bullet = Bullet {
        DUMMY_NAME,
        cfgKeeper.bullet.DF,
        cfgKeeper.bullet.BC,
        cfgKeeper.bullet.CFM0_9,
        cfgKeeper.bullet.CFM1_0,
        cfgKeeper.bullet.CFM1_1,
        cfgKeeper.bullet.MV,
        cfgKeeper.bullet.length,
        cfgKeeper.bullet.weight,
        cfgKeeper.bullet.caliber,
        cfgKeeper.bullet.MVtemp,
        cfgKeeper.bullet.thermSens,
        DUMMY_PTR
    };

    g_rifle = Rifle {
        DUMMY_NAME,
        cfgKeeper.rifle.zeroDist,
        cfgKeeper.rifle.scopeHight,
        cfgKeeper.rifle.twist,
        cfgKeeper.rifle.twistDir,
        cfgKeeper.rifle.zeroingAt,
        cfgKeeper.rifle.zeroT,
        cfgKeeper.rifle.zeroP,
        cfgKeeper.rifle.zeroVertDrift,
        cfgKeeper.rifle.zeroVertDriftDir,
        cfgKeeper.rifle.zeroHorizDrift,
        cfgKeeper.rifle.zeroHorizDriftDir,
        DUMMY_DATA
    };

    g_scope = Scope {
        DUMMY_NAME,
        cfgKeeper.rifle.scopeUnits,
        cfgKeeper.rifle.vertClick,
        cfgKeeper.rifle.horizClick,
        DUMMY_DATA
    };

    g_inputs = Inputs {
        cfgKeeper.target.distance,
        cfgKeeper.target.terrainAngle,
        cfgKeeper.target.speedMILs,
        cfgKeeper.target.azimuth,
        cfgKeeper.settings.latitude,
        cfgKeeper.settings.magneticIncl
    };

    g_options = Options {
        cfgKeeper.inputs.koriolis,
        cfgKeeper.inputs.rangeCard,
        cfgKeeper.inputs.termoCorr,
        cfgKeeper.inputs.aeroJump
    };
}

void UI::main_window::drawHead() {
    tft.setCursor(2, 2);
    tft.println(F("T,C="));
    tft.setCursor(2, 26);
    tft.println(F("P,hPa="));
    tft.setCursor(2, 50);
    tft.println(F("H,%="));
    tft.setCursor(2, 74);
    tft.println(F("W,m/s="));
    tft.drawLine(2, 98, 238, 98, ILI9341_WHITE);
    //tft.drawLine(2, 102, 238, 102, ILI9341_WHITE);

    //TODO: Wdir, BAT
}

void UI::main_window::drawBody() {
    tft.setCursor(2, 104);
    tft.print(F("BUL.> "));
    tft.println(cfgKeeper.bullet.name.substring(0,12));
    tft.setCursor(2, 128);
    tft.print(F("RIFLE> "));
    tft.println(cfgKeeper.rifle.name.substring(0,12));
    tft.setCursor(2, 152);
    tft.print(F("SCOPE> "));
    tft.print((cfgKeeper.rifle.scopeUnits == 1 ? "MOA" : "MRAD"));
    tft.drawLine(2, 176, 238, 176, ILI9341_WHITE);
}

void UI::main_window::drawTail() {
    //DIST ANGLE STATIC
}

void UI::main_window::drawCanvas() {
    tft.fillRect(2, 294, 236, 24, ILI9341_RED);
}

void UI::main_window::worker() {

    bool updateBC{true};

    Serial.begin(115200);

    while(1) {
        
        auto key = keypad.getKey();
        
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
            
            auto meteoInfo = meteo.getMeasurements();

            Serial.println(meteoInfo.W, 1);

            //Пересчитать поправки
            //Отрисовать на экране
        }

        //Обновить метеоданные для показа
    }
}