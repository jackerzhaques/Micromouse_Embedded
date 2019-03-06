#include "PositionControl.h"
#include "Drivers/MotorDriver.h"
#include "SpeedControl.h"
#include "ControlGlobals.h"

sPos RobotPosition;
sPos TargetPosition;

Direction RobotDir = NORTH;

static sPID PositionPIDs[] = {
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

//Forward declarations
void MoveToTarget(void);
Direction RobotDirTurnedLeft(void);
Direction RobotDirTurnedRight(void);
Direction RobotDirTurnedAround(void);

void PC_Initialize(void){
    //Default the position to 0,0
    RobotPosition.x = 0;
    RobotPosition.y = 0;

    //Initialize the target to 0,0
    TargetPosition.x = 0;
    TargetPosition.y = 0;

    //Initialize the motor driver
    MD_Initialize();

    //Initialize speed control
    SC_Initialize();

    //Initialize the speed to 0
    SC_SetLeftWheelSpeed(0);
    SC_SetRightWheelSpeed(0);
}

void PC_MoveForward(void){
    switch(RobotDir){
        case NORTH:
            TargetPosition.y++;
            break;
        case SOUTH:
            TargetPosition.y--;
            break;
        case EAST:
            TargetPosition.x++;
            break;
        case WEST:
            TargetPosition.x--;
            break;
    }
}

void ConfigurePIDForward(sPID* PID){
    PID->OutputMin = 0;
    PID->OutputMax = TURN_SPEED;
}

void ConfigurePIDReverse(sPID* PID){
    PID->OutputMin = -TURN_SPEED;
    PID->OutputMax = 0;
}

sPos* PC_TurnLeft(void){
    //Preset the PID
    sPID* LeftPID = &PositionPIDs[LEFT_TURN];
    sPID* RightPID = &PositionPIDs[RIGHT_TURN];
    LeftPID->Target = -TICKS_IN_90_DEGREES;
    RightPID->Target = TICKS_IN_90_DEGREES;
    ConfigurePIDForward(RightPID);
    ConfigurePIDReverse(LeftPID);

    //Reset the encoder ticks so that they start from 0 when turning
    MD_ResetLeftEncoderTicks();
    MD_ResetRightEncoderTicks();

    bool TurnComplete = false;
    while(!TurnComplete){
        //Get the new encoder ticks
        int32_t LeftTicks = MD_GetLeftEncoderTicks();
        int32_t RightTicks = MD_GetRightEncoderTicks();

        //Calculate the PID outputs
        UpdatePID(LeftPID, LeftTicks);
        UpdatePID(RightPID, RightTicks);

        //Update the wheel speeds
        SC_SetLeftWheelSpeed(LeftPID->Output);
        SC_SetRightWheelSpeed(RightPID->Output);

        //End the loop if both wheels are 'close enough' to the target
        if(TICKS_IN_90_DEGREES - abs(LeftTicks) < 5 &&
                TICKS_IN_90_DEGREES - abs(RightTicks) < 5){
            TurnComplete = true;
        }
    }

    //Change the direction
    RobotDir = RobotDirTurnedLeft();

    //Calculate the robot's new position

    return &RobotPosition;
}

void PC_TurnRight(void){

}

void PC_TurnAround(void){

}

Direction RobotDirTurnedLeft(void){
    switch(RobotDir){
    case NORTH:
        return WEST;
    case WEST:
        return SOUTH;
    case SOUTH:
        return EAST;
    case EAST:
        return NORTH;
    };

    return NORTH;
}

Direction RobotDirTurnedRight(void){
    switch(RobotDir){
    case NORTH:
        return WEST;
    case WEST:
        return SOUTH;
    case SOUTH:
        return EAST;
    case EAST:
        return NORTH;
    };

    return NORTH;
}
Direction RobotDirTurnedAround(void){
    switch(RobotDir){
    case NORTH:
        return SOUTH;
    case SOUTH:
        return NORTH;
    case WEST:
        return EAST;
    case EAST:
        return WEST;
    };

    return NORTH;
}
