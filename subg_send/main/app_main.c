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
    app_master_init();
    app_subg_init();
    ESP_LOGI(TAG, "app_subg_init");
    app_ble_init();
    ESP_LOGI(TAG, "app_ble_init");
    app_obd_init();
    ESP_LOGI(TAG, "init done\n");
    self_test_init();
    ESP_LOGI(TAG, "self test done\n");
    while (1)
    {
        spi_test();
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    
    // vTaskDelay(pdMS_TO_TICKS(1000));
}


