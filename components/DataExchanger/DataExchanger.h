#ifndef DATA_EXCHANGER_H
#define DATA_EXCHANGER_H

#include "lwip/sockets.h"
#include "errno.h"
#include "esp_event.h"
#include "esp_netif.h"

ESP_EVENT_DECLARE_BASE(DataExchangerEvent);

typedef struct DataReceivedEventData
{
    uint8_t *data;
    size_t dataLength;
} DataReceivedEventData;

typedef enum DataExchangerEventID
{
    DataReceived = 0,
    ConnectionStart,
    ConnectionEstablished,
    ConnectionFailed
} DataExchangerEventID;

struct DataExchanger
{
    uint8_t *readData;
    uint8_t *writeData;
    size_t readLength;
    size_t writeLength;
    char *ipAddress;
    uint16_t port;
};
typedef struct DataExchanger *DataExchangeListener;
typedef struct DataExchanger *DataExchangeClient;

DataExchangeListener DataExchangeListenerCtor(char *ipAddress, uint16_t port, size_t readLength, size_t writeLength);
DataExchangeClient DataExchangeClientCtor(char *ipAddress, uint16_t port, size_t readLength, size_t writeLength);
#endif