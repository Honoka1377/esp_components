#ifndef SERVO_H
#define SERVO_H

#include "stdio.h"
#include "string.h"
#include "driver/gpio.h"
#include "driver/ledc.h"

typedef struct Servo
{
    ledc_mode_t speedMode;
    ledc_channel_t pwmChannel;
    gpio_num_t pinNum;
    float minAngle;
    float maxAngle;
    float angle;
} *Servo;

void ServoLedcInit(void);

Servo ServoCtor(ledc_mode_t speedMode,
                ledc_channel_t pwmChannel,
                gpio_num_t pinNum,
                float minAngle,
                float maxAngle,
                float angle);

void ServoWriteAngle(Servo this, float angle);
void ServoWriteAngleTime(Servo this, float angle, uint32_t time);

#endif
