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


#define TX_GPIO_NUM 2
#define RX_GPIO_NUM 3
#define can_num  1
void app_main(void)
{   
    OBD_protocol_Handle protocol_status1 = handle_init(can_num);
    //实例化io口 ，可以有两个
    io_init(TX_GPIO_NUM,RX_GPIO_NUM,can_num);
    OBD_protocol_init(can_num);


    //打开总线
    OBD_twai_init(protocol_status1,can_num);
    
    //自动搜索协议，并且保存在结构体中
	Task_detectpro(protocol_status1,can_num);
  
    //获取速度
    for (int i = 0; i < 10; i++)
    {
        uint32_t speed = OBD_get_engine_speed_val_protocol(protocol_status1,can_num);
        printf("speed is %ld\n",speed);
    }
    
    OBD_twai_deinit();
}
