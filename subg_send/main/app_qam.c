#include "app_qam.h"

static float x_accuracy;
static float y_accuracy;
static float z_accuracy;
static const char* TAG = "app_qam";

void app_qam_task()
{
    while(1){	   
        x_accuracy = qma7981_read_DXM() ;
        y_accuracy = qma7981_read_DYM() ;
        z_accuracy = qma7981_read_DZM() ;
        ESP_LOGI(TAG, "X_A = %.3f g, Y_A = %.3f g, Z_A = %.3f g. \n", x_accuracy, y_accuracy, z_accuracy);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_master_init()
{
    ESP_ERROR_CHECK(qma7981_master_init());
    ESP_ERROR_CHECK(qma7981_setMode(QMA7981_MODE_100K_cmd));	
    ESP_ERROR_CHECK(qma7981_setAcc(QMA7981_RAG_2g_cmd));
    xTaskCreate(app_qam_task, "app_qam_task", 4096, NULL, 5, NULL);
}

float app_get_qam_x_acc()
{
    return x_accuracy;
}