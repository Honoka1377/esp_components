#include "PulseCounter.h"

PulseCounter PulseCounterCtor(gpio_num_t edgePinNum, gpio_num_t levelPinNum)
{
    PulseCounter counter = (PulseCounter)malloc(sizeof(struct PulseCounter));

    pcnt_unit_config_t pcntUnitConfig = {
        .high_limit = INT16_MAX,
        .low_limit = INT16_MIN,
    };
    pcnt_new_unit(&pcntUnitConfig, &(counter->pcntUnit));

    pcnt_chan_config_t pcntChannelConfig = {
        .edge_gpio_num = edgePinNum,
        .level_gpio_num = levelPinNum,
    };
    pcnt_new_channel(counter->pcntUnit, &pcntChannelConfig, &(counter->pcntChannel));

    pcnt_channel_set_edge_action(
        counter->pcntChannel,
        PCNT_CHANNEL_EDGE_ACTION_DECREASE,
        PCNT_CHANNEL_EDGE_ACTION_INCREASE);

    pcnt_channel_set_level_action(
        counter->pcntChannel,
        PCNT_CHANNEL_LEVEL_ACTION_KEEP,
        PCNT_CHANNEL_LEVEL_ACTION_INVERSE);

    pcnt_glitch_filter_config_t filterConfig = {
        .max_glitch_ns = 1000,
    };
    pcnt_unit_set_glitch_filter(counter->pcntUnit, &filterConfig);

    pcnt_unit_enable(counter->pcntUnit);
    pcnt_unit_start(counter->pcntUnit);

    return counter;
}

int PulseCounterReadCount(PulseCounter this)
{
    int ret = 0;
    pcnt_unit_get_count(this->pcntUnit, &ret);
    return ret;
}
