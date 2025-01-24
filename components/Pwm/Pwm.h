#ifndef PWM_H
#define PWM_H

#include "stdio.h"
#include "driver/gpio.h"
#include "driver/ledc.h"

typedef struct PwmController
{
    ledc_mode_t speedMode;
    ledc_channel_t pwmChannel;
    gpio_num_t pinNum;
    uint32_t freqHZ;
    uint32_t dutyRes;
    float dutyCycle;
} *PwmController;

void PwmTimerInit(uint32_t freqHZLowSpeed, uint32_t freqHZHighSpeed);

PwmController PwmControllerCtor(ledc_mode_t speedMode,
            ledc_channel_t pwmChannel,
            gpio_num_t pinNum,
            float dutyCycle);

void PwmControllerWriteDutyCycle(PwmController this, float rate);
void PwmControllerWriteDutyCycleTime(PwmController this, float rate, uint32_t time);
void PwmControllerWritePulseWidth(PwmController this, float width);
void PwmControllerWritePulseWidthTime(PwmController this, float width, uint32_t time);
#endif
