#include "qam.h"

SemaphoreHandle_t read_mux = NULL;
static const char* TAG = "qma";

esp_err_t qma7981_master_init(void)
 {
    int i2c_master_port = I2C_MASTER_NUM;
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };
    esp_err_t err = i2c_param_config(i2c_master_port, &conf);
    if (err != ESP_OK){
        return err;
    }
    read_mux = xSemaphoreCreateMutex();
    if (read_mux == NULL){
        ESP_LOGE(TAG, "mux init fail\n");
        return ESP_FAIL;
    }
    
    return i2c_driver_install(i2c_master_port, conf.mode, 1, I2C_MASTER_TX_BUF_DISABLE, 0);
}

 esp_err_t qma7981_register_read(uint8_t reg_addr, uint8_t *data, size_t len)
{
    return i2c_master_write_read_device(I2C_MASTER_NUM, QMA7981_ADDR, &reg_addr, 1, data, len, 1000 / portTICK_PERIOD_MS);
}

 esp_err_t qma7981_register_write_byte(uint8_t reg_addr, uint8_t cmd)
{
    int ret;
    uint8_t write_buf[2] = {reg_addr, cmd};
    ret = i2c_master_write_to_device(I2C_MASTER_NUM, QMA7981_ADDR, write_buf, sizeof(write_buf), 1000 / portTICK_PERIOD_MS);
    return ret;
}

float qma7981_read_DXM()
{
    uint8_t data[2];
    int16_t XA;
    float X_AXIS_A;
    BaseType_t esp_status = xSemaphoreTake(read_mux, 1000/portTICK_PERIOD_MS);
    if (esp_status != pdPASS){
        ESP_LOGE(TAG, "mux take fail\n");
    }
    qma7981_register_read(QMA7981_DXM_ADDR, data, 2);
    esp_status = xSemaphoreGive(read_mux);
    if (esp_status != pdPASS){
        ESP_LOGE(TAG, "mux give fail\n");
    }
    XA = (data[0] & 0xFC) | (data[1]<< 8);
    X_AXIS_A = (float) XA / QMA7981_MAX_VALUE ;
    return X_AXIS_A;
}

float qma7981_read_DYM()
{
    uint8_t data[2];
    int16_t YA;
    float Y_AXIS_A;
    BaseType_t esp_status = xSemaphoreTake(read_mux, 1000/portTICK_PERIOD_MS);
    if (esp_status != pdPASS){
        ESP_LOGE(TAG,  "mux take fail\n");
    }
    qma7981_register_read(QMA7981_DYM_ADDR, data, 2);
    esp_status =xSemaphoreGive(read_mux);
    if (esp_status != pdPASS){
        ESP_LOGE(TAG,  "mux give fail\n");
    }
    YA = (data[0] & 0xFC) | (data[1]<< 8);
    Y_AXIS_A = (float) YA / QMA7981_MAX_VALUE ;
    return Y_AXIS_A;
}

float qma7981_read_DZM()
{
    uint8_t data[2];
    int16_t ZA;
    float Z_AXIS_A;
    BaseType_t esp_status = xSemaphoreTake(read_mux, 1000/portTICK_PERIOD_MS);
    if (esp_status != pdPASS){
        ESP_LOGE(TAG, "mux take fail\n");
    }
    qma7981_register_read(QMA7981_DZM_ADDR, data, 2);
    esp_status = xSemaphoreGive(read_mux);
    if (esp_status != pdPASS){
        ESP_LOGE(TAG, "mux give fail\n");
    }
    ZA = (data[0] & 0xFC) | (data[1]<< 8);
    Z_AXIS_A = (float) ZA / QMA7981_MAX_VALUE ;
    return Z_AXIS_A;
}

esp_err_t qma7981_setMode(uint8_t cmd)
{
    return  qma7981_register_write_byte(QMA7981_MODE_ADDR, cmd);
}

esp_err_t qma7981_setAcc(uint8_t cmd)
{
    return  qma7981_register_write_byte(QMA7981_RANGE_ADDR, cmd);
}

esp_err_t qma7981_driver_delete()
{
   return  i2c_driver_delete(I2C_MASTER_NUM);  
}