#include "DistanceSensorInterface.h"

#include <inc/hw_memmap.h>
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"

void InitializeDistanceSensors(uint32_t SysClk){
    //Enable the i2c module and wait
     SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);
     SysCtlDelay(3);

     I2CSlaveEnable(I2C1_BASE);
     I2CSlaveInit(I2C1_BASE, 0x29);

     //Enable I2C Loopback
     I2CLoopbackEnable(I2C1_BASE);

     //Initialize the I2C Master module
     I2CMasterInitExpClk(I2C1_BASE, SysClk, false);

     //Shutdown all devices
     GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, GPIO_PIN_3);
     GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, GPIO_PIN_4);
     GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, GPIO_PIN_5);

     //Enable one device
     //GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0);

     while(I2CMasterBusy(I2C1_BASE)){

     }
     uint32_t data = 0, step = 0;

     //Write 0x0000 to the device
     I2CMasterSlaveAddrSet(I2C1_BASE, 0x29, false);

     I2CMasterDataPut(I2C1_BASE, 0x000 >> 8);
     I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_START);

     I2CMasterDataPut(I2C1_BASE, 0x000);
     I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);

     //Receive
     I2CMasterSlaveAddrSet(I2C1_BASE, 0x29, true);
     I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);

     while(!(I2CSlaveStatus(I2C1_BASE) & I2C_SLAVE_ACT_TREQ)){

     }

     I2CSlaveDataPut(I2C1_BASE, 0xB4);
     I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
     while(!(I2CSlaveStatus(I2C1_BASE) & I2C_SLAVE_ACT_TREQ)){

     }
     //while(I2CMasterBusy(I2C1_BASE)){

     //}

     SysCtlDelay(SysClk / 3);

     data = I2CMasterDataGet(I2C1_BASE);
     step = 1;
     step = 2;

     /* Transmit three bytes
     while(I2CMasterBusy(I2C1_BASE)){

     }
     uint32_t data = 0;

     //Start a write to the device
     I2CMasterSlaveAddrSet(I2C1_BASE, 0x29, false);

     I2CMasterDataPut(I2C1_BASE, 0xFA);
     I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_START);
     while(!(I2CSlaveStatus(I2C1_BASE) & I2C_SLAVE_ACT_RREQ)){

     }
     data = I2CSlaveDataGet(I2C1_BASE);

     I2CMasterDataPut(I2C1_BASE, 0x11);
     I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);
     while(!(I2CSlaveStatus(I2C1_BASE) & I2C_SLAVE_ACT_RREQ)){

     }
     data = I2CSlaveDataGet(I2C1_BASE);

     I2CMasterDataPut(I2C1_BASE, 0x20);
     I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);
     while(!(I2CSlaveStatus(I2C1_BASE) & I2C_SLAVE_ACT_RREQ)){

     }
     data = I2CSlaveDataGet(I2C1_BASE);

     I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_STOP);



     data = I2CSlaveDataGet(I2C1_BASE);
     data = I2CSlaveDataGet(I2C1_BASE);
     data = I2CSlaveDataGet(I2C1_BASE);
     data = I2CSlaveDataGet(I2C1_BASE);
     //Adds extra places to single step through
     uint32_t step = 0;
     step = 1;
     step = 2;
     */
}
