#include "./inc/key_worker.h"
#include "./inc/BC_windows.h"
#include "./inc/TFT_worker.h"
#include "./inc/main_window.h"
#include "./inc/settings_keeper.h"
#include "./inc/meteo_module.h"
#include "./inc/wind_director.h"
#include "./inc/energy_worker.h"

/****************************** PERIPHERALS ******************************/
Keypad keypad(makeKeymap(key::keys), key::rowPins, key::colPins, KEY_ROWS, KEY_COLS);
TFT_UNIT tft{TFT_CS, TFT_RS, TFT_MOSI, TFT_SCK, TFT_RESET, TFT_MISO};
configKeeper cfgKeeper;
meteoModule meteo;
energyWorker energy;
windDirector windDirection;

/*************************************************************************/
void setup() {

	if(!cfgKeeper.init() || (!cfgKeeper.readConfigsAndSetting())) {
		
		Serial.begin(115200);
		Serial.println("No SPIFFS inited/configs found...");
		while(1);
	}

	tft.begin();
	energy.setBacklightIntencity(cfgKeeper.settings.backlIntencity);

	if(!meteo.init()) {
		
		tft.setRotation(2);
		tft.fillScreen(ILI9341_BLACK);
		tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
		tft.setTextSize(2);
		tft.setCursor(0, 0);
		tft.println("Meteo is damaged...");
		delay(2000);
	}
}

/*************************************************************************/
void loop() {

	UIwindow::builder::makeWindow()
		.setSetup(UI::main_window::setup)
		.setDrawHead(UI::main_window::drawHead)
		.setDrawBody(UI::main_window::drawBody)
		.setDrawTail(UI::main_window::drawTail)
		.setDrawCanvas(UI::main_window::drawCanvas)
		.setWorker(UI::main_window::worker)
		.build()
	.start();
}