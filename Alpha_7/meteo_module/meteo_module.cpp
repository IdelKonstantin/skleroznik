#include "meteo_module.h"

meteoModule::meteoModule() {};

void meteoModule::init() {

	Wire.begin();
	
	m_windMeterInited = m_fs.begin();

	if(m_windMeterInited) {

		//for FS3000-1015 (0 - 15 m/sec)
		m_fs.setRange(AIRFLOW_RANGE_15_MPS);
		
		//for FS3000-1005 (0 - 7 m/sec)
		//m_fs.setRange(AIRFLOW_RANGE_7_MPS);
	}

	m_bmeInited = m_bme.begin(0x76);
	
	if(!m_bmeInited) {

		m_bmeInited = m_bme.begin(0x77);
	}
}

bc::meteoData meteoModule::getMeasurements() {

	if(m_windMeterInited) {

		m_meteoInfo.W = m_fs.readMetersPerSecond();
	}

	if(m_bmeInited) {

		m_meteoInfo.T = m_bme.readTemperature();
		m_meteoInfo.P = m_bme.readPressure()/100.0F;
		m_meteoInfo.H = m_bme.readHumidity();
	}

	return m_meteoInfo;
}
