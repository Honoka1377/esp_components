idf_component_register(
    SRCS "Wifi.c"
    INCLUDE_DIRS "."
    REQUIRES "esp_wifi" "esp_event" "lwip"
)
