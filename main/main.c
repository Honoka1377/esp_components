#include <stdio.h>
#include "esp_log.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"

#include "Pwm.h"

void app_main(void)
{
    PwmTimerInit(20000, 20000);
    PwmController pwm = PwmControllerCtor(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, GPIO_NUM_25, 0.5);
    PwmControllerWriteDutyCycle(pwm, 0.3);
    vTaskDelay(portMAX_DELAY);
}