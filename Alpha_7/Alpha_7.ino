#include "./inc/key_worker.h"
#include "./inc/BC_windows.h"
#include "./inc/TFT_worker.h"
#include "./inc/main_window.h"
#include "./inc/settings_keeper.h"
#include "./inc/meteo_module.h"

/****************************** PERIPHERALS ******************************/
Keypad keypad(makeKeymap(key::keys), key::rowPins, key::colPins, KEY_ROWS, KEY_COLS);
TFT_UNIT tft{TFT_CS, TFT_RS, TFT_MOSI, TFT_SCK, TFT_RESET, TFT_MISO};
configKeeper cfgKeeper;
meteoModule meteo;

/*************************************************************************/
void setup() {

	if(!cfgKeeper.init() || (!cfgKeeper.readConfigsAndSetting())) {
		
		Serial.begin(115200);
		Serial.println("No SPIFFS inited/configs found...");
		while(1);
	}

	//Зажечь подсветку, запустить экран
	tft.begin();
	//Подсветка тут :)))) -> написать класс, управлющий PWM

	if(!meteo.init()) {

		//Внимание метеодатчик поврежден -> на экран
	}

	//Впилить FTP!!!!!
	Serial.begin(115200); //А это выпилить!
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