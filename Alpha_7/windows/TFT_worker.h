#ifndef _TFT_WORKER_H_
#define _TFT_WORKER_H_

#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#define TFT_CS 5
#define TFT_RS 14
#define TFT_MOSI 23
#define TFT_SCK 18
#define TFT_RESET 27
#define TFT_MISO 19

//Установить Adafruit_ILI9341 в менеджере библиотек IDE

#define TFT_UNIT Adafruit_ILI9341

//TFT_UNIT tft{TFT_CS, TFT_RS, TFT_MOSI, TFT_SCK, TFT_RESET, TFT_MISO};

#endif /* _TFT_WORKER_H_ */