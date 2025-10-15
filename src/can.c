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

#include "can.h"

static void set_bitrate(slcan_bit_rete_t rate, twai_timing_config_t* configuretiming){

    switch (rate) {

        case SET_10K:       // not supported
            *configuretiming =  (twai_timing_config_t){};
            break;

        case SET_20K:       // not supported
            *configuretiming =  (twai_timing_config_t){};
            break;

        case SET_50K:       // not supported
            *configuretiming  = (twai_timing_config_t)TWAI_TIMING_CONFIG_50KBITS();
            break;

        case SET_100k:
            *configuretiming  = (twai_timing_config_t)TWAI_TIMING_CONFIG_100KBITS();
            break;

        case SET_125k:
            *configuretiming  = (twai_timing_config_t)TWAI_TIMING_CONFIG_125KBITS();
            break;

        case SET_250k:
            *configuretiming = (twai_timing_config_t)TWAI_TIMING_CONFIG_250KBITS();
            break;

        case SET_500k:
            *configuretiming  = (twai_timing_config_t)TWAI_TIMING_CONFIG_500KBITS();
            break;

        case SET_750k:  // not supported
            *configuretiming =  (twai_timing_config_t){};
            break;

        case SET_1M:
            *configuretiming  = (twai_timing_config_t)TWAI_TIMING_CONFIG_1MBITS();
            break;
    }
}

esp_err_t init_canbus(slcan_can_cfg_t* cfg) {
    esp_err_t ret = ESP_OK;

    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(GPIO_NUM_21, GPIO_NUM_22, TWAI_MODE_NORMAL);
    twai_timing_config_t t_config = {};
    twai_filter_config_t f_config = {.single_filter = true};

    set_bitrate(cfg->bitrate, &t_config);

    f_config.acceptance_code = (uint32_t) cfg->acceptance_code;
    f_config.acceptance_mask = (uint32_t) cfg->acceptance_mask;

    if(0 == t_config.brp)return ESP_FAIL;

    ret = twai_driver_install(&g_config, &t_config, &f_config);
    ret = twai_start();

    return ret;
}

esp_err_t transmit_from_sl(uint32_t id, uint8_t dlc, bool extd, bool rtr, uint8_t* data){
        twai_message_t canMessage = {};

        canMessage.identifier = id;
        canMessage.extd = extd;
        canMessage.rtr = rtr;
        canMessage.data_length_code = dlc;

        if(!rtr){
            for (size_t i = 0; i < dlc; i++){
                int bytedata = 0;
                sscanf((const char*)&data[i*2], "%02x", &bytedata);
                canMessage.data[i] = (uint8_t)bytedata;
            }
        }
       
        return twai_transmit(&canMessage, pdMS_TO_TICKS(100));
}