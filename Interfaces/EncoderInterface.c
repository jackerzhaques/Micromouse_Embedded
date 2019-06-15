#include "EncoderInterface.h"

//Project includes
#include "ControlLoop/ControlLoop.h"
#include "ControlLoop/ControlGlobals.h"

//Tivaware includes
#include <inc/hw_memmap.h>
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include <driverlib/gpio.h>
#include <driverlib/qei.h>

#define TIMER_FREQUENCY 1000

volatile static float FILTER_WEIGHT = 0.003;    //Keep 0.3% of the new value

//Encoder structs
volatile static Encoder LeftEncoder;
volatile static Encoder RightEncoder;

void LeftEncoder_ISR(void){
    //Capture the values from the ISR
    int32_t ticks = LeftEncoder.dir * QEIPositionGet(QEI1_BASE);
    LeftEncoder.ticks += ticks;
    LeftEncoder.position += ticks * MM_PER_TICK;
    //LeftEncoder.degrees += ticks * DEGREES_PER_TICK;

    //Reset the position
    QEIPositionSet(QEI1_BASE, 0);

    //Convert the ticks to speed
    float vel = MM_PER_TICK * ticks * TIMER_FREQUENCY;

    //Filter the velocity
    LeftEncoder.speed =
            ((1 - FILTER_WEIGHT) * LeftEncoder.speed) +
            (FILTER_WEIGHT * vel);

    //Clear the interrupt
    QEIIntClear(QEI1_BASE, QEI_INTTIMER);
}

void RightEncoder_ISR(void){
    //Capture the values from the ISR
    int32_t ticks = RightEncoder.dir * QEIPositionGet(QEI0_BASE);
    RightEncoder.ticks += ticks;
    RightEncoder.position += ticks * MM_PER_TICK;
    //LeftEncoder.degrees += ticks * DEGREES_PER_TICK;

    //Reset the position
    QEIPositionSet(QEI0_BASE, 0);

    //Convert the ticks to speed
    float vel = MM_PER_TICK * ticks * TIMER_FREQUENCY;

    //Filter the velocity
    RightEncoder.speed =
            ((1 - FILTER_WEIGHT) * RightEncoder.speed) +
            (FILTER_WEIGHT * vel);

    //Clear the interrupt
    QEIIntClear(QEI0_BASE, QEI_INTTIMER);
}

void InitializeEncoders(uint32_t systemClk){
    //Initialize the encoder structs
    LeftEncoder.ticks       = 0;
    LeftEncoder.position    = 0;
    LeftEncoder.speed       = 0;
    LeftEncoder.degrees     = 0;
    LeftEncoder.dir         = 1;

    RightEncoder.ticks      = 0;
    RightEncoder.position   = 0;
    RightEncoder.speed      = 0;
    RightEncoder.degrees    = 0;
    RightEncoder.dir        = 1;

    //Enable and configure QEI1 for left encoder ticks
    SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI1);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_QEI1)){

    }
    QEIConfigure(QEI1_BASE,
                 QEI_CONFIG_CAPTURE_A |
                 QEI_CONFIG_NO_RESET |
                 QEI_CONFIG_NO_SWAP |
                 QEI_CONFIG_CLOCK_DIR,
                 0xFFFFFFFF);
    QEIFilterConfigure(QEI1_BASE, QEI_FILTCNT_17);  //17 sys clocks for filter
    QEIVelocityConfigure(QEI1_BASE, QEI_VELDIV_1, systemClk / TIMER_FREQUENCY);
    QEIIntRegister(QEI1_BASE, LeftEncoder_ISR);

    QEIIntEnable(QEI1_BASE, QEI_INTTIMER);
    QEIFilterEnable(QEI1_BASE);
    QEIVelocityEnable(QEI1_BASE);
    QEIEnable(QEI1_BASE);

    //Enable and configure QEI0 for right encoder ticks
    SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_QEI0)){

    }
    QEIConfigure(QEI0_BASE,
                 QEI_CONFIG_CAPTURE_A |
                 QEI_CONFIG_NO_RESET |
                 QEI_CONFIG_NO_SWAP |
                 QEI_CONFIG_CLOCK_DIR,
                 0xFFFFFFFF);
    QEIFilterConfigure(QEI0_BASE, QEI_FILTCNT_17);  //17 sys clocks for filter
    QEIVelocityConfigure(QEI0_BASE, QEI_VELDIV_1, systemClk / TIMER_FREQUENCY);
    QEIIntRegister(QEI0_BASE, RightEncoder_ISR);

    QEIIntEnable(QEI0_BASE, QEI_INTTIMER);
    QEIFilterEnable(QEI0_BASE);
    QEIVelocityEnable(QEI0_BASE);
    QEIEnable(QEI0_BASE);
}

void ResetEncoders(void){
    LeftEncoder.degrees = 0;
    LeftEncoder.ticks = 0;
    LeftEncoder.position = 0;

    RightEncoder.degrees = 0;
    RightEncoder.ticks = 0;
    RightEncoder.position = 0;
}

Encoder* GetLeftEncoder(void){
    return (Encoder*)&LeftEncoder;
}

Encoder* GetRightEncoder(void){
    return (Encoder*)&RightEncoder;
}
