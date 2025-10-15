#ifndef __CAN_HANDLE__
#define __CAN_HANDLE__

#include "driver/twai.h"
#include "esp_err.h"
#include "freertos/portmacro.h"
#include "slcan.h"

esp_err_t init_canbus(slcan_can_cfg_t *cfg);
esp_err_t transmit_from_sl(uint32_t id, uint8_t dlc, bool extd, bool rtr, uint8_t* data);

#endif