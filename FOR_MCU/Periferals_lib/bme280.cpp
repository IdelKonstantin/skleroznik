#include "bme280.h"

void bme280::errorHandler() {

	if(this->errorHandlerCallback != nullptr) {

		this->errorHandlerCallback();
	}
}

void bme280::checkTransferStatus() {

	if(this->status != HAL_OK) {

		this->errorHandler();
	}
}

void bme280::I2Cx_WriteData(uint16_t Addr, uint8_t Reg, uint8_t Value) {

	this->status = HAL_OK;
	this->status = HAL_I2C_Mem_Write(&hi2c1, Addr, (uint16_t)Reg, I2C_MEMADD_SIZE_8BIT, &Value, 1, 0x10000);
	this->checkTransferStatus();
}

uint8_t bme280::I2Cx_ReadData(uint16_t Addr, uint8_t Reg) {

	uint8_t value{0};
	this->status = HAL_OK;
	this->status = HAL_I2C_Mem_Read(&hi2c1, Addr, Reg, I2C_MEMADD_SIZE_8BIT, &value, 1, 0x10000);
	this->checkTransferStatus();
	return value;
}

void bme280::I2Cx_ReadData16(uint16_t Addr, uint8_t Reg, uint16_t *Value) {

	this->status = HAL_OK;
	this->status = HAL_I2C_Mem_Read(&hi2c1, Addr, Reg, I2C_MEMADD_SIZE_8BIT, (uint8_t*)Value, 2, 0x10000);
	this->checkTransferStatus();
}

void bme280::I2Cx_ReadData24(uint16_t Addr, uint8_t Reg, uint32_t *Value) {

	this->status = HAL_OK;
	this->status = HAL_I2C_Mem_Read(&hi2c1, Addr, Reg, I2C_MEMADD_SIZE_8BIT, (uint8_t*)Value, 3, 0x10000);
	this->checkTransferStatus()	;
}

void bme280::BME280_WriteReg(uint8_t Reg, uint8_t Value) {

	this->I2Cx_WriteData(BME280_ADDRESS, Reg, Value);
}

uint8_t bme280::BME280_ReadReg(uint8_t Reg) {

	return this->I2Cx_ReadData(BME280_ADDRESS,Reg);
}

void bme280::BME280_ReadReg_U16(uint8_t Reg, uint16_t *Value) {

	this->I2Cx_ReadData16(BME280_ADDRESS,Reg,Value);
}

void bme280::BME280_ReadReg_S16(uint8_t Reg, int16_t *Value) {

	this->I2Cx_ReadData16(BME280_ADDRESS,Reg, (uint16_t*) Value);
}

void bme280::BME280_ReadReg_BE_S16(uint8_t Reg, int16_t *Value) {

	this->I2Cx_ReadData16(BME280_ADDRESS,Reg,(uint16_t*)Value);
	*(uint16_t *) Value = BE16_TO_WORD(*(uint16_t *) Value);
}

void bme280::BME280_ReadReg_U24(uint8_t Reg, uint32_t *Value) {

	this->I2Cx_ReadData24(BME280_ADDRESS,Reg,Value);
	*(uint32_t *) Value &= 0x00FFFFFF;
}

void bme280::BME280_ReadReg_BE_U24(uint8_t Reg, uint32_t *Value) {

	this->I2Cx_ReadData24(BME280_ADDRESS,Reg,Value);
	*(uint32_t *) Value = BE24_TO_WORD(*(uint32_t *) Value) & 0x00FFFFFF;
}

uint8_t bme280::BME280_ReadStatus() {

	return this->BME280_ReadReg(BME280_REGISTER_STATUS)&0x09;
}

void bme280::BME280_ReadCoefficients() {

	BME280_ReadReg_U16(BME280_REGISTER_DIG_T1,&CalibData.dig_T1);
	BME280_ReadReg_S16(BME280_REGISTER_DIG_T2,&CalibData.dig_T2);
	BME280_ReadReg_S16(BME280_REGISTER_DIG_T3,&CalibData.dig_T3);
	BME280_ReadReg_U16(BME280_REGISTER_DIG_P1,&CalibData.dig_P1);
	BME280_ReadReg_S16(BME280_REGISTER_DIG_P2,&CalibData.dig_P2);
	BME280_ReadReg_S16(BME280_REGISTER_DIG_P3,&CalibData.dig_P3);
	BME280_ReadReg_S16(BME280_REGISTER_DIG_P4,&CalibData.dig_P4);
	BME280_ReadReg_S16(BME280_REGISTER_DIG_P5,&CalibData.dig_P5);
	BME280_ReadReg_S16(BME280_REGISTER_DIG_P6,&CalibData.dig_P6);
	BME280_ReadReg_S16(BME280_REGISTER_DIG_P7,&CalibData.dig_P7);
	BME280_ReadReg_S16(BME280_REGISTER_DIG_P8,&CalibData.dig_P8);
	BME280_ReadReg_S16(BME280_REGISTER_DIG_P9,&CalibData.dig_P9);
	CalibData.dig_H1 = BME280_ReadReg(BME280_REGISTER_DIG_H1);
	BME280_ReadReg_S16(BME280_REGISTER_DIG_H2,&CalibData.dig_H2);
	CalibData.dig_H3 = BME280_ReadReg(BME280_REGISTER_DIG_H3);
	CalibData.dig_H4 = (BME280_ReadReg(BME280_REGISTER_DIG_H4) << 4) | (BME280_ReadReg(BME280_REGISTER_DIG_H4+1) & 0xF);
	CalibData.dig_H5 = (BME280_ReadReg(BME280_REGISTER_DIG_H5+1) << 4) | (BME280_ReadReg(BME280_REGISTER_DIG_H5) >> 4);
	CalibData.dig_H6 = (int8_t)BME280_ReadReg(BME280_REGISTER_DIG_H6);
}

void bme280::BME280_SetStandby(uint8_t tsb) {

	uint8_t reg = this->BME280_ReadReg(BME280_REG_CONFIG) & ~BME280_STBY_MSK;
	reg |= tsb & BME280_STBY_MSK;
	this->BME280_WriteReg(BME280_REG_CONFIG,reg);
}

void bme280::BME280_SetFilter(uint8_t filter) {

	uint8_t reg = this->BME280_ReadReg(BME280_REG_CONFIG) & ~BME280_FILTER_MSK;
	reg |= filter & BME280_FILTER_MSK;
	this->BME280_WriteReg(BME280_REG_CONFIG,reg);
}

void bme280::BME280_SetOversamplingTemper(uint8_t osrs) {

	uint8_t reg = this->BME280_ReadReg(BME280_REG_CTRL_MEAS) & ~BME280_OSRS_T_MSK;
	reg |= osrs & BME280_OSRS_T_MSK;
	this->BME280_WriteReg(BME280_REG_CTRL_MEAS,reg);
}

void bme280::BME280_SetOversamplingPressure(uint8_t osrs) {

	uint8_t reg = this->BME280_ReadReg(BME280_REG_CTRL_MEAS) & ~BME280_OSRS_P_MSK;
	reg |= osrs & BME280_OSRS_P_MSK;
	this->BME280_WriteReg(BME280_REG_CTRL_MEAS,reg);
}

void bme280::BME280_SetOversamplingHum(uint8_t osrs) {

	uint8_t reg = this->BME280_ReadReg(BME280_REG_CTRL_HUM) & ~BME280_OSRS_H_MSK;
	reg |= osrs & BME280_OSRS_H_MSK;
	this->BME280_WriteReg(BME280_REG_CTRL_HUM,reg);

	//The 'ctrl_hum' register needs to be written
	//after changing 'ctrl_hum' for the changes to become effective.
	reg = this->BME280_ReadReg(BME280_REG_CTRL_MEAS);
	this->BME280_WriteReg(BME280_REG_CTRL_MEAS,reg);
}

void bme280::BME280_SetMode(uint8_t mode) {

	uint8_t reg = this->BME280_ReadReg(BME280_REG_CTRL_MEAS) & ~BME280_MODE_MSK;
	reg |= mode & BME280_MODE_MSK;
	this->BME280_WriteReg(BME280_REG_CTRL_MEAS,reg);
}

/******************************* INTEFACES *******************************/

void bme280::Init() {

	uint8_t value = this->BME280_ReadReg(BME280_REG_ID);

	if(value != BME280_ID) {

		this->errorHandler();
		return;
	}

	this->BME280_WriteReg(BME280_REG_SOFTRESET,BME280_SOFTRESET_VALUE);

	while (this->BME280_ReadStatus() & BME280_STATUS_IM_UPDATE) {};

	this->BME280_ReadCoefficients();
	this->BME280_SetStandby(BME280_STBY_1000);
	this->BME280_SetFilter(BME280_FILTER_4);
	this->BME280_SetOversamplingTemper(BME280_OSRS_T_x4);
	this->BME280_SetOversamplingPressure(BME280_OSRS_P_x2);
	this->BME280_SetOversamplingHum(BME280_OSRS_H_x1);

	uint32_t value32 = this->BME280_ReadReg(BME280_REG_CTRL_MEAS);
	value32 |= this->BME280_ReadReg(BME280_REG_CTRL_HUM) << 8;

	this->BME280_SetMode(BME280_MODE_NORMAL);
}

void bme280::SetErrorHandler(void(*func)(void)) {

	this->errorHandlerCallback = func;
}

float bme280::ReadTemperature() { //TODO - check for negative temperatures

	float temper_float{0.0f};
	uint32_t temper_raw{0};
	int32_t val1{0};
	int32_t val2{0};

	this->BME280_ReadReg_BE_U24(BME280_REGISTER_TEMPDATA,&temper_raw);

	temper_raw >>= 4;

	val1 = ((((temper_raw>>3) - ((int32_t)CalibData.dig_T1 <<1))) * ((int32_t)CalibData.dig_T2)) >> 11;

	val2 = (int32_t)((((((temper_raw>>4) - ((int32_t)CalibData.dig_T1)) *
			((temper_raw>>4) - ((int32_t)CalibData.dig_T1))) >> 12) *
			((int32_t)CalibData.dig_T3)) >> 14);

	this->temper_int = val1 + val2;
	temper_float = ((temper_int * 5 + 128) >> 8);
	temper_float /= 100.0f;
	return temper_float;
}

float bme280::ReadPressure() {

	float press_float{0.0f};
	uint32_t press_raw, pres_int;

	this->ReadTemperature(); // must be done first to get temper_int
	this-> BME280_ReadReg_BE_U24(BME280_REGISTER_PRESSUREDATA,&press_raw);
	press_raw >>= 4;

	int64_t val1, val2, p;

	val1 = ((int64_t) this->temper_int) - 128000;
	val2 = val1 * val1 * (int64_t)CalibData.dig_P6;
	val2 = val2 + ((val1 * (int64_t)CalibData.dig_P5) << 17);
	val2 = val2 + ((int64_t)CalibData.dig_P4 << 35);
	val1 = ((val1 * val1 * (int64_t)CalibData.dig_P3) >> 8) + ((val1 * (int64_t)CalibData.dig_P2) << 12);
	val1 = (((((int64_t)1) << 47) + val1)) * ((int64_t)CalibData.dig_P1) >> 33;

	if (val1 == 0) {

		return (uint16_t)0; // avoid exception caused by division by zero
	}

	p = 1048576 - press_raw;
	p = (((p << 31) - val2) * 3125) / val1;
	val1 = (((int64_t)CalibData.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
	val2 = (((int64_t)CalibData.dig_P8) * p) >> 19;
	p = ((p + val1 + val2) >> 8) + ((int64_t)CalibData.dig_P7 << 4);
	pres_int = ((p >> 8) * 1000) + (((p & 0xff) * 390625) / 100000);
	press_float = pres_int / 100000.0f;

	return press_float;
}

float bme280::ReadHumidity() {

	float hum_float{0.0f};
	int16_t hum_raw{};
	int32_t hum_raw_sign, v_x1_u32r;

	this->ReadTemperature(); // must be done first to get t_fine
	this->BME280_ReadReg_BE_S16(BME280_REGISTER_HUMIDDATA,&hum_raw);

	hum_raw_sign = ((int32_t)hum_raw)&0x0000FFFF;

	v_x1_u32r = (temper_int - ((int32_t)76800));
	v_x1_u32r = (((((hum_raw_sign << 14) - (((int32_t)CalibData.dig_H4) << 20) -
		(((int32_t)CalibData.dig_H5) * v_x1_u32r)) + ((int32_t)16384)) >> 15) *
		(((((((v_x1_u32r * ((int32_t)CalibData.dig_H6)) >> 10) *
		(((v_x1_u32r * ((int32_t)CalibData.dig_H3)) >> 11) + ((int32_t)32768))) >> 10) +
		((int32_t)2097152)) * ((int32_t)CalibData.dig_H2) + 8192) >> 14));

	v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) *
		((int32_t)CalibData.dig_H1)) >> 4));

	v_x1_u32r = (v_x1_u32r < 0) ? 0 : v_x1_u32r;
	v_x1_u32r = (v_x1_u32r > 419430400) ? 419430400 : v_x1_u32r;
	hum_float = (v_x1_u32r>>12);
	hum_float /= 1024.0f;
	return hum_float;
}