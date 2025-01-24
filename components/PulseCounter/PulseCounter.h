#ifndef PULSE_COUNTER_H
#define PULSE_COUNTER_H

#include "driver/gpio.h"
#include "driver/pulse_cnt.h"

typedef struct PulseCounter
{
    pcnt_unit_handle_t pcntUnit;
    pcnt_channel_handle_t pcntChannel;
} *PulseCounter;

int PulseCounterReadCount(PulseCounter this);
PulseCounter PulseCounterCtor(gpio_num_t edgePinNum, gpio_num_t levelPinNum);
#endif