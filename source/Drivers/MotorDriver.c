#include "MotorDriver.h"
#include <stdint.h>
#include <stdbool.h>
#include <inc/hw_memmap.h>
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "driverlib/pwm.h"
#include "utils/uartstdio.h"

typedef struct sEncoder_tag{
    int32_t    Count;
    int32_t    LastTimerVal;

    float      Speed;
    bool       IntReceived;

} sEncoder;

sEncoder LeftEncoder    = {0,0,0,0};
sEncoder RightEncoder   = {0,0,0,0};

MotorDir LeftMotorState = FWD;
MotorDir RightMotorState = FWD;

//Forward declarations
void EnablePWM(void);
void EnableEncoderInterrupts(void);
void EnableTimer1(void);

/*
 * ISRs
 */
void LeftEncoderISR(void){
    uint32_t TimerVal = (TimerValueGet(TIMER1_BASE, TIMER_B));
    int32_t TimerDiff = LeftEncoder.LastTimerVal - TimerVal;
    if(TimerDiff < 0){
        TimerDiff += 0xFFFFFF;
    }
    float Speed = (80000000.0) / TimerDiff;

    if(LeftMotorState == FWD){
        LeftEncoder.Speed = Speed;
        LeftEncoder.Count++;
    }
    else if(LeftMotorState == REV){
        LeftEncoder.Speed = -Speed;
        LeftEncoder.Count--;
    }

    LeftEncoder.LastTimerVal = TimerVal;

    TimerIntClear(TIMER1_BASE, TIMER_CAPB_EVENT);
}

void RightEncoderISR(void){
    uint32_t TimerVal = TimerValueGet(TIMER1_BASE, TIMER_A);
    int32_t TimerDiff = RightEncoder.LastTimerVal - TimerVal;
    if(TimerDiff < 0){
        TimerDiff += 0xFFFFFF;
    }
    float Speed = 80000000.0 / TimerDiff;

    if(RightMotorState == FWD){
        RightEncoder.Speed = Speed;
        RightEncoder.Count++;
    }
    else if(RightMotorState == REV){
        RightEncoder.Speed = -Speed;
        RightEncoder.Count--;
    }

    RightEncoder.LastTimerVal = TimerVal;
    TimerIntClear(TIMER1_BASE, TIMER_CAPA_EVENT);
}

void Timer1ISR(void){

    /*
    //If an encoder did not generate an interrupt since the last timeout
    // then assume the encoder isn't moving
    if(LeftEncoder.IntReceived == 0){
        LeftEncoder.Speed = 0;
    }
    else{
        LeftEncoder.IntReceived = 0;
    }
    if(RightEncoder.IntReceived == 0){
        RightEncoder.Speed = 0;
    }
    else{
        RightEncoder.IntReceived = 0;
    }
    */
    UARTprintf("event");

    TimerIntClear(TIMER1_BASE, TIMER_CAPA_EVENT);
}

void MD_Initialize(void){
    //Direction pins should already be set using pinout.h
    MD_SetLeftMotorDir(STOPPED);
    MD_SetRightMotorDir(STOPPED);

    //Enable motor PWM
    EnablePWM();

    //Enable timer 1
    EnableTimer1();

    //Enable master interrupts
    IntMasterEnable();
}

//Initializes PWM on PB6 and PB7
void EnablePWM(void){
    //Set the PWM clock
    SysCtlPWMClockSet(SYSCTL_PWMDIV_1);

    //Enable the PWM peripheral and wait
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
    SysCtlDelay(1);

    //Configure PWM0 to count up/down without synchronization
    PWMGenConfigure(PWM_BASE, PWM_GEN,
                    PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);

    //Set the PWM frequency to 1250Hz
    //N = (1 / f) * SysClk, where n is the function parameter
    // and f is the PWM frequency
    PWMGenPeriodSet(PWM_BASE, PWM_GEN, 64000);

    //Default the duty cycle to 0%
    MD_SetLeftMotorDutyCycle(0.0);
    MD_SetRightMotorDutyCycle(0.0);

    //Enable the PWM signals (Bit 6 and Bit 7 on PortB)
    PWMOutputState(PWM_BASE, PWM_OUT_0_BIT | PWM_OUT_1_BIT, true);

    //Enable the PWM generator block
    PWMGenEnable(PWM_BASE, PWM_GEN);
}

void EnableTimer1(void){
    //Enable the timer peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);

    //Configure the timer to be a split 24-bit timer. Enable Time captures for both timers
    TimerConfigure(TIMER1_BASE, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_CAP_TIME | TIMER_CFG_A_CAP_TIME));

    //Set the event to be falling edge triggered
    TimerControlEvent(TIMER1_BASE, TIMER_B, TIMER_EVENT_NEG_EDGE);
    TimerControlEvent(TIMER1_BASE, TIMER_A, TIMER_EVENT_NEG_EDGE);

    //Configure the timers to be 24 bit by giving them 8 bits of prescale
    TimerPrescaleSet(TIMER1_BASE, TIMER_B, 0xFF);
    TimerPrescaleSet(TIMER1_BASE, TIMER_A, 0xFF);

    //Register the ISRs
    TimerIntRegister(TIMER1_BASE, TIMER_B, LeftEncoderISR);
    TimerIntRegister(TIMER1_BASE, TIMER_A, RightEncoderISR);

    //Clear the event interrupts
    TimerIntClear(TIMER1_BASE, TIMER_CAPB_EVENT);
    TimerIntClear(TIMER1_BASE, TIMER_CAPA_EVENT);

    //Enable the event interrupts
    TimerIntEnable(TIMER1_BASE, TIMER_CAPB_EVENT);
    TimerIntEnable(TIMER1_BASE, TIMER_CAPA_EVENT);

    //Enable the interrupts for the timer module
    IntEnable(INT_TIMER1B);
    IntEnable(INT_TIMER1A);

    //Enable both timers
    TimerEnable(TIMER1_BASE, TIMER_B);
    TimerEnable(TIMER1_BASE, TIMER_A);
}

//Sets the left motor's duty cycle
void MD_SetLeftMotorDutyCycle(float DutyCycle){
    //Constrain the duty cycle to a value between 0 and 1
    if(DutyCycle >= 1){
        DutyCycle = 0.99;//Duty cycle of 1 doesn't work for some reason
    }
    else if(DutyCycle < 0){
        DutyCycle = 0;
    }
    //TODD: Not sure why but the PWM comparator will overflow if you don't subtract 2
    uint32_t PWMRegisterValue = (PWMGenPeriodGet(PWM_BASE, PWM_GEN)-2) * DutyCycle;
    PWMPulseWidthSet(PWM_BASE, LEFT_MOTOR_PWM, PWMRegisterValue);
}

//Sets the right motor's duty cycle
void MD_SetRightMotorDutyCycle(float DutyCycle){
    //Constrain the duty cycle to a value between 0 and 1
    if(DutyCycle >= 1){
        DutyCycle = 0.99;//Duty cycle of 1 doesn't work for some reason
    }
    else if(DutyCycle < 0){
        DutyCycle = 0;
    }
    //TODD: Not sure why but the PWM comparator will overflow if you don't subtract 2
    uint32_t PWMRegisterValue = (PWMGenPeriodGet(PWM_BASE, PWM_GEN)-2) * DutyCycle;
    PWMPulseWidthSet(PWM_BASE, RIGHT_MOTOR_PWM, PWMRegisterValue);
}

//Sets the left motor's direction
void MD_SetLeftMotorDir(MotorDir Direction){
    LeftMotorState = Direction;
    switch(Direction){
        case FWD:
            GPIOPinWrite(AIN1_BASE, AIN1_PIN, AIN1_PIN);
            GPIOPinWrite(AIN2_BASE, AIN2_PIN, 0x00);
            break;
        case REV:
            GPIOPinWrite(AIN1_BASE, AIN1_PIN, 0x00);
            GPIOPinWrite(AIN2_BASE, AIN2_PIN, AIN2_PIN);
            break;
        case STOPPED:
            GPIOPinWrite(AIN1_BASE, AIN1_PIN, 0x00);
            GPIOPinWrite(AIN2_BASE, AIN2_PIN, 0x00);
            break;
        case BRAKE:
            GPIOPinWrite(AIN1_BASE, AIN1_PIN, AIN1_PIN);
            GPIOPinWrite(AIN2_BASE, AIN2_PIN, AIN2_PIN);
            break;
        default:
            //Invalid direction, ignore it
            break;
    }
}

//Sets the right motor's direction
void MD_SetRightMotorDir(MotorDir Direction){
    RightMotorState = Direction;
    switch(Direction){
        case FWD:
            GPIOPinWrite(BIN1_BASE, BIN1_PIN, 0x00);
            GPIOPinWrite(BIN2_BASE, BIN2_PIN, BIN2_PIN);
            break;
        case REV:
            GPIOPinWrite(BIN1_BASE, BIN1_PIN, BIN1_PIN);
            GPIOPinWrite(BIN2_BASE, BIN2_PIN, 0x00);
            break;
        case STOPPED:
            GPIOPinWrite(BIN1_BASE, BIN1_PIN, 0x00);
            GPIOPinWrite(BIN2_BASE, BIN2_PIN, 0x00);
            break;
        case BRAKE:
            GPIOPinWrite(BIN1_BASE, BIN1_PIN, BIN1_PIN);
            GPIOPinWrite(BIN2_BASE, BIN2_PIN, BIN2_PIN);
            break;
        default:
            //Invalid direction, ignore it
            break;
    }
}

MotorDir MD_GetLeftMotorDir(void){
    return LeftMotorState;
}

MotorDir MD_GetRightMotorDir(void){
    return RightMotorState;
}

void MD_StopLeftMotor(void){
    MD_SetLeftMotorDir(STOPPED);
    MD_SetLeftMotorDutyCycle(0);
}

void MD_StopRightMotor(void){
    MD_SetRightMotorDir(STOPPED);
    MD_SetRightMotorDutyCycle(0);
}

int32_t MD_GetLeftEncoderTicks(void){
    return LeftEncoder.Count;
}

float MD_GetLeftEncoderSpeed(void){
    return LeftEncoder.Speed;
}
void MD_ResetLeftEncoderTicks(void){
    LeftEncoder.Count = 0;
}

int32_t MD_GetRightEncoderTicks(void){
    return RightEncoder.Count;
}

float MD_GetRightEncoderSpeed(void){
    return RightEncoder.Speed;
}
void MD_ResetRightEncoderTicks(void){
    RightEncoder.Count = 0;
}
