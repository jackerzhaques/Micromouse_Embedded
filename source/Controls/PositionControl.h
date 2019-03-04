#ifndef MM_POSITION_CONTROL_H
#define MM_POSITION_CONTROL_H

/*
 * Robot sizes
 *
 * Wheel diameter: 60mm
 * Robot Turn Diameter: 93mm
 *
 * Wheel Circumference: pi*d = 188mm
 * Robot Circ.          pi*d = 292mm
 *
 * Wheel Degrees Per Tick       360/20 = 18
 * Distance per tick            188/20 = 9.4mm
 *
 *
 */

//Distance in mm
//Angles in degrees
#define WHEEL_DIAMETER      60
#define ROBOT_TURN_DIAMETER 93
#define WHEEL_CIRC          188
#define ROBOT_CIRC          292
#define DEGREES_PER_TICK    18
#define DISTANCE_PER_TICK   9.4

typedef struct sPos_tag{
    float x;
    float y;
} sPos;

typedef enum{
    NORTH,
    SOUTH,
    EAST,
    WEST
} Direction;

void PC_Initialize(void);
void PC_MoveForward(void);
sPos* PC_TurnLeft(void);
void PC_TurnRight(void);
void PC_TurnAround(void);
sPos* PC_GetPosition(void);

#endif
