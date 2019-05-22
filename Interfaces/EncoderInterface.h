#ifndef INTERFACES_ENCODER_INTERFACE_H
#define INTERFACES_ENCODER_INTERFACE_H

#include <stdint.h>
#include <stdbool.h>

typedef struct Encoder_tag{
    uint32_t ticks;
    float position;
    float degrees;
    float speed;
} Encoder;

void InitializeEncoders(void);
Encoder* GetLeftEncoder(void);
Encoder* GetRightEncoder(void);

#endif
