#ifndef CONTROL_LOOP_CONTROL_LOOP_H
#define CONTROL_LOOP_CONTROL_LOOP_H

#define CONTROL_LOOP_FREQUENCY      100  //Hz

#include <stdint.h>

void InitializeControlLoop(uint32_t sysClk);

void SetLeftWheelSpeed(float Speed);
void SetRightWheelSpeed(float Speed);
void SetLeftWheelTarget(float Target);
void SetRightWheelTarget(float Target);

#endif
