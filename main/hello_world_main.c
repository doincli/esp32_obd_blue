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








void app_main(void)
{   

    
    detect_config_t t_config;
    config_struct(&t_config);

    OBD_twai_init(&t_config);
    
	Task_detectpro(&t_config);
  
     
    OBD_twai_init(&t_config);
    for (int i = 0; i < 10; i++)
    {
        uint32_t speed = OBD_get_engine_speed_val_protocol(&t_config);
        printf("speed is %ld\n",speed);
    }
    
    OBD_twai_deinit();

}
