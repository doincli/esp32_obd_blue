/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "OBD_detect.h"


#define TX_GPIO_NUM 2
#define RX_GPIO_NUM 3

void app_main(void)
{   
    

    obd_protocol_handle protocol_status = obd_create(TX_GPIO_NUM,RX_GPIO_NUM);
  

    for (int i = 0; i < 10; i++)
    {
        uint32_t speed = obd_get_engine_speed_val(protocol_status);
        printf("speed is %ld\n",speed);
    }
    
    obd_delete(protocol_status);
}
