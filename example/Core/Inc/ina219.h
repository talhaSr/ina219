/*
 * ina219.h
 *
 *  Created on: Sep 15, 2021
 *      Author: Talha
 */

#ifndef INC_INA219_H_
#define INC_INA219_H_

#include "main.h"
#include <stdint.h>

/*
 * This option is turning on some secret things..
 * Do not touch if you're newbie ;)
 */
#define ADVANCED_MODE 0

/*
 * Uncomment the chosen configuration preset and comment the others
 */
#if !ADVANCED_MODE
#define INA219_32V_2A
//#define INA219_32V_1A
//#define INA219_16V_400mA
#endif

/* Device Address */
#define INA219_ADDR		0x40

/* Register Map */
#define CONFIGURATION	0x00
#define SHUNT_VOLTAGE	0x01
#define BUS_VOLTAGE		0x02
#define POWER			0x03
#define CURRENT			0x04
#define CALIBRATION		0x05

/* Sensor Handler */
typedef struct INA219 {
	uint8_t address;
	uint16_t shuntVoltage;
	uint16_t busVoltage;
	int16_t power;
	int16_t current;
	float currentCoeff;
	float powerCoeff;
} INA219_t;

/* Enums */
/* BRNG[13] -  Bus Voltage Range */
typedef enum {
	FSR16V = 0x0000,
	FSR32V = 0x2000
} INA219_BusVoltageRange_t;

/* PG[11:12] - PGA Gain */
typedef enum {
	GAIN_1 = 0x0000,	/* ±40 mV	*/
	GAIN_2 = 0x0800,	/* ±80 mV	*/
	GAIN_4 = 0x1000,	/* ±160 mV	*/
	GAIN_8 = 0x1800		/* ±320 mV	*/
} INA219_Gain_t;

/* BADC[7:10] - Bus ADC Resolution/Averaging */
typedef enum {
	BADC_9bit	= 0x0000,	/* 9-bit  - 84us		*/
	BADC_10bit	= 0x0080,	/* 10-bit - 148us		*/
	BADC_11bit	= 0x0100,	/* 11-bit - 276us		*/
	BADC_12bit	= 0x0180,	/* 12-bit - 532us		*/
	BADC_2S		= 0x0480,	/* 2 x 12-bit averaged samples		*/
	BADC_4S		= 0x0500,	/* 4 x 12-bit averaged samples		*/
	BADC_8S		= 0x0580,	/* 8 x 12-bit averaged samples		*/
	BADC_16S	= 0x0600,	/* 16 x 12-bit averaged samples		*/
	BADC_32S	= 0x0680,	/* 32 x 12-bit averaged samples		*/
	BADC_64S	= 0x0700,	/* 64 x 12-bit averaged samples		*/
	BADC_128S	= 0x0780	/* 128 x 12-bit averaged samples	*/
} INA219_BusADC_t;

/* SADC[3:6] - Shunt ADC Resolution/Averaging */
typedef enum {
	SADC_9bit	= 0x0000,	/* 9-bit  - 84us		*/
	SADC_10bit	= 0x0080,	/* 10-bit - 148us		*/
	SADC_11bit	= 0x0010,	/* 11-bit - 276us		*/
	SADC_12bit	= 0x0018,	/* 12-bit - 532us		*/
	SADC_2S		= 0x0048,	/* 2 x 12-bit averaged samples		*/
	SADC_4S		= 0x0050,	/* 4 x 12-bit averaged samples		*/
	SADC_8S		= 0x0058,	/* 8 x 12-bit averaged samples		*/
	SADC_16S	= 0x0060,	/* 16 x 12-bit averaged samples		*/
	SADC_32S	= 0x0068,	/* 32 x 12-bit averaged samples		*/
	SADC_64S	= 0x0070,	/* 64 x 12-bit averaged samples		*/
	SADC_128S	= 0x0078	/* 128 x 12-bit averaged samples	*/
} INA219_ShuntADC_t;

/* MODE[0:2] - Operation Mode */
typedef enum {
	MODE_POWERDOWN 				= 0x0000,	/* Power-down 				*/
	MODE_SHUNT_TRIG				= 0x0001,	/* Shunt Voltage, Triggered */
	MODE_BUS_TRIG				= 0x0002,	/* Bus Voltage, Triggered 	*/
	MODE_SHUNT_BUS_TRIG 		= 0x0003,	/* Shunt & Bus, Triggered	*/
	MODE_ADC_OFF 				= 0x0004,	/* ADC Disabled				*/
	MODE_SHUNT_CONTINUOUS		= 0x0005,	/* Shunt , Continuous		*/
	MODE_BUS_CONTINUOUS			= 0x0006,	/* Bus, Continuous			*/
	MODE_SHUNT_BUS_CONTINUOUS	= 0x0007	/* Shunt & Bus, Continuous	*/
} INA219_Mode_t;

/* Sensor Status Enum */
typedef enum {
	INA219_ERROR,
	INA219_OK
} INA219_Status_t;

/* Sensor Function Prototypes */
#if ADVANCED_MODE
INA219_Status_t INA219_Init(INA219_t *sensor, I2C_HandleTypeDef *hi2c, uint8_t deviceAddr,
							INA219_BusVoltageRange_t, busRange, INA219_Gain_t gain,
							INA219_BusADC_t busADC, INA219_ShuntADC_t shuntADC, INA219_Mode_t mode,
							uint16_t calibration);
#else
INA219_Status_t INA219_Init(INA219_t *sensor, I2C_HandleTypeDef *hi2c, uint8_t deviceAddr);
#endif

void INA219_GetBusVoltage(INA219_t *sensor, I2C_HandleTypeDef *hi2c);
void INA219_GetShuntVoltage(INA219_t *sensor, I2C_HandleTypeDef *hi2c);
void INA219_GetCurrent(INA219_t *sensor, I2C_HandleTypeDef *hi2c);
void INA219_GetPower(INA219_t *sensor, I2C_HandleTypeDef *hi2c);

void INA219_Reset(INA219_t *sensor, I2C_HandleTypeDef *hi2c);

void INA219_SetCalibration(INA219_t *sensor, I2C_HandleTypeDef *hi2c, uint16_t calibration);
void INA219_SetConfiguration(INA219_t *sensor, I2C_HandleTypeDef *hi2c, uint16_t config);


#endif /* INC_INA219_H_ */
