#include "EncoderInterface.h"

#define FILTER_WEIGHT   0.25

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

#define TIMER_MAX_COUNT 0xFFFFFF    //Value determined experimentally
#define SYS_CLK         80000000

//Encoder structs
volatile static Encoder LeftEncoder;
volatile static Encoder RightEncoder;

void LeftEncoder_ISR(void){
    //Capture the values from the ISR
    LeftEncoder.ticks = QEIPositionGet(QEI1_BASE);
    uint32_t velocityTicks = QEIVelocityGet(QEI1_BASE);

    //Convert the velocity ticks to speed and lp filter the value
    float vel = velocityTicks / ((float)TIMER_MAX_COUNT / (float)SYS_CLK);
    vel = ((1 - FILTER_WEIGHT) * vel) + (FILTER_WEIGHT * LeftEncoder.speed);
    LeftEncoder.speed = vel;

    //Clear the interrupt
    QEIIntClear(QEI1_BASE, QEI_INTTIMER);
}

void RightEncoder_ISR(void){
    //Capture the values from the ISR
    RightEncoder.ticks = QEIPositionGet(QEI0_BASE);
    uint32_t velocityTicks = QEIVelocityGet(QEI0_BASE);

    //Convert the velocity ticks to speed and lp filter the value
    float vel = velocityTicks / ((float)TIMER_MAX_COUNT / (float)SYS_CLK);
    vel = ((1 - FILTER_WEIGHT) * vel) + (FILTER_WEIGHT * RightEncoder.speed);
    RightEncoder.speed = vel;

    //Clear the interrupt
    QEIIntClear(QEI0_BASE, QEI_INTTIMER);
}

void InitializeEncoders(void){
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
    QEIVelocityConfigure(QEI1_BASE, QEI_VELDIV_1, TIMER_MAX_COUNT);
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
    QEIVelocityConfigure(QEI0_BASE, QEI_VELDIV_1, TIMER_MAX_COUNT);
    QEIIntRegister(QEI0_BASE, RightEncoder_ISR);

    QEIIntEnable(QEI0_BASE, QEI_INTTIMER);
    QEIFilterEnable(QEI0_BASE);
    QEIVelocityEnable(QEI0_BASE);
    QEIEnable(QEI0_BASE);
}

Encoder* GetLeftEncoder(void){
    return (Encoder*)&LeftEncoder;
}

Encoder* GetRightEncoder(void){
    return (Encoder*)&RightEncoder;
}
