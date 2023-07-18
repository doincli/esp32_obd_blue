#include "app_qam.h"

float X_AXIS_A;
float Y_AXIS_A;
float Z_AXIS_A;
static const char* TAG = "app_qam";

void app_qam_task()
{
    while(1){	   
        X_AXIS_A = qma7981_read_DXM() ;
        Y_AXIS_A = qma7981_read_DYM() ;
        Z_AXIS_A = qma7981_read_DZM() ;
        ESP_LOGI(TAG, "X_A = %.3f g, Y_A = %.3f g, Z_A = %.3f g. \n", X_AXIS_A, Y_AXIS_A, Z_AXIS_A);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_master_init()
{
    qma7981_master_init();
    ESP_ERROR_CHECK(qma7981_setMode(QMA7981_MODE_100K_cmd));	
    ESP_ERROR_CHECK(qma7981_setAcc(QMA7981_RAG_2g_cmd));
    xTaskCreate(app_qam_task, "app_qam_task", 4096, NULL, 5, NULL);
}

float app_get_qam_x_acc()
{
    return X_AXIS_A;
}