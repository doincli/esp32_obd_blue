#include "obd2.h"
static const char* TAG = "obd";
uint8_t speed;

uint8_t app_obd_get_speed()
{
    return speed;
}

//task1  get speed 1/s
void app_obd_speed_task(void *pvParameters) 
{
    obd_protocol_handle obd_handle = (obd_protocol_handle)pvParameters;
    SemaphoreHandle_t obd_api_mux = xSemaphoreCreateMutex();

    while (1) {
        BaseType_t mux_ret = xSemaphoreTake(obd_api_mux, pdMS_TO_TICKS(200));
        if (mux_ret != pdPASS) {
            ESP_LOGE(TAG, "take mux fail\n");
            continue;
        }
        speed = obd_get_engine_speed_val(obd_handle);
        mux_ret =  xSemaphoreGive(obd_api_mux);
        if (mux_ret != pdPASS) {
            ESP_LOGE(TAG, "give mux fail\n");
        }
        ESP_LOGI(TAG, "speed is %d\n", speed);
        
        vTaskDelay(pdMS_TO_TICKS(1000)); // 延时 1500 毫秒
    }
}

void app_obd_init()
{
    //odb init
    TaskHandle_t speed_task;
    obd_protocol_handle obd_handle = obd_create(TX_GPIO_NUM, RX_GPIO_NUM);

    xTaskCreate(app_obd_speed_task, "app_obd_speed_task", 4096, obd_handle, 5, &speed_task);
}