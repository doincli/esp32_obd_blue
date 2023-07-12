
#ifndef APP_H
#define APP_H

#include "esp_err.h"
#include "gatts.h"
#include "obd2.h"

esp_err_t app_ble_init();
uint8_t app_obd_get_speed();
void app_obd_init();
void app_subg_send_and_recv(uint32_t ticks_to_wait,uint16_t data);
void app_subg_init();
ebyte_status_t app_get_status();

#endif