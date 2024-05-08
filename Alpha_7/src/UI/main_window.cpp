#include "../../inc/main_window.h"
#include "../../inc/settings_window.h"
#include "../../inc/keys_names.h"
#include "../../inc/meteo_module.h"
#include "../../inc/TFT_worker.h"
#include "../../inc/settings_keeper.h"
#include "../../inc/input_utils.h"
#include "../../inc/wind_director.h"
#include "../../inc/BC_windows.h"

#include "../../inc/trajectory_solver_API.h"
#include "../../inc/trajectory_solver.h"

#include <Keypad.h>
#include <Arduino.h>

extern Keypad keypad;
extern meteoModule meteo;
extern TFT_UNIT tft;
extern configKeeper cfgKeeper;
extern windDirector windDirection;

extern Bullet g_bullet;
extern Rifle g_rifle;
extern Scope g_scope;
extern Meteo g_meteo;
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
        DUMMY_DATA,
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

    //VCC
    tft.setCursor(184, 2);
    tft.println(F("V"));
    tft.drawRoundRect(198, 0, 41, 19, 2, ILI9341_WHITE);

    windDirection.drawClock();

    tft.drawLine(2, 96, 238, 96, ILI9341_WHITE);
    tft.drawLine(2, 98, 238, 98, ILI9341_WHITE);
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

    tft.drawLine(2, 174, 238, 174, ILI9341_WHITE);
    tft.drawLine(2, 176, 238, 176, ILI9341_WHITE);

    tft.setCursor(2, 182);
    tft.print(F("D,m= ")); 
    tft.print(cfgKeeper.target.distance);
    tft.print(F(" TA,dg= "));
    tft.print(cfgKeeper.target.terrainAngle);

    tft.drawLine(2, 206, 238, 206, ILI9341_WHITE);
    tft.drawLine(2, 208, 238, 208, ILI9341_WHITE);
}

void UI::main_window::drawTail() {
    
    tft.setTextSize(3);
    tft.setCursor(40, 214);
    tft.print(F("V="));
    tft.setCursor(40, 238);
    tft.print(F("H="));
    tft.setCursor(40, 262);
    tft.print(F("T="));
}

void UI::main_window::drawCanvas() {
    tft.fillRect(0, 296, 240, 32, ILI9341_RED);
    
    tft.setTextSize(2);
    tft.setTextColor(ILI9341_BLACK, ILI9341_RED);
    tft.setCursor(2, 300);
    tft.println(F("B.C."));

    tft.setCursor(166, 300);
    tft.println(F("EXTRAS"));

    tft.setTextSize(2);
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
}

//////////////////////////////////////////////////////////////////////////////////

namespace UI {
    namespace main_window {

        inline void mainWindowShowResults(const Results& g_results) {

            tft.setTextSize(3);
            tft.setCursor(96, 214);
            tft.print(g_results.vertAngleUnits, 2);
            tft.println(" ");

            tft.setCursor(96, 238);
            tft.print(g_results.horizAngleUnits + g_results.derivAngleUnits, 2);
            tft.println(" ");

            tft.setCursor(96, 262);
            tft.print(g_results.flightTime, 2);
            tft.println(" ");
        };

        inline void mainWindowShowMeteo(const bc::meteoData& meteoInfo) {

            tft.setTextSize(2);
            tft.setCursor(62, 2);
            tft.print(meteoInfo.T, 1);
            tft.println(" ");
            tft.setCursor(86, 26);
            tft.println(meteoInfo.P);
            tft.setCursor(62, 50);
            tft.println(meteoInfo.H);
            tft.setCursor(86, 74);
            tft.println(meteoInfo.W, 1);        
        }

        inline void mainWindowShowVoltage() {

            tft.setCursor(202, 2);
            tft.println(4.2, 1);//TODO analogRead bla-bla        
        }
    }    
}

void UI::main_window::worker() {

    bool updateBC{true};

    while(1) {
        
        auto meteoInfo = meteo.getMeasurements();
        UI::main_window::mainWindowShowMeteo(meteoInfo);
        UI::main_window::mainWindowShowVoltage();
        windDirection.drawHourLine();

        auto key = keypad.getKey();

        if (key == LEFT_KEY) {
            
            //BC
            break;

        } else if (key == RIGHT_KEY) {
            
            UIwindow::builder::makeWindow()
                .setSetup(UI::settings_window::setup)
                .setDrawBody(UI::settings_window::drawBody)
                .setDrawCanvas(UI::settings_window::drawCanvas)
                .setMarkers(UI::settings_window::drawMarker)
                .setWorker(UI::settings_window::worker)
                .build()
            .start();
            break;

        } else if (key == UP_KEY) {
            windDirection.increaseHourAndDrawLine();

        } else if (key == DOWN_KEY) {
            windDirection.decreaseHourAndDrawLine();

        } else if (key == OK_KEY) {
            updateBC = !updateBC;
        }

        if(updateBC) {

            g_meteo = Meteo {
                
                meteoInfo.T,
                meteoInfo.P,
                meteoInfo.H,
                meteoInfo.W,
                windDirection.getWindAngle(),
                DUMMY_DATA,
                SIMPLE_CASE,
                DUMMY_PTR
            };

            trajectorySolver (&g_meteo, &g_bullet, &g_rifle, &g_scope, &g_inputs, &g_options, &g_results);
            UI::main_window::mainWindowShowResults(g_results);
        }
    }
}