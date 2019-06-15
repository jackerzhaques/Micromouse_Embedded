#include "Robot.h"

//Standard includes
#include <math.h>

//Project includes
#include "ControlLoop/ControlLoop.h"
#include "Interfaces/EncoderInterface.h"
#include "ControlLoop/ControlGlobals.h"

//Tivaware includes

//Forward declarations
Encoder* GetEncoders(void);
void ResetTarget(void);
void ResetSpeed(void);
void WaitForTarget(int32_t nTicks);

static Map map;

void InitializeRobot(uint32_t sysClk){
    //Initialize map
    map.x = 0;
    map.y = 0;
    map.orientation = 0;

    InitializeControlLoop(sysClk);
}

void Drive(float Delta){
    Encoder *encoders[2] = {GetLeftEncoder(), GetRightEncoder()};

    //Reset the target and drive until the robot reaches it
    ResetTarget();

    if(Delta > 0){
        SetLeftWheelSpeed(350);
        SetRightWheelSpeed(350);
    }
    else if(Delta < 0){
        SetLeftWheelSpeed(-350);
        SetRightWheelSpeed(-350);
    }
    else{
        //Do nothing
    }

    WaitForTarget(Delta);

    //Stop the robot
    ResetSpeed();

    //Store the position data
    float avgPos = (encoders[0]->position + encoders[1]->position) / 2;
    float dx = cos(map.orientation) * avgPos;
    float dy = sin(map.orientation) * avgPos;
    map.x += dx;
    map.y += dy;

    //Reset the target
    ResetEncoders();
}

void TurnLeft(void){

}

void TurnRight(void){

}

void TurnAround(void){

}

void Turn(float Degrees, bool isClockwise){

}

void Resetmap(void){

}

Encoder* GetEncoders(void){
    Encoder *encoders[2] = {GetLeftEncoder(),GetRightEncoder()};
    return encoders;
}

void ResetTarget(void){
    ResetSpeed();
    ResetEncoders();
}

void ResetSpeed(void){
    SetLeftWheelSpeed(0);
    SetRightWheelSpeed(0);
}

void WaitForTarget(int32_t nTicks){
    //TODD - Refactoring and designing Robot
//    Encoder *encoders[2] = GetEncoders();
//
//    while(
//            fabs(encoders[0]->ticks) < fabs(Delta) - 1 ||
//            fabs(encoders[1]->ticks) < fabs(Delta) - 1
//         )
//    {
//
//    }
}
