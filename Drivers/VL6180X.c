#include "VL6180X.h"

//Tivaware includes
#include <inc/hw_memmap.h>
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"


void VL6180X_ReadDistance(VL6180X_Driver* driver){
    //Enable device
//    GPIOPinWrite(driver->GPIO_Port, driver->GPIO_Pin, driver->GPIO_Pin);
//
//    I2CMasterSlaveAddrSet(I2C1_BASE, driver->address, false);
//
//    I2CMasterDataPut(I2C1_BASE, 0x00);
//    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_START);
//
//    while(I2CMasterBusy(I2C1_BASE));
//
//    I2CMasterDataPut(I2C1_BASE, 0x18);
//    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);
//
//    while(I2CMasterBusy(I2C1_BASE));
//
//    I2CMasterDataPut(I2C1_BASE, 0x01);
//    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
//
//    while(I2CMasterBusy(I2C1_BASE));
//
//    bool valGood = false;
//    while(!valGood){
//        uint8_t result = 0;
//        I2CMasterDataPut(I2C1_BASE, 0x00);
//    }
//
//    I2CMasterSlaveAddrSet(I2C1_BASE, 0x29, true);
//    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
//
//    while(I2CMasterBusy(I2C1_BASE));
//
//    data = I2CMasterDataGet(I2C1_BASE);
}
