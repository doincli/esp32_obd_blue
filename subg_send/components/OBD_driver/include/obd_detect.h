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
#ifndef OBD_DETECT_H
#define OBD_DETECT_H

#include <stddef.h>
#include "esp_err.h"
#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <driver/twai.h>


#define ISO15765_11bit 1        /*!< protocol_t   */
#define ISO15765_29bit 0
#define BPS_500K   1            /*!< speed   */
#define BPS_250K   0

#define MSG_ID 0x7DF            /*!< MSG_ID the head of message of protocol   */
#define MSG_ID_EXP 0x18DB33F1 

typedef enum 
{
    ISO15765_11bit_500K=0,       /*!< Show current search mode is ISO15765_11bit,speed is 500KB */
    ISO15765_11bit_250K,         /*!< Show current search mode is ISO15765_11bit,speed is 250KB */
    ISO15765_29bit_500K,         /*!< Show current search mode is ISO15765_29bit,speed is 500KB */
    ISO15765_29bit_250K          /*!< Show current search mode is ISO15765_29bit,speed is 250KB */
} protocol;

typedef struct 
{
  uint8_t  tx_port;            /*!< IO port */
  uint8_t  rx_port;
} obd_io;

typedef obd_io * obd_io_handle;
typedef struct 
{
  uint8_t    protocol_t;          /*!< protocol can be defined as  ISO15765_11bit and ISO15765_29bit */
  uint8_t    statu ;              /*!< current search mode */
  uint8_t    speed ;              /*!< speed can be defined as  BPS_500K and BPS_250K */
  obd_io_handle io_port;
  twai_general_config_t * g_config;
  twai_timing_config_t *  t_config;
} detect_config_t;

typedef detect_config_t* obd_protocol_handle;

/**
  * @brief  breate a obd struct and init ,can match the right protocol
  * 
  * @param  obd_handle  can bus handle
  * @param  tx_port     the number of tx port
  * @param  rx_port     the number of rx port
  * 
  * @return can bus handle
  *  
  */
obd_protocol_handle obd_create(uint8_t tx_port,uint8_t rx_port);

/**
  * @brief  get the speed of obd
  * 
  * @param  obd_handle  can bus handle
  * 
  * @return the speed of obd
  * 
  */
int obd_get_engine_speed_val(obd_protocol_handle protocol_status );

/**
  * @brief  delete the handle
  * 
  * @param  obd_handle  can bus handle
  * 
  * @return
  *     - ESP_OK: succeed
  *     - others: fail
  */
esp_err_t obd_delete(obd_protocol_handle obd_handle);


/**
  * @brief  close the bus configuration  
  * 
  * @return
  *     - ESP_OK: succeed
  *     - others: fail
  */
esp_err_t obd_twai_deinit();

/**
  * @brief  modifed the bus configuration 
  * 
  * @param  obd_handle  can bus handle
  * 
  * @return
  *     - ESP_OK: succeed
  *     - others: fail
  */
esp_err_t obd_twai_modifed(obd_protocol_handle obd_handle);

/**
  * @brief  detect the protocol is right
  * 
  * @param  obd_handle  can bus handle
  * 
  * @return
  *     - ESP_OK: succeed
  *     - others: fail
  */
esp_err_t obd_detect(obd_protocol_handle obd_handle);

/**
  * @brief  auto match the right protocol
  * 
  * @param  obd_handle  can bus handle
  * 
  * @return
  *     - ESP_OK: succeed
  *     - others: fail
  */
esp_err_t obd_detect_match(obd_protocol_handle obd_handle);

#endif // OBD_DETECT_H