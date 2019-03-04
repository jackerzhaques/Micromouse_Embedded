#include "PositionControl.h"
#include "Drivers/MotorDriver.h"
#include "SpeedControl.h"
#include "ControlGlobals.h"

sPos RobotPosition;
sPos TargetPosition;

Direction RobotDir = NORTH;

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

sPos* PC_TurnLeft(void){
    //Preset the PID
    sPID* LeftPID = &PIDs[LEFT_TURN];
    sPID* RightPID = &PIDs[RIGHT_TURN];
    LeftPID->Target = TICKS_IN_90_DEGREES;
    RightPID->Target = -TICKS_IN_90_DEGREES;

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

        if(TICKS_IN_90_DEGREES - abs(LeftTicks) < 5 &&
                TICKS_IN_90_DEGREES - abs(RightTicks) < 5){
            TurnComplete = true;
        }
    }

    //Change the direction
    RobotDir = RobotDirTurnedLeft();

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
