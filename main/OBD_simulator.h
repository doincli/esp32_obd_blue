// Copyright 2015-2019 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#ifndef OBD_SIMULATOR_H
#define OBD_SIMULATOR_H

#include <stddef.h>
#include "esp_err.h"
#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <driver/twai.h>


#define ISO15765_11bit 1        /*!< protocol_t   */
#define ISO15765_29bit 0
#define BPS_500K   1            /*!< speed   */
#define BPS_250K   0


#define MSG_ID 0x7DF            /*!< MSG_ID the head of message of protocol   */
#define MSG_ID_EXP 0x18DB33F1 

// //CAN io的数量
// #define FIRST 1         
// #define SECOND 2


#define MAX_NUM_CAN 3

typedef enum 
{
    ISO15765_11bit_500K=0,       /*!< Show current search mode is ISO15765_11bit,speed is 500KB */
    ISO15765_11bit_250K,         /*!< Show current search mode is ISO15765_11bit,speed is 250KB */
    ISO15765_29bit_500K,         /*!< Show current search mode is ISO15765_29bit,speed is 500KB */
    ISO15765_29bit_250K          /*!< Show current search mode is ISO15765_29bit,speed is 250KB */
}protocol;


typedef struct 
{
    uint8_t  tx_port;
    uint8_t  rx_port;
}OBD_IO;

typedef OBD_IO * OBD_IO_Handle;

typedef struct 
{
    uint8_t    protocol_t;
    uint8_t    statu ;
    uint8_t    speed ;
    OBD_IO_Handle io_port;
    twai_general_config_t * g_config;
    twai_timing_config_t *  t_config;
}detect_config_t;

typedef detect_config_t* OBD_protocol_Handle;


esp_err_t OBD_protocol_init(uint8_t Can_num);

esp_err_t OBD_twai_init(OBD_protocol_Handle protocol_status,uint8_t Can_num);

esp_err_t OBD_twai_deinit();

esp_err_t io_init(uint8_t tx_port,uint8_t rx_port,uint8_t Can_num);



//检测这个协议情况是否正确
esp_err_t detect_get_protocol(OBD_protocol_Handle protocol_status, uint8_t Can_num);

//自动匹配正确的协议
esp_err_t Task_detectpro(OBD_protocol_Handle protocol_status , uint8_t Can_num);

//获取速度
uint32_t OBD_get_engine_speed_val_protocol(OBD_protocol_Handle protocol_status , uint8_t Can_num);


OBD_protocol_Handle handle_init(uint8_t Can_num);


#endif // OBD_SIMULATOR_H
