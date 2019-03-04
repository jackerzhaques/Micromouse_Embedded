#ifndef MM_PID_H
#define MM_PID_H

typedef struct sPID_tag{
    float Kp;
    float Ki;
    float Kd;

    float Target;

    float iState;
    float iMin;
    float iMax;

    float LastError;

    float Output;
    float OutputMin;
    float OutputMax;
} sPID;

float UpdatePID(sPID* PID, float Measurement);

#endif
