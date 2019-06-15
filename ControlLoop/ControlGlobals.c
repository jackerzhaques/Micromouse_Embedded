#include "ControlGlobals.h"
#include "Interfaces/EncoderInterface.h"

volatile static PID PIDs[N_CONTROL_LOOPS] = {
        //Left wheel position
        {
         .Kp                = 40,
         .Ki                = 0,
         .Kd                = 0,
         .dcOffset          = 0,
         .iState            = 0,
         .iMin              =-2,
         .iMax              = 2,
         .dState            = 0,
         .target            = 0,
         .targetMin         =-100000,
         .targetMax         = 100000,
         .error             = 0,
         .threshold         = 0,
         .output            = 0,
         .outputMin         =-1000,
         .outputMax         = 1000,
        },
        //Right wheel position
        {
         .Kp                = 40,
         .Ki                = 0,
         .Kd                = 0,
         .dcOffset          = 0,
         .iState            = 0,
         .iMin              =-2,
         .iMax              = 2,
         .dState            = 0,
         .target            = 0,
         .targetMin         =-100000,
         .targetMax         = 100000,
         .error             = 0,
         .threshold         = 0,
         .output            = 0,
         .outputMin         =-1000,
         .outputMax         = 1000,
        },
        //Left wheel speed
        {
         .Kp                = 0.001,
         .Ki                = 0.0000001,
         .Kd                = 0,
         .dcOffset          = 0,
         .iState            = 0,
         .iMin              =-1,
         .iMax              = 1,
         .dState            = 0,
         .target            = 0,
         .targetMin         =-1000,
         .targetMax         = 1000,
         .error             = 0,
         .threshold         = 0,
         .output            = 0,
         .outputMin         =-1,
         .outputMax         = 1,
        },
        //Right wheel speed
        {
         .Kp                = 0.001,
         .Ki                = 0.0000001,
         .Kd                = 0,
         .dcOffset          = 0,
         .iState            = 0,
         .iMin              =-1,
         .iMax              = 1,
         .dState            = 0,
         .target            = 0,
         .targetMin         =-1000,
         .targetMax         = 1000,
         .error             = 0,
         .threshold         = 0,
         .output            = 0,
         .outputMin         =-1,
         .outputMax         = 1,
        },
};


volatile PID* GetControlPIDs(void){
    return PIDs;
}
