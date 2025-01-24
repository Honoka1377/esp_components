#include "DataExchanger.h"
#include "esp_log.h"

#define TASK_STACK_DEPTH 81920
#define TASK_PRIORITY 15
#define WAIT_AFTER_FAILURE 1000
#define WAIT_AFTER_EXCHANGED 20

ESP_EVENT_DEFINE_BASE(DataExchangerEvent);
static const char *CLIENT_TAG = "DataExchangeClient";
static const char *LISTENER_TAG = "DataExchangeListener";

static const int reuseAddrOpt = 1;
static const int keepAliveOpt = 1;
static const int keepIdleOpt = 5;
static const int keepIntervalOpt = 5;
static const int keepCountOpt = 3;

#pragma region Event
static void OnDataReceived(DataReceivedEventData *arg)
{
    esp_event_post(DataExchangerEvent,
                   DataReceived,
                   arg,
                   sizeof(DataReceivedEventData),
                   portMAX_DELAY);
}
static void OnConnectionStart()
{
    esp_event_post(DataExchangerEvent,
                   ConnectionStart,
                   NULL,
                   0,
                   portMAX_DELAY);
}
static void OnConnectionEstablished()
{
    esp_event_post(DataExchangerEvent,
                   ConnectionEstablished,
                   NULL,
                   0,
                   portMAX_DELAY);
}
static void OnConnectionFailed()
{
    esp_event_post(DataExchangerEvent,
                   ConnectionFailed,
                   NULL,
                   0,
                   portMAX_DELAY);
}
#pragma endregion

#pragma region Client
static void ClientAction(void *arg)
{
    DataExchangeClient this = (DataExchangeClient)arg;
    struct sockaddr_in ipEndPoint = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = inet_addr(this->ipAddress),
        .sin_port = htons(this->port),
    };

    for (;;)
    {
        int client = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
        if (client < 0)
        {
            ESP_LOGE(CLIENT_TAG, "Failed to creat socket: %s", strerror(errno));
            OnConnectionFailed();
            vTaskDelay(WAIT_AFTER_FAILURE / portTICK_PERIOD_MS);
            continue; // Try to connect to the Listener again
        }
        ESP_LOGI(CLIENT_TAG, "Try to connect to the listener");
        OnConnectionStart();

        int err = connect(client, (struct sockaddr *)&ipEndPoint, sizeof(ipEndPoint));
        if (err != 0)
        {
            ESP_LOGE(CLIENT_TAG, "Failed to connect to the listener: %s", strerror(errno));
            close(client);
            OnConnectionFailed();
            vTaskDelay(WAIT_AFTER_FAILURE / portTICK_PERIOD_MS);
            continue; // Try to connect to the Listener again
        }
        OnConnectionEstablished();
        ESP_LOGI(CLIENT_TAG, "Connection established");
        for (;;) // Start exchanging data
        {
            int err = send(client, this->writeData, this->writeLength, 0);
            if (err < 0)
                break;
            int len = recv(client, this->readData, this->readLength, 0);
            if (len < 0)
                break;
            DataReceivedEventData arg = {
                .data = this->readData,
                .dataLength = this->readLength};
            OnDataReceived(&arg);
            vTaskDelay(WAIT_AFTER_EXCHANGED / portTICK_PERIOD_MS);
        }
        ESP_LOGE(CLIENT_TAG, "Connection lost: %s", strerror(errno));
        close(client);
        OnConnectionFailed();
        // Reconnect without waiting
    }
}

DataExchangeClient DataExchangeClientCtor(char *ipAddress, uint16_t port, size_t readLength, size_t writeLength)
{
    DataExchangeClient this = (DataExchangeClient)malloc(sizeof(struct DataExchanger));
    this->ipAddress = ipAddress;
    this->port = port;
    this->readData = (uint8_t *)calloc(readLength, sizeof(uint8_t));
    this->writeData = (uint8_t *)calloc(writeLength, sizeof(uint8_t));
    this->readLength = readLength;
    this->writeLength = writeLength;

    xTaskCreate(ClientAction, "DataExchangeClient", TASK_STACK_DEPTH, this, TASK_PRIORITY, NULL);
    return this;
}
#pragma endregion

#pragma region Listener
static void ListenerAction(void *arg)
{
    DataExchangeListener this = (DataExchangeListener)arg;
    struct sockaddr_in ipEndPoint = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = inet_addr(this->ipAddress),
        .sin_port = htons(this->port),
    };

    for (;;)
    {
        int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
        if (listener < 0)
        {
            ESP_LOGE(LISTENER_TAG, "Failed to creat socket: %s", strerror(errno));
            OnConnectionFailed();
            vTaskDelay(WAIT_AFTER_FAILURE / portTICK_PERIOD_MS);
            continue;
        }
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &reuseAddrOpt, sizeof(int));

        ESP_LOGI(LISTENER_TAG, "Waiting for connections");
        OnConnectionStart();

        int err = 0;
        err = bind(listener, (struct sockaddr *)&ipEndPoint, sizeof(ipEndPoint));
        if (err != 0)
        {
            ESP_LOGE(LISTENER_TAG, "Failed to bind socket: %s", strerror(errno));
            close(listener);
            OnConnectionFailed();
            vTaskDelay(WAIT_AFTER_FAILURE / portTICK_PERIOD_MS);
            continue;
        }

        err = listen(listener, 1);
        if (err != 0)
        {
            ESP_LOGE(LISTENER_TAG, "Failed to start listening: %s", strerror(errno));
            close(listener);
            OnConnectionFailed();
            vTaskDelay(WAIT_AFTER_FAILURE / portTICK_PERIOD_MS);
            continue;
        }

        struct sockaddr addr;
        socklen_t addrLength = sizeof(struct sockaddr);
        int client = accept(listener, &addr, &addrLength);
        if (client < 0)
        {
            ESP_LOGE(LISTENER_TAG, "Failed to accept client: %s", strerror(errno));
            close(listener);
            OnConnectionFailed();
            vTaskDelay(WAIT_AFTER_FAILURE / portTICK_PERIOD_MS);
            continue;
        }
        setsockopt(client, SOL_SOCKET, SO_KEEPALIVE, &keepAliveOpt, sizeof(int));
        setsockopt(client, IPPROTO_TCP, TCP_KEEPIDLE, &keepIdleOpt, sizeof(int));
        setsockopt(client, IPPROTO_TCP, TCP_KEEPINTVL, &keepIntervalOpt, sizeof(int));
        setsockopt(client, IPPROTO_TCP, TCP_KEEPCNT, &keepCountOpt, sizeof(int));

        OnConnectionEstablished();
        ESP_LOGI(LISTENER_TAG, "Connection established");

        for (;;) // Start exchanging data
        {
            int len = recv(client, this->readData, this->readLength, 0);
            if (len < 0)
                break;
            DataReceivedEventData arg = {
                .data = this->readData,
                .dataLength = this->readLength};
            OnDataReceived(&arg);
            int err = send(client, this->writeData, this->writeLength, 0);
            if (err < 0)
                break;
        }
        ESP_LOGE(LISTENER_TAG, "Connection lost: %s", strerror(errno));
        close(client);
        close(listener);
        OnConnectionFailed();
    }
}

DataExchangeListener DataExchangeListenerCtor(char *ipAddress, uint16_t port, size_t readLength, size_t writeLength)
{
    DataExchangeListener this = (DataExchangeListener)malloc(sizeof(struct DataExchanger));
    this->ipAddress = ipAddress;
    this->port = port;
    this->readData = (uint8_t *)calloc(readLength, sizeof(uint8_t));
    this->writeData = (uint8_t *)calloc(writeLength, sizeof(uint8_t));
    this->readLength = readLength;
    this->writeLength = writeLength;

    xTaskCreate(ListenerAction, "DataExchangeListener", TASK_STACK_DEPTH, this, TASK_PRIORITY, NULL);
    return this;
}
#pragma endregion