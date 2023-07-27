/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 * 
 * need to exchange the pin of can    the define in bod2.h
 */

#include "app.h"
#include "self_test.h"

static const char* TAG = "app_main";

void app_main(void)
{   
    //self_test
    #ifdef SELF_TEST
        #ifdef SUBG_CMT2300
            app_subg_cmt2300_init();
        #else
            app_subg_init();
        #endif
            self_test_init();
            ESP_LOGI(TAG, "self test done\n");
    #else
        #ifdef SUBG_CMT2300
            app_subg_cmt2300_init();
        #else
            app_subg_init();
        #endif
            ESP_LOGI(TAG, "app_subg_init");
            app_ble_init();
            ESP_LOGI(TAG, "app_ble_init");
            app_obd_init();
            ESP_LOGI(TAG, "init done\n");
            app_master_init();
    #endif
}


