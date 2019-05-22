#include "EncoderInterface.h"

//Tivaware includes
#include <inc/hw_memmap.h>
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include <driverlib/gpio.h>

#define TIMER_MAX_COUNT 80000000

//Encoder structs
volatile static Encoder LeftEncoder;
volatile static Encoder RightEncoder;

void LeftEncoderUpdate_ISR(void){
    LeftEncoder.ticks++;

    TimerIntClear(TIMER1_BASE, TIMER_CAPB_EVENT);
}

void RightEncoderUpdate_ISR(void){
    uint32_t TimerVal = TimerValueGet(TIMER1_BASE, TIMER_B);
    int32_t TimerDiff = LeftEncoder.lastTimerVal - TimerVal;
    LeftEncoder.lastTimerVal = TimerVal;

    if(TimerDiff < 0){
        TimerDiff += 0xFFFFFFFF;
    }
    float speed = 80000000 / TimerDiff;

    LeftEncoder.speed = speed;
    LeftEncoder.ticks++;

    TimerIntClear(TIMER1_BASE, TIMER_CAPA_EVENT);
}

void EncoderAnyISR(void){

}

void InitializeEncoders(void){
    //IntEnable(INT_GPIOF);
    /*
    //Enable timer 1 for speed timeout
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);

    //Configure the timer to be a split 24-bit timer.
    //Enable time captures for both splits
    TimerConfigure(TIMER1_BASE, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_CAP_TIME | TIMER_CFG_A_CAP_TIME));

    //Set the event to be falling edge triggered
    TimerControlEvent(TIMER1_BASE, TIMER_B, TIMER_EVENT_NEG_EDGE);
    TimerControlEvent(TIMER1_BASE, TIMER_A, TIMER_EVENT_NEG_EDGE);

    //Configure the timers to be 24 bit by giving them 8 bits of prescale
    TimerPrescaleSet(TIMER1_BASE, TIMER_B, 0xFF);
    TimerPrescaleSet(TIMER1_BASE, TIMER_A, 0xFF);

    //Register the ISRs
    TimerIntRegister(TIMER1_BASE, TIMER_B, LeftEncoderUpdate_ISR);
    TimerIntRegister(TIMER1_BASE, TIMER_A, RightEncoderUpdate_ISR);

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
    */
}

Encoder* GetLeftEncoder(void){
    return (Encoder*)&LeftEncoder;
}

Encoder* GetRightEncoder(void){
    return (Encoder*)&RightEncoder;
}
