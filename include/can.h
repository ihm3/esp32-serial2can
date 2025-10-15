/******************************************************************************
 * @file           : main.c
 * @brief          : can interface for esp32 usb2can
 * @author         : İ.H. Metehan Erol
 * @date           : 2023-8-5
 ******************************************************************************
 * @details
 * This file is part of the ESP32 seril2can repository.
 * 
 * Description:
 *  can bus handler aim to work with slcan protochol
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

#ifndef __CAN_HANDLE__
#define __CAN_HANDLE__

#include "driver/twai.h"
#include "esp_err.h"
#include "freertos/portmacro.h"
#include "slcan.h"

esp_err_t init_canbus(slcan_can_cfg_t *cfg);
esp_err_t transmit_from_sl(uint32_t id, uint8_t dlc, bool extd, bool rtr, uint8_t* data);

#endif