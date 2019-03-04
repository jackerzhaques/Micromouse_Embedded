#include <stdint.h>
#include <stdbool.h>
#include <inc/hw_memmap.h>
#include <inc/hw_types.h>
#include <driverlib/debug.h>
#include <driverlib/fpu.h>
#include <driverlib/pin_map.h>
#include <driverlib/rom.h>
#include <driverlib/sysctl.h>
#include <driverlib/uart.h>
#include <driverlib/gpio.h>
#include "utils/uartstdio.h"
#include "pinout.h"

#include "Drivers/MotorDriver.h"
#include "Controls/PositionControl.h"
#include "Controls/SpeedControl.h"

void SystemInit(void);
void UARTInit(void);

int main(){
    SystemInit();
    PC_Initialize();

    UARTprintf("System Initialized\n");


    char str[30];
    sprintf(str, "%u\n", SysCtlClockGet());
    UARTprintf(str);

    for(;;){
        PC_TurnLeft();
        SysCtlDelay(SysCtlClockGet());
    }
}

void SystemInit(void){
    //Enable lazy stacking for ISRs. Allows using floats in ISRs.
    FPULazyStackingEnable();

    //Set the clock to run at 80MHz
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    //Initialize the UART
    UARTInit();

    //Initialize pins
    PinoutSet();
}

void UARTInit(void){
    //Enable the GPIO and UART peripherals
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    //Configure GPIO Pins for UART Mode
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //Use the internal 16MHz oscillator for the UART
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    //Initialize the UART for console I/O
    UARTStdioConfig(0, 115200, 16000000);
}
