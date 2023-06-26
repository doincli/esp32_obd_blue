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

#define TX_GPIO_NUM 2
#define RX_GPIO_NUM 3

void app_main(void)
{   
    //实例化io口 ，可以有两个
    config_io(RX_GPIO_NUM,TX_GPIO_NUM,FIRST);

    //实例化一个can总线结构体 包括协议和速度
    detect_config_t t_config;

    //初始化结构体
    config_detect(&t_config);

    //打开总线
    OBD_twai_init(&t_config,FIRST);
    
    //自动搜索协议，并且保存在结构体中
	Task_detectpro(&t_config);
  
    //获取速度
    for (int i = 0; i < 10; i++)
    {
        uint32_t speed = OBD_get_engine_speed_val_protocol(&t_config);
        printf("speed is %ld\n",speed);
    }
    
    OBD_twai_deinit();
}
