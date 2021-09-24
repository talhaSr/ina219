/*
 * ina219.c
 *
 *  Created on: Sep 15, 2021
 *      Author: Talha
 */

#include "ina219.h"

/* Private Function Prototypes */
static uint16_t _Read(INA219_t *sensor, I2C_HandleTypeDef *hi2c, uint8_t registerAddr);
static void _Write(INA219_t *sensor, I2C_HandleTypeDef *hi2c, uint8_t registerAddr, uint16_t data);

/* Private Functions */
static uint16_t _Read(INA219_t *sensor, I2C_HandleTypeDef *hi2c, uint8_t registerAddr)
{
	uint8_t data[2];
	HAL_I2C_Mem_Read_IT(hi2c, (sensor->address << 1), registerAddr, 1, data, 2);
	return (uint16_t)((data[0] << 8) | data[1]);
}

static void _Write(INA219_t *sensor, I2C_HandleTypeDef *hi2c, uint8_t registerAddr, uint16_t data)
{
	uint8_t buffer[2];
	buffer[0] = (data >> 8) & 0xFF;
	buffer[1] = (data >> 0) & 0xFF;
	HAL_I2C_Mem_Write_IT(hi2c, (sensor->address << 1), registerAddr, 1, buffer, 2);
}

/* Sensor Functions */
#if ADVANCED_MODE
INA219_Status_t INA219_Init(INA219_t *sensor, I2C_HandleTypeDef *hi2c, uint8_t deviceAddr,
							INA219_BusVoltageRange_t, busRange, INA219_Gain_t gain,
							INA219_BusADC_t busADC, INA219_ShuntADC_t shuntADC, INA219_Mode_t mode,
							uint16_t calibration)
{
	/* TO-DO */
	sensor->address = deviceAddr;
	sensor->currentCoeff = 0;
	sensor->powerCoeff = 0;

	if (HAL_I2C_IsDeviceReady(hi2c, (sensor->address), 3, 2) != HAL_OK)
		return INA219_ERROR;

	INA219_Reset(sensor, hi2c);
	uint16_t buffer = busRange | gain | busADC | shuntADC | mode;
	INA219_SetConfiguration(sensor, hi2c, buffer);
	INA219_SetCalibration(sensor, hi2c, calibration);

	return INA219_OK;
}
#else
INA219_Status_t INA219_Init(INA219_t *sensor, I2C_HandleTypeDef *hi2c, uint8_t deviceAddr)
{
#ifdef INA219_32V_2A
	sensor->currentCoeff = 10.0f;
	sensor->powerCoeff = 2.0f;
	sensor->address = (uint8_t)deviceAddr;
	if (HAL_I2C_IsDeviceReady(hi2c, (sensor->address << 1), 3, 2) != HAL_OK)
		return INA219_ERROR;
	INA219_Reset(sensor, hi2c);
	INA219_SetCalibration(sensor, hi2c, 0x1000);
	INA219_SetConfiguration(sensor, hi2c, (FSR32V | GAIN_8 | BADC_12bit | SADC_12bit | MODE_SHUNT_BUS_CONTINUOUS));
	return INA219_OK;
#endif /* INA219_32V_2A */
#ifdef INA219_32V_1A
	sensor->currentCoeff = 25.0f;
	sensor->powerCoeff = 0.8f;
	if (HAL_I2C_IsDeviceReady(hi2c, (sensor->address), 3, 2) != HAL_OK)
		return INA219_ERROR;
	INA219_Reset(sensor, hi2c);
	INA219_SetCalibration(sensor, hi2c, 0x2800);
	INA219_SetConfiguration(sensor, hi2c, (FSR32V | GAIN_8 | BADC_12bit | SADC_12bit | MODE_SHUNT_BUS_CONTINUOUS));
	return INA219_OK;
#endif /* INA219_32V_1A */
#ifdef INA219_16V_400mA
	sensor->currentCoeff = 20.0f;
	sensor->powerCoeff = 1.0f;
	if (HAL_I2C_IsDeviceReady(hi2c, (sensor->address), 3, 2) != HAL_OK)
		return INA219_ERROR;
	INA219_Reset(sensor, hi2c);
	INA219_SetCalibration(sensor, hi2c, 0x2000);
	INA219_SetConfiguration(sensor, hi2c, (FSR16V | GAIN_1 | BADC_12bit | SADC_12bit | MODE_SHUNT_BUS_CONTINUOUS));
	return INA219_OK;
#endif /* INA219_16V_400mA */
}
#endif

void INA219_GetBusVoltage(INA219_t *sensor, I2C_HandleTypeDef *hi2c)
{
	uint16_t data = _Read(sensor, hi2c, BUS_VOLTAGE);
	sensor->busVoltage = (uint16_t)(((data >> 3) * 4) * 0.01f);	/* mV */
}

void INA219_GetShuntVoltage(INA219_t *sensor, I2C_HandleTypeDef *hi2c)
{
	uint16_t data = _Read(sensor, hi2c, SHUNT_VOLTAGE);
	sensor->shuntVoltage = (uint16_t)(data * 0.01f);	/* mV */
}

void INA219_GetCurrent(INA219_t *sensor, I2C_HandleTypeDef *hi2c)
{
	sensor->current = (int16_t)(_Read(sensor, hi2c, CURRENT) / sensor->currentCoeff);	/* mA */
}

void INA219_GetPower(INA219_t *sensor, I2C_HandleTypeDef *hi2c)
{
	sensor->power = (int16_t)(_Read(sensor, hi2c, POWER) * sensor->powerCoeff);		/* mW */
}

void INA219_Reset(INA219_t *sensor, I2C_HandleTypeDef *hi2c)
{
	_Write(sensor, hi2c, CONFIGURATION, 0x8000);
}

void INA219_SetCalibration(INA219_t *sensor, I2C_HandleTypeDef *hi2c, uint16_t calibration)
{
	_Write(sensor, hi2c, CALIBRATION, calibration);
}

void INA219_SetConfiguration(INA219_t *sensor, I2C_HandleTypeDef *hi2c, uint16_t config)
{
	_Write(sensor, hi2c, CONFIGURATION, config);
}
