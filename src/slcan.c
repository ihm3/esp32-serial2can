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
 *  slcan p. handler
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

 

#include "slcan.h"
#include "serial.h"
#include "can.h"

uint32_t convert_canid(char* buf, bool extd){

    unsigned int high_pars, low_pars = 0;

    if(extd)sscanf(&buf[1], "%04x%04x", &high_pars, &low_pars);     //Tiiiiiiiildd...
    else sscanf(&buf[1], "%03x", &low_pars);                         //tiiildd...

    return (uint32_t)(((uint32_t) high_pars << 16) | (uint32_t)low_pars);
}

void process_rx_data(uint8_t *data, slcan_can_cfg_t* can_cfg) {

    slcan_protochol_t *command = (slcan_protochol_t *)data[0];

    uint8_t *urt_ret = (uint8_t *) malloc(60);

    switch (*command) {

    case OPEN_CAN:
        urt_ret[0] = 1;             /** lenght of data */
        if(init_canbus(can_cfg) == ESP_OK)urt_ret[1] = ACK;
        else urt_ret[1] = NACK;
        xQueueSend(slTxQueue, (void *)urt_ret, portMAX_DELAY);
        break;

    case CLOSE_CAN:
        urt_ret[0] = 1;             /** lenght of data */
        esp_err_t unins =  twai_stop();
        if(unins == ESP_OK)unins = twai_driver_uninstall();
        if(unins == ESP_OK)urt_ret[1] = ACK;
        else urt_ret[1] = NACK;
        xQueueSend(slTxQueue, (void *)urt_ret, portMAX_DELAY);
        break;

    case TRANSMIT_STD:
        urt_ret[0] = 1;             /** lenght of data */
        if(ESP_OK == transmit_from_sl(convert_canid((char *)&data[1], false), (uint8_t)atoi((const char *)&data[4]), false, false, &data[5]))
                urt_ret[1] = ACK;
        else urt_ret[1] = NACK;
        xQueueSend(slTxQueue, (void *)urt_ret, portMAX_DELAY);
        break;

    case TRANSMIT_EXTD:
        urt_ret[0] = 1;             /** lenght of data */
        if(ESP_OK == transmit_from_sl(convert_canid((char *)&data[1], true), (uint8_t)atoi((char *)&data[9]), true, false, &data[10]))
                urt_ret[1] = ACK;
        else urt_ret[1] = NACK;
        xQueueSend(slTxQueue, (void *)urt_ret, portMAX_DELAY);
        break;

    case TRANSMIT_STD_RTR:  
        urt_ret[0] = 1;             /** lenght of data */
         if(ESP_OK == transmit_from_sl(convert_canid((char *)&data[1], false), (uint8_t)atoi((char *)&data[4]), false, true, &data[5]))
                urt_ret[1] = ACK;
        else urt_ret[1] = NACK;
        xQueueSend(slTxQueue, (void *)urt_ret, portMAX_DELAY);
        break;

    case TRANSMIT_EXTD_RTR:
        urt_ret[0] = 1;             /** lenght of data */
        if(ESP_OK == transmit_from_sl(convert_canid((char *)&data[1], true), (uint8_t)atoi((char *)&data[9]), true, false, &data[10]))
                urt_ret[1] = ACK;
        else urt_ret[1] = NACK;
        xQueueSend(slTxQueue, (void *)urt_ret, portMAX_DELAY);
        break;

    case SET_CAN:
        can_cfg->bitrate = data[1];
        break;

    case READ_STATUS_FLAG:
        urt_ret[0] = 3;             /** lenght of data */
        urt_ret[1] = 'F';
        urt_ret[2] = '0';
        urt_ret[3] = '0';   // todo : for now fake responce -- should add real status of can
        xQueueSend(slTxQueue, (void *)urt_ret, portMAX_DELAY);
        break;
    
    case SET_ACCEPTANCE_CODE: 
        int acceptance_int = 0;
        sscanf((const char *) &data[1], "%08x", &acceptance_int);
        can_cfg->acceptance_code = (uint32_t)acceptance_int;
        break;

    case SET_ACCEPTANCE_MASK: 
        int acceptance_mask_int = 0;
        sscanf((const char *) &data[1], "%08x", &acceptance_mask_int);
        can_cfg->acceptance_mask = (uint32_t)acceptance_mask_int;
        break;

    default:
        break;
    }

    free(urt_ret);
}