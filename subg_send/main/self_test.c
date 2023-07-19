#include "self_test.h"
#include "esp_log.h"
#include "app.h"
// #include "ebyte_e220x.h"

static const char* TAG = "self_test";
int8_t rssi;
static int speed;
float data_z;

void self_test()
{   
    //accuracy test
    esp_err_t ret = ESP_FAIL;
    ret = accuracy_test();
    if(ret == ESP_OK){
        ESP_LOGI(TAG,"accuracy test is pass, accuarcy is %f\n",data_z);
    }else{
        ESP_LOGE(TAG,"accuracy test is fail, accuarcy is %f\n",data_z);
    }
    
    //can test
    ret = can_test();
    if(ret == ESP_OK){
        ESP_LOGI(TAG,"can test is pass, speed is %d\n",speed);
    }else{
        ESP_LOGE(TAG,"can test is fail, speed is %d\n",speed);
    }
    
    //spi test
    ret = spi_test();
    if(ret == ESP_OK){
        ESP_LOGI(TAG,"spi test is pass, rssi is %d\n",rssi);
    }else{
        ESP_LOGE(TAG,"spi test is fail, rssi is %d\n",rssi);
    }
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