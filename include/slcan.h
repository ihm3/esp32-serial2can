#ifndef __CAN_2_USB
#define __CAN_2_USB

#include "string.h"
#include "stdint.h"

typedef enum {

    OPEN_CAN = 'O',             // open can channel
    CLOSE_CAN  =   'C',         // close can channel
    SET_CAN =  'S',             // set bit rate
    SET_CAN_CUSTOM = 's',       // set custom bit rate
    TRANSMIT_STD  = 't',        // transmit standard frame message
    TRANSMIT_EXTD = 'T',        // transmit extended frame  message
    TRANSMIT_STD_RTR = 'r',     // transmit std RTR  frame message
    TRANSMIT_EXTD_RTR = 'R',    // transmit extd  RTR  frame message
    SET_TIMESTAMP = 'Z',        //  on/off  timestamp for each received message
    GET_VERSION  = 'V',         // get version  of hardware & software
    GET_SERIAL_NUMBER = 'N',    // get serial  number of canbus
    READ_STATUS_FLAG = 'F',     //
    SET_ACCEPTANCE_CODE = 'M',  // set acceptance code
    SET_ACCEPTANCE_MASK = 'm',  // set acceptance mask
    ACK = '\r',
    NACK = '\a',
} slcan_protochol_t;

/**
 * 
 * @brief
 * 
 *  tydef enum for slcan protochol bit rate define
 */
typedef enum {
    SET_10K = '0',
    SET_20K = '1',
    SET_50K = '2',
    SET_100k = '3',
    SET_125k = '4',
    SET_250k = '5',
    SET_500k = '6',
    SET_750k = '7',
    SET_1M   = '8',
} slcan_bit_rete_t;

typedef uint32_t slcan_acceptance_code;
typedef uint32_t slcan_acceptance_mask;

typedef struct 
{
    slcan_bit_rete_t bitrate;
    slcan_acceptance_code acceptance_code;
    slcan_acceptance_mask  acceptance_mask;
}slcan_can_cfg_t;

void serial_rx_task(void *arg);
void process_rx_data(uint8_t *data, slcan_can_cfg_t* can_cfg);

#endif // !__CAN_2_USB