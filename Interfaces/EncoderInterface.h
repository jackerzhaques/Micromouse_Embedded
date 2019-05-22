#ifndef INTERFACES_ENCODER_INTERFACE_H
#define INTERFACES_ENCODER_INTERFACE_H

#include <stdint.h>
#include <stdbool.h>

typedef struct Encoder_tag{
    uint32_t ticks;
    float degrees;
    float speed;
    float ticksToDegreesRatio;
    uint8_t tickReceived;
    uint32_t lastTimerVal;
} Encoder;

void InitializeEncoders(void);
Encoder* GetLeftEncoder(void);
Encoder* GetRightEncoder(void);

#endif
