#include "self_test.h"
#include "esp_log.h"
#include "app.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
// #include "ebyte_e220x.h"

static const char* TAG = "self_test";
int8_t rssi;
static int speed;
float data_z = 0;
gpio_config_t io_conf;
#define led_io 3
int io_level = 0;

void timerCallback(TimerHandle_t xTimer) 
{
    io_level++;
    gpio_set_level(led_io, io_level % 2);
}

void vLED(int pvParameters)
{
    TimerHandle_t timer = xTimerCreate("MyTimer", pdMS_TO_TICKS(pvParameters), pdTRUE, NULL, timerCallback);
    xTimerStart(timer, portMAX_DELAY);
}

esp_err_t self_test()
{   
    //accuracy test
    esp_err_t ret = ESP_FAIL;
    ret = accuracy_test();
    if(ret == ESP_OK){
        ESP_LOGI(TAG,"accuracy test is pass, accuarcy is %f\n",data_z);
    }else{
        ESP_LOGE(TAG,"accuracy test is fail, accuarcy is %f\n",data_z);
        return ESP_FAIL;
    }
    
    //can test
    ret = can_test();
    if(ret == ESP_OK){
        ESP_LOGI(TAG,"can test is pass, speed is %d\n",speed);
    }else{
        ESP_LOGE(TAG,"can test is fail, speed is %d\n",speed);
        return ESP_FAIL;
    }
    
    //spi test
    ret = spi_test();
    if(ret == ESP_OK){
        ESP_LOGI(TAG,"spi test is pass, rssi is %d\n",rssi);
    }else{
        ESP_LOGE(TAG,"spi test is fail, rssi is %d\n",rssi);
        return ESP_FAIL;
    }
    return ESP_OK;
}

esp_err_t accuracy_test()
{
    data_z = qma7981_read_DZM();
    if (data_z >= 0.85 && data_z <= 1.15){
        return ESP_OK;
    }
    return ESP_FAIL;
}

esp_err_t can_test()
{
    speed = app_obd_get_speed();
    if (speed != -1){
        return ESP_OK;
    }
    return ESP_FAIL;
}

esp_err_t spi_test()
{
    ebyte_handle_t tmp = get_ebyte();
    rssi = Ebyte_getRSSI(tmp);
    if (rssi >= -120){
        return ESP_OK;
    }
    return ESP_FAIL;
}

void gpio_int()
{
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << led_io);
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 1;
    gpio_config(&io_conf);
}

void self_test_init()
{
    esp_err_t ret = self_test();
    //gpio_int();
    int time1 = 1000;
    int time2 = 100;
    if (ret == ESP_OK){
        vLED(time1);
    }else{
        vLED(time2);
    }
}