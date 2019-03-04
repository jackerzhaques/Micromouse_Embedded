#include "PID.h"

float CalculateError(float Target, float Measurement){
    return Target - Measurement;
}

float Proportional(sPID *PID, float Error){
    return PID->Kp * Error;
}

float Integral(sPID *PID, float Error){
    PID->iState += Error;
    if(PID->iState > PID->iMax){
        PID->iState = PID->iMax;
    }
    else if(PID->iState < PID->iMin){
        PID->iState = PID->iMin;
    }

    return PID->Ki * PID->iState;
}

float Derivative(sPID *PID, float Error){
    float Delta = Error - PID->LastError;
    PID->LastError = Error;

    return Delta*PID->Kd;
}

float UpdatePID(sPID *PID, float Measurement){
    float Error = CalculateError(PID->Target, Measurement);

    float P = Proportional(PID, Error);
    float I = Integral(PID, Error);
    float D = Derivative(PID, Error);

    float Output = P + I + D;

    if(Output > PID->OutputMax){
        Output = PID->OutputMax;
    }
    else if(Output < PID->OutputMin){
        Output = PID->OutputMin;
    }

    PID->Output = Output;

    return Output;
}
