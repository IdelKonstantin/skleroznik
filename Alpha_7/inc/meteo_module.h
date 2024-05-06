#ifndef _METEO_MODULE_H_
#define _METEO_MODULE_H_

#include <cstdint>
#include <Wire.h>
#include <SparkFun_FS3000_Arduino_Library.h> //http://librarymanager/All#SparkFun_FS3000
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

/*

Необходимо установить библиотеки в менеджере Ардуино на FS3000 и bme280 от Adafruit

*/

namespace bc {
	
	struct meteoData {

		float T{15.0};
		uint16_t P{1000};
		uint8_t H{76};
		float W{0.0};
	};
};

class meteoModule {

private:

	bool m_bmeInited{false};
	bool m_windMeterInited{false};

	FS3000 m_fs;
	Adafruit_BME280 m_bme;
	bc::meteoData m_meteoInfo;

public:
	
	meteoModule();
	bool init();
	bc::meteoData getMeasurements();
};

#endif /* _METEO_MODULE_H_ */