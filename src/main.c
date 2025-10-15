/******************************************************************************
 * @file           : main.c
 * @brief          : Main application entry point for  esp32 usb2can
 * @author         : İ.H. Metehan Erol
 * @date           : 2023-8-5
 ******************************************************************************
 * @details
 * This file is part of the ESP32 seril2can repository.
 * 
 * Description:
 *  device firmware for esp32seril2can device -- able work with alla esp devboards
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






#include "driver/uart.h"
#include "slcan.h"
#include "serial.h"
#include "freertos/portmacro.h"
#include "string.h"
#include <stdbool.h>
#include <stdint.h>

QueueHandle_t slTxQueue;

run_mode_t runningMode;

void serial_rx_task(void *arg) {

    slcan_can_cfg_t* can_cfg = (slcan_can_cfg_t *)arg;
    uint8_t rBuffer[UART_RX_BUF_SIZE];

    while (true) {
        int r_len = uart_read_bytes(UART_NUM, (void *)rBuffer, UART_RX_BUF_SIZE,
                                    1 / portTICK_PERIOD_MS);
        if (r_len) {
            process_rx_data(rBuffer, can_cfg);
        }
    }
}

void serial_tx_task(void* agr) {
    
    while (true) {
        uint8_t* tx_buf = (uint8_t *) malloc(60);

        xQueueReceive(slTxQueue, (void *)tx_buf, portMAX_DELAY);

        size_t len = tx_buf[0];

        send_serial((uint8_t *)tx_buf[1], len);
        free(tx_buf);
    }
}

void app_main() {

    serial_init();

    slcan_can_cfg_t can_conf = {};

    xTaskCreatePinnedToCore(serial_rx_task, "Rx task", 4096, &can_conf, 2, NULL, 1);
    xTaskCreatePinnedToCore(serial_tx_task, "TX task", 4096, &can_conf, 2, NULL, 1);

}
