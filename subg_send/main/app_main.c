/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 * 
 * need to exchange the pin of can    the define in bod2.h
 */

#include "app.h"


void app_main(void)
{   
    master_init();
    app_subg_init();
    printf("app_subg_init");
    app_ble_init();
     printf("app_ble_init");
    app_obd_init();
    printf("init done\n");

}


