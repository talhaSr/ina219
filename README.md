# INA219 Zerø-Drift, Bidirectional Current/Power Monitor With I2C Interface
![Licence](https://img.shields.io/badge/License-GPL--3.0-orange)

**Author:** Talha Sarı
## Description
The INA219 is a current shunt and power monitor with an I2C- or SMBUS-compatible interface. The device monitors both shunt voltage drop and bus supply voltage, with programmable conversion times and filtering. A programmable calibration value, combined with an internal multiplier, enables direct readouts of current in amperes. An additional multiplying register calculates power in watts. The I2C- or SMBUS-compatible interface features 16 programmable addresses.
The INA219 is available in two grades: A and B. The B grade version has higher accuracy and higher precision specifications.
The INA219 senses across shunts on buses that can vary from 0 to 26 V. The device uses a single 3- to 5.5-V supply, drawing a maximum of 1 mA of supply current. The INA219 operates from –40°C to 125°C. For more check the datasheet: [`/doc/LIS3MDL_Datasheet.pdf`](./doc/LIS3MDL_Datasheet.pdf)
## Features
- Senses Bus Voltages from 0 to 26 V
- Reports Current, Voltage, and Power
- 16 Programmable Addresses
- High Accuracy: 0.5% (Maximum) Over Temperature (INA219B)
- Filtering Options
- Calibration Registers
- SOT23-8 and SOIC-8 Packages

## Pin Description
![INA219](./doc/ina219_img.png)
### SOT-23 Package
|Pin #|Name|Desciption|
|--|--|--|
|1|IN+|Positive differential shunt voltage. Connect to positive side of shunt resistor.|
|2|IN-|Negative differential shunt voltage. Connect to negative side of shunt resistor. Bus voltage is measured from this pin to ground.|
|3|GND|Connect to GND|
|4|Vs|Power supply, 3 to 5.5V|
|5|SCL|I2C serial clock|
|6|SDA|I2C serial data|
|7|A0|Address pin|
|8|A1|Address pin|

## About
This library is created for STM32 development environment and can be used only with STM32F series microcontrollers for now. Example project will be added soon in this directory: [`/example`](./example)

***Note:**  This library requires HAL!*

# License
GNU General Public License v3.0
