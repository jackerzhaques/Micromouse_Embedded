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
    uint8_t address;
    uint16_t distance;
} VL6180X_Driver;

void VL6180X_ReadDistance(VL6180X_Driver* driver);

#endif
