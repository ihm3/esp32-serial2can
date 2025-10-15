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
 *  hardware level serial interface for ÊSP32 to handle serial com. side for
 * serial2can
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


#include "serial.h"
#include "freertos/timers.h"

static void uart_init() {

    const uart_config_t uart_config = {
        .baud_rate = UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE, /** metehan er1 */
        .source_clk = UART_SCLK_DEFAULT,
    };

    if (uart_is_driver_installed(UART_NUM)) {
        uart_driver_delete(UART_NUM);
    }

    // We won't use a buffer for sending data.
    ESP_ERROR_CHECK(
        uart_driver_install(UART_NUM, UART_RX_BUF_SIZE * 2, 0, 0, NULL, 0));
    ESP_ERROR_CHECK(uart_param_config(UART_NUM, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM, UART_TXD_PIN, UART_RXD_PIN,
                                 UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
}

#if BLE_FEATURE
static void ble_init() {

    esp_bt_controller_config_t bleConfig = BT_CONTROLLER_INIT_CONFIG_DEFAULT();

    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));
    ESP_ERROR_CHECK(esp_bt_controller_init(&bleConfig));
}
#endif

void serial_init() {

    uart_init();

    vTaskDelay(2 / portTICK_PERIOD_MS);
#if BLE_FEATURE
    ble_init();
#endif
}

void send_serial(uint8_t *data, size_t len) {

#if BLE_FEATURE
    switch (runningMode) {
    case UART_MODE:
#endif

        uart_write_bytes(UART_NUM, (void *)data, len);

#if BLE_FEATURE
        break;
    case BLE_MODE:
        break;
    case NO_MODE:
        break;
    default:
        break;
    }
#endif
}