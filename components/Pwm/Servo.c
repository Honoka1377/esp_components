#include "Servo.h"

#define FREQ_HZ 50
#define MIN_PLUSE_WIDTH 0.5
#define MAX_PLUSE_WIDTH 2.5

uint32_t PulseWidth2Duty(float pulseWidth)
{
    return 8192 * pulseWidth / (1000 / FREQ_HZ);
}

uint32_t Angle2Duty(const Servo this, float angle)
{
    float k = (angle - this->minAngle) / (this->maxAngle - this->minAngle);
    float pulseWidth = (MAX_PLUSE_WIDTH - MIN_PLUSE_WIDTH) * k + MIN_PLUSE_WIDTH;
    return PulseWidth2Duty(pulseWidth);
}

void ServoLedcInit(void)
{
    ledc_fade_func_install(0);

    ledc_timer_config_t timerConf1 = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_num = LEDC_TIMER_0,
        .duty_resolution = LEDC_TIMER_13_BIT,
        .freq_hz = 50,
        .clk_cfg = LEDC_AUTO_CLK};

    ledc_timer_config_t timerConf2 = {
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .timer_num = LEDC_TIMER_1,
        .duty_resolution = LEDC_TIMER_13_BIT,
        .freq_hz = 50,
        .clk_cfg = LEDC_AUTO_CLK};

    ledc_timer_config(&timerConf1);
    ledc_timer_config(&timerConf2);
}

Servo ServoCtor(ledc_mode_t speedMode,
                ledc_channel_t pwmChannel,
                gpio_num_t pinNum,
                float minAngle,
                float maxAngle,
                float angle)
{
    Servo this = (Servo)malloc(sizeof(struct Servo));
    this->speedMode = speedMode;
    this->pwmChannel = pwmChannel;
    this->pinNum = pinNum;
    this->minAngle = minAngle;
    this->maxAngle = maxAngle;
    this->angle = angle;

    ledc_channel_config_t pwmCfg = {
        .speed_mode = this->speedMode,
        .channel = this->pwmChannel,
        .timer_sel = this->speedMode == LEDC_LOW_SPEED_MODE ? LEDC_TIMER_0 : LEDC_TIMER_1,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = this->pinNum,
        .duty = Angle2Duty(this, this->angle),
        .hpoint = 0};
    ledc_channel_config(&pwmCfg);

    return this;
}

void ServoWriteAngle(Servo this, float angle)
{
    ledc_set_duty_and_update(this->speedMode,
                             this->pwmChannel,
                             Angle2Duty(this, angle),
                             0);
    this->angle = angle;
}

void ServoWriteAngleTime(Servo this, float angle, uint32_t time)
{
    ledc_set_fade_time_and_start(this->speedMode,
                                 this->pwmChannel,
                                 Angle2Duty(this, angle),
                                 time,
                                 LEDC_FADE_NO_WAIT);
    this->angle = angle;
}
