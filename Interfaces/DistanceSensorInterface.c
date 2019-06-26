#include "DistanceSensorInterface.h"

#include <inc/hw_memmap.h>
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"

#include "Drivers/I2C.h"

void InitializeDistanceSensors(uint32_t SysClk){
     //Shutdown all devices
     GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0);
     GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0);
     GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, 0);

     //Enable one device
     GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, GPIO_PIN_3);

     InitializeI2C(SysClk);

     //Initialize device

     // private settings from page 24 of app note
     I2CWrite(0x0207, 0x01, 0x29);
     I2CWrite(0x0208, 0x01, 0x29);
     I2CWrite(0x0096, 0x00, 0x29);
     I2CWrite(0x0097, 0xfd, 0x29);
     I2CWrite(0x00e3, 0x00, 0x29);
     I2CWrite(0x00e4, 0x04, 0x29);
     I2CWrite(0x00e5, 0x02, 0x29);
     I2CWrite(0x00e6, 0x01, 0x29);
     I2CWrite(0x00e7, 0x03, 0x29);
     I2CWrite(0x00f5, 0x02, 0x29);
     I2CWrite(0x00d9, 0x05, 0x29);
     I2CWrite(0x00db, 0xce, 0x29);
     I2CWrite(0x00dc, 0x03, 0x29);
     I2CWrite(0x00dd, 0xf8, 0x29);
     I2CWrite(0x009f, 0x00, 0x29);
     I2CWrite(0x00a3, 0x3c, 0x29);
     I2CWrite(0x00b7, 0x00, 0x29);
     I2CWrite(0x00bb, 0x3c, 0x29);
     I2CWrite(0x00b2, 0x09, 0x29);
     I2CWrite(0x00ca, 0x09, 0x29);
     I2CWrite(0x0198, 0x01, 0x29);
     I2CWrite(0x01b0, 0x17, 0x29);
     I2CWrite(0x01ad, 0x00, 0x29);
     I2CWrite(0x00ff, 0x05, 0x29);
     I2CWrite(0x0100, 0x05, 0x29);
     I2CWrite(0x0199, 0x05, 0x29);
     I2CWrite(0x01a6, 0x1b, 0x29);
     I2CWrite(0x01ac, 0x3e, 0x29);
     I2CWrite(0x01a7, 0x1f, 0x29);
     I2CWrite(0x0030, 0x00, 0x29);

     // Recommended : Public registers - See data sheet for more detail
     I2CWrite(0x0011, 0x10, 0x29);       // Enables polling for 'New Sample ready'
                                 // when measurement completes
     I2CWrite(0x010a, 0x30, 0x29);       // Set the averaging sample period
                                 // (compromise between lower noise and
                                 // increased execution time)
     I2CWrite(0x003f, 0x46, 0x29);       // Sets the light and dark gain (upper
                                 // nibble). Dark gain should not be
                                 // changed.
     I2CWrite(0x0031, 0xFF, 0x29);       // sets the # of range measurements after
                                 // which auto calibration of system is
                                 // performed
     I2CWrite(0x0040, 0x63, 0x29);       // Set ALS integration time to 100ms
     I2CWrite(0x002e, 0x01, 0x29);       // perform a single temperature calibration
                                 // of the ranging sensor

     // Optional: Public registers - See data sheet for more detail
     I2CWrite(0x001b, 0x09, 0x29);       // Set default ranging inter-measurement
                                 // period to 100ms
     I2CWrite(0x003e, 0x31, 0x29);       // Set default ALS inter-measurement period
                                 // to 500ms
     I2CWrite(0x0014, 0x24, 0x29);       // Configures interrupt on 'New Sample
 // Ready threshold event'

     //Clear fresh out of reset
     I2CWrite(0x016, 0x00, 0x29);

     uint8_t id = I2CRead(0x00, 0x29);
     uint8_t addr = I2CRead(0x212, 0x29);

     I2CWrite(0x212, 0x50, 0x29);
     id = I2CRead(0x212, 0x50);

     while(1){
     I2CWrite(0x18, 0x01, 0x50);
     uint8_t result = 0;
     while(!(result & 0x04)){
         result = I2CRead(0x04f, 0x50);
     }

     static uint8_t range = 0;
     range = I2CRead(0x62, 0x50);
     }
//    uint32_t data = 0, step = 0;
//    //Enable the i2c module and wait
//     SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);
//     SysCtlDelay(3);
//
//     //I2CSlaveEnable(I2C1_BASE);
//     //I2CSlaveInit(I2C1_BASE, 0x29);
//
//     //Enable I2C Loopback
//     //I2CLoopbackEnable(I2C1_BASE);
//
//     //Initialize the I2C Master module
//     I2CMasterInitExpClk(I2C1_BASE, SysClk, false);
//
//     //Shutdown all devices
//     GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0);
//     GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0);
//     GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, 0);
//
//     //Enable one device
//     GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, GPIO_PIN_3);
//
//     I2CMasterSlaveAddrSet(I2C1_BASE, 0x29, false);
//
//     I2CMasterDataPut(I2C1_BASE, 0x00);
//     I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_START);
//
//     while(I2CMasterBusy(I2C1_BASE));
//
//     I2CMasterDataPut(I2C1_BASE, 0x00);
//     I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
//
//     while(I2CMasterBusy(I2C1_BASE));
//
//     I2CMasterSlaveAddrSet(I2C1_BASE, 0x29, true);
//     I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
//
//     while(I2CMasterBusy(I2C1_BASE));
//
//     data = I2CMasterDataGet(I2C1_BASE);

//     while(I2CMasterBusy(I2C1_BASE)){
//
//     }
//     uint32_t data = 0, step = 0;
//
//     //Write 0x0000 to the device
//     I2CMasterSlaveAddrSet(I2C1_BASE, 0x29, false);
//
//     I2CMasterDataPut(I2C1_BASE, 0x000 >> 8);
//     I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_START);
//
//     I2CMasterDataPut(I2C1_BASE, 0x000);
//     I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
//
//     //Receive
//     I2CMasterSlaveAddrSet(I2C1_BASE, 0x29, true);
//     I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
//
//     while(!(I2CSlaveStatus(I2C1_BASE) & I2C_SLAVE_ACT_TREQ)){
//
//     }
//
//     I2CSlaveDataPut(I2C1_BASE, 0xB4);
//     I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
//     //while(!(I2CSlaveStatus(I2C1_BASE) & I2C_SLAVE_ACT_TREQ)){
//
//     //}
//     while(I2CMasterBusy(I2C1_BASE)){
//
//     }

     //SysCtlDelay(SysClk / 3);

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
