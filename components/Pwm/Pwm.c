#include "Pwm.h"

#define MAX_FREQ_HZ 40000000
#define DUTY_RES_BIT_MAX 14

static uint32_t Rate2Duty(PwmController this, float cycle)
{
    return (uint32_t)(this->dutyRes * cycle);
}

static uint32_t PulseWidth2DutyCycle(PwmController this, float pulseWidth)
{
    return (uint32_t)(pulseWidth / (1000 / this->freqHZ));
}

static ledc_timer_bit_t AutoDutyResBit(uint32_t freqHZ)
{
    uint32_t freq = MAX_FREQ_HZ / freqHZ;
    uint32_t bit = 1;
    while (freq > 1)
    {
        freq >>= 1;
        bit++;
    }
    bit = bit > DUTY_RES_BIT_MAX ? DUTY_RES_BIT_MAX : bit;
    return bit;
}

static ledc_timer_config_t timerConf0, timerConf1;

void PwmTimerInit(uint32_t freqHZLowSpeed, uint32_t freqHZHighSpeed)
{
    ledc_fade_func_install(0);

    timerConf0.speed_mode = LEDC_LOW_SPEED_MODE;
    timerConf0.timer_num = LEDC_TIMER_0;
    timerConf0.duty_resolution = AutoDutyResBit(freqHZLowSpeed);
    timerConf0.freq_hz = freqHZLowSpeed;
    timerConf0.clk_cfg = LEDC_AUTO_CLK;
    ledc_timer_config(&timerConf0);

    timerConf1.speed_mode = LEDC_HIGH_SPEED_MODE;
    timerConf1.timer_num = LEDC_TIMER_1;
    timerConf1.duty_resolution = AutoDutyResBit(freqHZHighSpeed);
    timerConf1.freq_hz = freqHZHighSpeed;
    timerConf1.clk_cfg = LEDC_AUTO_CLK;
    ledc_timer_config(&timerConf1);
}

PwmController PwmControllerCtor(ledc_mode_t speedMode,
                                ledc_channel_t pwmChannel,
                                gpio_num_t pinNum,
                                float dutyCycle)
{
    ledc_timer_config_t *timerConf =
        speedMode == LEDC_LOW_SPEED_MODE
            ? &timerConf0
            : &timerConf1;

    PwmController this = (PwmController)malloc(sizeof(struct PwmController));
    this->speedMode = speedMode;
    this->pwmChannel = pwmChannel;
    this->pinNum = pinNum;
    this->freqHZ = timerConf->freq_hz;
    this->dutyRes = 1 << timerConf->duty_resolution;
    this->dutyCycle = dutyCycle;

    ledc_channel_config_t pwmCfg = {
        .speed_mode = this->speedMode,
        .channel = this->pwmChannel,
        .timer_sel = timerConf->timer_num,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = this->pinNum,
        .duty = Rate2Duty(this, this->dutyCycle),
        .hpoint = 0};
    ledc_channel_config(&pwmCfg);

    return this;
}

void PwmControllerWriteDutyCycle(PwmController this, float cycle)
{
    ledc_set_duty_and_update(this->speedMode,
                             this->pwmChannel,
                             Rate2Duty(this, cycle),
                             0);
    this->dutyCycle = cycle;
}

void PwmControllerWriteDutyCycleTime(PwmController this, float cycle, uint32_t time)
{
    ledc_set_fade_time_and_start(this->speedMode,
                                 this->pwmChannel,
                                 Rate2Duty(this, cycle),
                                 time,
                                 LEDC_FADE_NO_WAIT);
    this->dutyCycle = cycle;
}

void PwmControllerWritePulseWidth(PwmController this, float width)
{
    PwmControllerWriteDutyCycle(this, PulseWidth2DutyCycle(this, width));
}

void PwmControllerWritePulseWidthTime(PwmController this, float width, uint32_t time)
{
    PwmControllerWriteDutyCycleTime(this, PulseWidth2DutyCycle(this, width), time);
}