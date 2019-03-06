#ifndef MM_CONTROL_GLOBALS_H
#define MM_CONTROL_GLOBALS_H

#include "PID.h"

#define WHEEL_SPEED_KP              0.01
#define WHEEL_SPEED_KI              0.005
#define WHEEL_SPEED_KD              0.0005
//Working but slow repsonse
//.01
//.005
//.0005

#define WHEEL_SPEED_I_MIN            -2.0 / WHEEL_SPEED_KI
#define WHEEL_SPEED_I_MAX             2.0 / WHEEL_SPEED_KI

#define WHEEL_SPEED_OUTPUT_MIN       0.0
#define WHEEL_SPEED_OUTPUT_MAX       1.0

#define TURN_SPEED  15.0
#define TURN_KP     1.00

#define MOVE_SPEED  35.0
#define MOVE_KP     1.00

#define TICKS_IN_90_DEGREES 40

typedef enum{
    LEFT_WHEEL,
    RIGHT_WHEEL,
} SPEED_CONTROL_PID_ENUM;

typedef enum{
    POSITION,
    LEFT_TURN,
    RIGHT_TURN,
} POSITION_CONTROL_PID_ENUM;
#endif
