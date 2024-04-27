#include "BC_windows.h"
#include "TFT_worker.h"
#include "main_window.h"

TFT_UNIT tft{TFT_CS, TFT_RS, TFT_MOSI, TFT_SCK, TFT_RESET, TFT_MISO};

void setup() {

    Serial.begin(115200);
    tft.begin();

    UIwindow window_1{mw::setup, mw::drawHead, mw::drawBody, mw::drawTail, mw::drawCanvas, mw::markers, mw::worker};

    auto window_2 = UIwindow::builder::makeWindow()
        .setDrawBody(mw::drawBody)
        .setWorker(mw::worker)
        .build();

    auto window_3 = UIwindow::builder::makeWindow()
        .setDrawTail(mw::drawTail)
        .setSetup(mw::setup)
        .build();

    window_1.start();
    window_2.start();
    window_3.start();
}

void loop() {


}
