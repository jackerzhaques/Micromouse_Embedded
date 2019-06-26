#ifndef I2C_H
#define I2C_H

//Project includes

//Standard includes
#include <stdint.h>
#include <stdbool.h>

//Tivaware includes

void InitializeI2C(uint32_t SysClk);
void I2CWrite(uint16_t reg, uint8_t byte, uint8_t slaveAddress);
uint8_t I2CRead(uint16_t reg, uint8_t slaveAddress);

#endif
