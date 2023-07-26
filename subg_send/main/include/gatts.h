#ifndef GATTS_H
#define GATTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_system.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_bt.h"
#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_bt_defs.h"
#include "esp_bt_main.h"
#include "esp_gatt_common_api.h"
#include "obd_detect.h"
#include "sdkconfig.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* Attributes State Machine */
enum
{
    IDX_SVC,
    IDX_CHAR_A,
    IDX_CHAR_VAL_A,
    IDX_CHAR_CFG_A,

    IDX_CHAR_B,
    IDX_CHAR_VAL_B,

    IDX_CHAR_C,
    IDX_CHAR_VAL_C,

    IDX_CHAR_D,
    IDX_CHAR_VAL_D,

    HRS_IDX_NB,
};

enum
{
    IDX_SVC2,
    IDX_CHAR_A2,
    IDX_CHAR_VAL_A2,

    HRS_IDX_NB2,
};

/**
  * @brief  init bluetooth
  * 
  */
esp_err_t blue_init();

/**
  * @brief  get blue test value
  * 
  * @return value of blue_self_test
  */
uint8_t get_blue_value();

#endif // GATTS_H
