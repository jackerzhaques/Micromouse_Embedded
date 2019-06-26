#include "I2C.h"

#include <inc/hw_memmap.h>
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"

void InitializeI2C(uint32_t SysClk){
    //Enable the i2c module and wait
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);
    SysCtlDelay(3);

    //Initialize the I2C Master module
    I2CMasterInitExpClk(I2C1_BASE, SysClk, false);
}

void I2CWrite(uint16_t reg, uint8_t byte, uint8_t slaveAddress){
    uint8_t upperByte = (reg & 0xFF00) >> 8;
    uint8_t lowerByte = (reg & 0x00FF);
    I2CMasterSlaveAddrSet(I2C1_BASE, slaveAddress, false);

    I2CMasterDataPut(I2C1_BASE, upperByte);
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_START);

    while(I2CMasterBusy(I2C1_BASE));

    I2CMasterDataPut(I2C1_BASE, lowerByte);
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);

    while(I2CMasterBusy(I2C1_BASE));

    I2CMasterDataPut(I2C1_BASE, byte);
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);

    while(I2CMasterBusy(I2C1_BASE));
}

uint8_t I2CRead(uint16_t reg, uint8_t slaveAddress){
    uint8_t data = 0;
    uint8_t upperByte = (reg & 0xFF00) >> 8;
    uint8_t lowerByte = (reg & 0x00FF);
    I2CMasterSlaveAddrSet(I2C1_BASE, slaveAddress, false);

    I2CMasterDataPut(I2C1_BASE, upperByte);
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_START);

    while(I2CMasterBusy(I2C1_BASE));

    I2CMasterDataPut(I2C1_BASE, lowerByte);
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);

    while(I2CMasterBusy(I2C1_BASE));

    I2CMasterSlaveAddrSet(I2C1_BASE, slaveAddress, true);
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);

    while(I2CMasterBusy(I2C1_BASE));

    data = I2CMasterDataGet(I2C1_BASE);

    return data;
}
