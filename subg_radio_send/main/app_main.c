/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "obd2.h"
#include "app.h"

//车速
// uint8_t speed;
// SemaphoreHandle_t mux;
// QueueHandle_t my_que;




void app_main(void)
{   
    //blue init
    app_subg_init();
    
    app_ble_init();

    //odb init
    app_obd_init();


    printf("init done\n");
    
}


