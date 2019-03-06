#include "SpeedControl.h"
#include "ControlGlobals.h"
#include "Drivers/MotorDriver.h"

#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "inc/hw_types.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"

//TODO: Remove debug include
#include "utils/uartstdio.h"

//Forward Declarations
//...

MotorDir LeftMotorDir = STOPPED;
MotorDir RightMotorDir = STOPPED;

static sPID SpeedPIDs[] = {
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
};

//ISRs
void ControlLoopISR(void){
    static int32_t LastLeftEncoderCount = 0;
    static int32_t LastRightEncoderCount = 0;

    //Grab the latest speed data
    float LeftSpeed = MD_GetLeftEncoderSpeed();
    float RightSpeed = MD_GetRightEncoderSpeed();

    if(LastLeftEncoderCount == MD_GetLeftEncoderTicks()){
        LeftSpeed = 0;
    }
    if(LastRightEncoderCount == MD_GetRightEncoderTicks()){
        RightSpeed = 0;
    }

    LastLeftEncoderCount = MD_GetLeftEncoderTicks();
    LastRightEncoderCount = MD_GetRightEncoderTicks();

    //Calculate the outputs
    UpdatePID(&SpeedPIDs[LEFT_WHEEL], LeftSpeed);
    UpdatePID(&SpeedPIDs[RIGHT_WHEEL], RightSpeed);

    //Set the directions
    MD_SetLeftMotorDir(LeftMotorDir);
    MD_SetRightMotorDir(RightMotorDir);

    //Set the outputs
    MD_SetLeftMotorDutyCycle(SpeedPIDs[LEFT_WHEEL].Output);
    MD_SetRightMotorDutyCycle(SpeedPIDs[RIGHT_WHEEL].Output);

    TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
}

void SC_Initialize(void){
    //Initialize Timer2
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);

    //Wait for the clock to stabilize
    SysCtlDelay(10);

    //Configure the timer to be periodic
    TimerConfigure(TIMER2_BASE, TIMER_CFG_PERIODIC);

    //Configure the timer to have a 50ms period
    TimerLoadSet(TIMER2_BASE, TIMER_A, 0.1 * SysCtlClockGet());

    //Register the timer ISR
    TimerIntRegister(TIMER2_BASE, TIMER_A, ControlLoopISR);

    //Enable interrupts for the timer
    IntEnable(INT_TIMER2A);
    TimerIntEnable(TIMER2_BASE, TIMER_TIMA_TIMEOUT);

    //Enable the timer
    TimerEnable(TIMER2_BASE, TIMER_A);
}

void SC_SetLeftWheelSpeed(float Speed){
    SpeedPIDs[LEFT_WHEEL].Target = abs(Speed);
    if(Speed < 0){
        LeftMotorDir = REV;
    }
    else if(Speed > 0){
        LeftMotorDir = FWD;
    }
    else{
        LeftMotorDir = STOPPED;
    }
}


void SC_SetRightWheelSpeed(float Speed){
    SpeedPIDs[RIGHT_WHEEL].Target = abs(Speed);
    if(Speed < 0){
        RightMotorDir = REV;
    }
    else if(Speed > 0){
        RightMotorDir = FWD;
    }
    else{
        RightMotorDir = STOPPED;
    }
}
