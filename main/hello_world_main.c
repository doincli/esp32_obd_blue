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
#include "OBD_simulator.h"
#include "func.h"






//一个配置结构体，不需要改变，如果有第二个同样赋值就好了，可以自动匹配
detect_config_t t_config = {
    .protocol_t = ISO15765_11bit,
    .speed = BPS_500K,
    .statu = ISO15765_11bit_500K
};



void app_main(void)
{   
    printf("step 1\n");
    OBD_twai_init(&t_config);
     printf("step 2\n");
	Task_detectpro(&t_config);
     printf("step 3\n");
     printf("protocol_t is %d ,speed is %d\n",t_config.protocol_t,t_config.speed);
     OBD_twai_init(&t_config);
    for (int i = 0; i < 10; i++)
    {
        uint32_t speed = OBD_get_engine_speed_val_protocol(&t_config);
        printf("speed is %ld\n",speed);
    }
    
    OBD_twai_deinit();

}
