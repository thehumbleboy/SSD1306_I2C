#ifndef I2C_H
#define I2C_H

//--------------Includes-----------------------
#include "stm32f4xx.h"

//--------------Macros-------------------------
#define I2C_FREQCLOCK                     0x04


//-------------Function Definitions------------
void I2C_Init();
void I2C_Start();
void I2C_SendAddress(uint8_t Address);
void I2C_Transmit(uint8_t Data);
void I2C_Stop();

#endif