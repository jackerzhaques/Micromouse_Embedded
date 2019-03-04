/*
 * File: MotorDriver.h
 * Author: Todd Morehouse
 * Date: February 23rd, 2019
 * Description:
 *  This driver initializes PWM for both motors, and provides
 *  functions for setting the duty cycle and direction for each motor
 *
 *  TODO: Revisit this driver and clean it up to be cleaner and more modular
 */

#ifndef MM_MOTOR_DRIVER_H
#define MM_MOTOR_DRIVER_H

#include <stdint.h>
#include <stdbool.h>
#include <inc/hw_memmap.h>

#define TIMER_MAX_VALUE     16000000
#define LEFT_ENCODER_BIT    0x08
#define RIGHT_ENCODER_BIT   0x04

#define PWM_BASE            PWM0_BASE
#define PWM_GEN             PWM_GEN_0
#define LEFT_MOTOR_PWM      PWM_OUT_0
#define RIGHT_MOTOR_PWM     PWM_OUT_1

/*
 * These defines dictate which GPIO pins the H-Bridge inputs are on
 *
 * AIN refers to the left motor
 * BIN refers to the right motor
 */
#define AIN1_PIN            GPIO_PIN_2
#define AIN1_BASE           GPIO_PORTE_BASE

#define AIN2_PIN            GPIO_PIN_3
#define AIN2_BASE           GPIO_PORTE_BASE

#define BIN1_PIN            GPIO_PIN_4
#define BIN1_BASE           GPIO_PORTF_BASE

#define BIN2_PIN            GPIO_PIN_1
#define BIN2_BASE           GPIO_PORTE_BASE

typedef enum{
    FWD,
    REV,
    STOPPED,
    BRAKE
} MotorDir;

void MD_Initialize(void);

void MD_SetLeftMotorDutyCycle(float DutyCycle);
void MD_SetRightMotorDutyCycle(float DutyCycle);

void MD_SetLeftMotorDir(MotorDir Direction);
void MD_SetRightMotorDir(MotorDir Direction);
MotorDir MD_GetLeftMotorDir(void);
MotorDir MD_GetRightMotorDir(void);

void MD_StopLeftMotor(void);
void MD_StopRightMotor(void);

int32_t MD_GetLeftEncoderTicks(void);
float MD_GetLeftEncoderSpeed(void);
void MD_ResetLeftEncoderTicks(void);

int32_t MD_GetRightEncoderTicks(void);
float MD_GetRightEncoderSpeed(void);
void MD_ResetRightEncoderTicks(void);

#endif
