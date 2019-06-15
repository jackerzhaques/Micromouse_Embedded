#include "ControlLoop.h"

//Project includes
#include "Interfaces/EncoderInterface.h"
#include "Interfaces/MotorInterface.h"
#include "ControlLoop/ControlGlobals.h"

//Standard includes
#include <stdint.h>
#include <stdbool.h>

//Tivaware includes
#include <inc/hw_memmap.h>
#include <inc/hw_ints.h>
#include <driverlib/sysctl.h>
#include <driverlib/timer.h>
#include <driverlib/interrupt.h>

volatile static PID* PIDs;

//Forward declarations
void ControlSample_ISR(void);
void ControlLoop_ISR(void);

void InitializeControlLoop(uint32_t sysClk){
    //Initialize PID structs

    //Initialize other peripherals
    InitializeMotors(sysClk);
    InitializeEncoders(sysClk);

    //Initialize Timer 1 for the control loop ISR
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
    SysCtlDelay(10);

    TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);

    TimerLoadSet(TIMER1_BASE, TIMER_A, sysClk / CONTROL_LOOP_FREQUENCY);

    TimerIntRegister(TIMER1_BASE, TIMER_A, ControlLoop_ISR);
    IntEnable(INT_TIMER1A);
    TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

    TimerEnable(TIMER1_BASE, TIMER_A);

    //Grab a reference to all PID structs
    PIDs = GetControlPIDs();
}

void SetLeftWheelSpeed(float Speed){
    //TODO: Bound
    PIDs[LEFT_WHEEL_SPEED_LOOP].target = Speed;
}
void SetRightWheelSpeed(float Speed){
    //TODO: Bound
    PIDs[RIGHT_WHEEL_SPEED_LOOP].target = Speed;
}

void SetLeftWheelTarget(float Target){
    PIDs[LEFT_WHEEL_POSITION_LOOP].target = Target;
}

void SetRightWheelTarget(float Target){
    PIDs[RIGHT_WHEEL_POSITION_LOOP].target = Target;
}

void ControlLoop_ISR(void){
    uint8_t i;

    for(i = 0; i < N_CONTROL_LOOPS; i++){
        //Measure error
        float error = 0;
        switch(i){
            case LEFT_WHEEL_SPEED_LOOP:
                error = PIDs[i].target - GetLeftEncoder()->speed;
                break;
            case RIGHT_WHEEL_SPEED_LOOP:
                error = PIDs[i].target - GetRightEncoder()->speed;
                break;
            case LEFT_WHEEL_POSITION_LOOP:
                error = PIDs[i].target - GetLeftEncoder()->ticks;
                break;
            case RIGHT_WHEEL_POSITION_LOOP:
                error = PIDs[i].target - GetRightEncoder()->ticks;
                break;
            default:
                //do nothing
                break;
        }

        //Calculate coefficients
        float P, I, D;
        P = PIDs[i].Kp * error;
        I = PIDs[i].iState + (PIDs[i].Ki * error);
        D = PIDs[i].Kd * (error - PIDs[i].dState);

        //Bound integral
        if(I > PIDs[i].iMax){
            I = PIDs[i].iMax;
        }
        else if(I < PIDs[i].iMin){
            I = PIDs[i].iMin;
        }
        else{
            //Do nothing
        }

        //Calculate output
        PIDs[i].output = P + I + D;

        //Bounds for speed loop only
        if(i == LEFT_WHEEL_SPEED_LOOP || i == RIGHT_WHEEL_SPEED_LOOP){
            //Default to 0 if target is 0
            if(PIDs[i].target == 0){
                PIDs[i].output = 0;
            }
            else{
                //Do nothing
            }

            //Bound by direction
            if(PIDs[i].output < 0 && PIDs[i].target > 0){
                PIDs[i].output = 0;
            }
            else if(PIDs[i].output > 0 & PIDs[i].target < 0){
                PIDs[i].output = 0;
            }
            else{
                //Do nothing
            }
        }

        //Bound output by maximum
        if(PIDs[i].output > PIDs[i].outputMax){
            PIDs[i].output = PIDs[i].outputMax;
        }
        else if(PIDs[i].output < PIDs[i].outputMin){
            PIDs[i].output = PIDs[i].outputMin;
        }
        else{
            //Do nothing
        }

        //Assign output
        switch(i){
            case LEFT_WHEEL_SPEED_LOOP:
                if(PIDs[i].output < 0){
                    SetLeftMotorDirection(false);
                }
                else if(PIDs[i].output > 0){
                    SetLeftMotorDirection(true);
                }
                else{
                    //Leave at previous state
                }

                SetLeftMotorDutyCycle(fabs(PIDs[i].output));
                break;
            case RIGHT_WHEEL_SPEED_LOOP:
                if(PIDs[i].output < 0){
                    SetRightMotorDirection(true);   //Opposite of left wheel
                }
                else if(PIDs[i].output > 0){
                    SetRightMotorDirection(false);  //Opposite of left wheel
                }
                else{
                    //Leave at previous state
                }

                SetRightMotorDutyCycle(fabs(PIDs[i].output));
                break;
            case LEFT_WHEEL_POSITION_LOOP:
                //PIDs[LEFT_WHEEL_SPEED_LOOP].target = PIDs[i].output;
                break;
            case RIGHT_WHEEL_POSITION_LOOP:
                //PIDs[RIGHT_WHEEL_SPEED_LOOP].target = PIDs[i].output;
                break;
            default:
                //Do nothing
                break;
        }

        //Save states
        PIDs[i].iState = I;
        PIDs[i].dState = error;
    }

    TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
}
