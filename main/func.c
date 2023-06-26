#include "func.h"

static const twai_timing_config_t t_config_250 = TWAI_TIMING_CONFIG_250KBITS();
static const twai_timing_config_t t_config_500 = TWAI_TIMING_CONFIG_500KBITS();
static const twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

//gpio 
#define TX_GPIO_NUM 2
#define RX_GPIO_NUM 3

//配置IO
 const twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(TX_GPIO_NUM, RX_GPIO_NUM, TWAI_MODE_NORMAL);


void OBD_twai_init(detect_config_t  *t_config)
{   
    if (t_config->speed == BPS_250K )
    {
       ESP_ERROR_CHECK(twai_driver_install(&g_config, &t_config_250, &f_config));
    }else
    {
       ESP_ERROR_CHECK(twai_driver_install(&g_config, &t_config_500, &f_config));
    }
    ESP_ERROR_CHECK(twai_start());
}


void OBD_twai_deinit(void)
{
    ESP_ERROR_CHECK(twai_stop());
    ESP_ERROR_CHECK(twai_driver_uninstall());
}


void config_struct(detect_config_t *t_config){
    t_config->protocol_t = ISO15765_11bit;
    t_config->speed = BPS_500K;
    t_config->statu = ISO15765_11bit_500K;
}