/*
 * Driver code for the Adafruit VL6180X IR Distance Sensor
 *
 * Author: Todd Morehouse
 */

#ifndef DRIVERS_VL6180X_H
#define DRIVERS_VL6180X_H

#include <stdint.h>
#include <stdbool.h>

typedef struct VL6180X_Driver_tag{
    float DutyCycle;
    bool isClockwise;

    uint32_t PWMBase;
    uint32_t PWMGen;
    uint32_t PWMBit;
    uint32_t PWMOut;
    uint32_t AIN1_BASE;
    uint32_t AIN2_BASE;
    uint32_t AIN1_PIN;
    uint32_t AIN2_PIN;
} VL6180X_Driver;

void VL6180X_Init(
        VL6180X_Driver *driver,
        uint32_t I2CBase,
        uint32_t PWMGenerator,
        uint32_t SysCtlPWMModule,
        uint32_t PWMFrequency,
        uint32_t PWMBit,
        uint32_t PWMOut,
        uint32_t AIN1Base,
        uint32_t AIN2Base,
        uint32_t AIN1Pin,
        uint32_t AIN2Pin,
        uint32_t SysClk);

void VL6180X_SetDutyCycle(VL6180X_Driver *driver,
                         float DutyCycle);

void VL6180X_SetDirection(VL6180X_Driver *driver,
                         bool isClockwise);

#endif
