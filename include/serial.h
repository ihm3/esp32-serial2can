/**
 ******************************************************************************
 * @file           : serial.c
 * @brief          : serial com. part of  esp32 usb2can
 * @author         : İ.H. Metehan Erol
 * @date           : 2023-8-5
 ******************************************************************************
 * @details
 * This file is part of the ESP32 seril2can repository.
 * 
 * Description:
 *  hardware level serial interface for ÊSP32 to handle serial com. side for serial2can
 *   
 * 
 *
 *
 * Dependencies:
 *   - ESP IDF
 *   - GCC / Make
 *   - Platformio
 *
 * License:
 *   This software is distributed under the MIT License.
 *
 ******************************************************************************
 */

#ifndef __UART_HANDLE
#define __UART_HANDLE

 


//  ----   config ----

#define BLE_FETURE 0 // enabe ble







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