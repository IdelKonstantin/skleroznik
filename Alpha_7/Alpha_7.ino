#include "./inc/key_worker.h"
#include "./inc/BC_windows.h"
#include "./inc/TFT_worker.h"

Keypad keypad(makeKeymap(key::keys), key::rowPins, key::colPins, KEY_ROWS, KEY_COLS);
TFT_UNIT tft{TFT_CS, TFT_RS, TFT_MOSI, TFT_SCK, TFT_RESET, TFT_MISO};

void setup() {

	//1) Инициировать экран, если не работает,
	// то высрать в сериальный порт ошибку и зациклиться,
	// если работает, включить подсветку

	//2) Считать конфиги, если не считались, то высрать на экран ошибку инициации
	//3) Инициировать датчики, если что-то не инициировалось, то вывести на экран предупреждение

	tft.begin();

    // auto window_2 = UIwindow::builder::makeWindow()
    //     .setDrawBody(mw::drawBody)
    //     .setWorker(mw::worker)
    //     .build();

}

void loop() {

	auto key = keypad.getKey();

	if (key == LEFT_KEY) {
		
		Serial.println("LEFT");
	}

	if (key == RIGHT_KEY) {

		Serial.println("RIGHT");
	}

	if (key == UP_KEY) {
		
		Serial.println("UP");
	}	

	if (key == DOWN_KEY) {
		
		Serial.println("DOWN");
	}

	if (key == OK_KEY) {
		
		Serial.println("OK");
	}
}