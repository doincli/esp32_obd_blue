#ifndef FUNC_H
#define FUNC_H

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <driver/twai.h>


//protocol_t  
#define ISO15765_11bit 1
#define ISO15765_29bit 0
#define BPS_500K   1
#define BPS_250K   0

//msg_id
#define MSG_ID 0x7DF
#define MSG_ID_EXP 0x18DB33F1 



typedef enum 
{
    ISO15765_11bit_500K=1, 
    ISO15765_11bit_250K, 
    ISO15765_29bit_500K, 
    ISO15765_29bit_250K
}protocol;



typedef struct 
{
    uint8_t    protocol_t;
    uint8_t    statu ;
    uint8_t    speed ;
    //uint32_t   msg_id;   //11 bit standard format ID
}detect_config_t;

void config_struct(detect_config_t *t_config);

void OBD_twai_init(detect_config_t *t_config);


void OBD_twai_deinit(void);


#endif // FUNC_H