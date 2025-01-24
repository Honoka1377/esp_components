#include "Wifi.h"

static void WifiEventHandler(void *arg,
                             esp_event_base_t eventBase,
                             int32_t eventID,
                             void *eventData)
{
    esp_wifi_connect();
}

void WifiConnect(char *ssid, char *password)
{
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    esp_event_handler_instance_register(WIFI_EVENT,
                                        WIFI_EVENT_STA_START,
                                        &WifiEventHandler,
                                        NULL,
                                        NULL);
    esp_event_handler_instance_register(WIFI_EVENT,
                                        WIFI_EVENT_STA_DISCONNECTED,
                                        &WifiEventHandler,
                                        NULL,
                                        NULL);

    esp_wifi_set_mode(WIFI_MODE_STA);

    wifi_config_t wifiCfg = {
        .sta = {
            .ssid = "",
            .password = ""}};
    strcpy((char *)wifiCfg.sta.ssid, ssid);
    strcpy((char *)wifiCfg.sta.password, password);

    esp_wifi_set_config(WIFI_IF_STA, &wifiCfg);
    esp_wifi_start();
}