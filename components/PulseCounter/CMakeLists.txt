idf_component_register(
    SRCS "PulseCounter.c"
    INCLUDE_DIRS "."
    REQUIRES "driver" "esp_driver_gpio"
)
