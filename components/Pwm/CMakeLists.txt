idf_component_register(
    SRCS "Servo.c" "Pwm.c"
    INCLUDE_DIRS "."
    REQUIRES "esp_driver_ledc" "esp_driver_gpio"
)
