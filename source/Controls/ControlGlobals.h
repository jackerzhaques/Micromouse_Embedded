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
    POSITION,
    LEFT_TURN,
    RIGHT_TURN,
    PLANT_SIZE
} PID_enum;

static sPID PIDs[] = {
                //Left Wheel
                {
                     WHEEL_SPEED_KP,
                     WHEEL_SPEED_KI,
                     WHEEL_SPEED_KD,
                     0,                 //Default target to 0
                     0,                 //Default iState to 0
                     WHEEL_SPEED_I_MIN,
                     WHEEL_SPEED_I_MAX,
                     0,                 //Default last error to 0
                     0,                 //Default output to 0
                     WHEEL_SPEED_OUTPUT_MIN,
                     WHEEL_SPEED_OUTPUT_MAX,
                },
                //Right wheel
                {
                     WHEEL_SPEED_KP,
                     WHEEL_SPEED_KI,
                     WHEEL_SPEED_KD,
                     0,                 //Default target to 0
                     0,                 //Default iState to 0
                     WHEEL_SPEED_I_MIN,
                     WHEEL_SPEED_I_MAX,
                     0,                 //Default last error to 0
                     0,                 //Default output to 0
                     WHEEL_SPEED_OUTPUT_MIN,
                     WHEEL_SPEED_OUTPUT_MAX,
                },
                //Position PID
                 {
                    MOVE_KP,
                    0,            //No Ki or Kd
                    0,
                    0,            //Default target to 0
                    0,            //All Ki terms to 0
                    0,
                    0,
                    0,            //Kd terms to 0
                    0,            //Default output to 0
                    0,            //Lowest speed is 0
                    MOVE_SPEED,   //Max move speed
                 },
                 //Left Motor PID
                 {
                    TURN_KP,
                    0,            //No Ki or Kd
                    0,
                    0,            //Default target to 0
                    0,            //All Ki terms to 0
                    0,
                    0,
                    0,            //Kd terms to 0
                    0,            //Default output to 0
                    0,            //Lowest speed is 0
                    TURN_SPEED,   //Max move speed
                 },
                 //Right Motor PID
                 {
                    TURN_KP,
                    0,            //No Ki or Kd
                    0,
                    0,            //Default target to 0
                    0,            //All Ki terms to 0
                    0,
                    0,
                    0,            //Kd terms to 0
                    0,            //Default output to 0
                    0,            //Lowest speed is 0
                    TURN_SPEED,   //Max move speed
                 }
};
#endif
