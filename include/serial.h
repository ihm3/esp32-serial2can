#ifndef __UART_HANDLE
#define __UART_HANDLE

#include "driver/uart.h"
#include "nvs.h"
#include "nvs_flash.h"

#include "esp_bt.h"
#include "esp_gap_ble_api.h"
#include "esp_gattc_api.h"
#include "esp_gatt_defs.h"
#include "esp_bt_main.h"
#include "esp_gatt_common_api.h"
#include "esp_log.h"

extern QueueHandle_t slTxQueue;

typedef enum {
    NO_MODE,
    UART_MODE,
    BLE_MODE,
} run_mode_t;

extern run_mode_t runningMode;

#define UART_NUM     UART_NUM_1
#define UART_TXD_PIN 1
#define UART_RXD_PIN 3

#define UART_RX_BUF_SIZE 1024
#define UART_BAUD_RATE  115200

void serial_init();
void send_serial(uint8_t* data, size_t len);

#endif