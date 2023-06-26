#include "func.h"

static const twai_timing_config_t t_config_250 = TWAI_TIMING_CONFIG_250KBITS();
static const twai_timing_config_t t_config_500 = TWAI_TIMING_CONFIG_500KBITS();
static const twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

//gpio 
#define TX_GPIO 6
#define RX_GPIO 8

extern 

//配置IO
twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(TX_GPIO, RX_GPIO, TWAI_MODE_NORMAL);

twai_general_config_t g_config1 = TWAI_GENERAL_CONFIG_DEFAULT(TX_GPIO, RX_GPIO, TWAI_MODE_NORMAL);


void OBD_twai_init(detect_config_t  *t_config,int can_num)
{   
    if (t_config->speed == BPS_250K )
    {
        if (can_num == FIRST)
        {
            ESP_ERROR_CHECK(twai_driver_install(&g_config, &t_config_250, &f_config));
        }else{
            ESP_ERROR_CHECK(twai_driver_install(&g_config1, &t_config_250, &f_config));
        }      
    }else
    {
        if (can_num == FIRST)
        {
            ESP_ERROR_CHECK(twai_driver_install(&g_config, &t_config_500, &f_config));
        }else{
            ESP_ERROR_CHECK(twai_driver_install(&g_config1, &t_config_500, &f_config));
        }  
    }
    ESP_ERROR_CHECK(twai_start());
}


void OBD_twai_deinit(void)
{
    ESP_ERROR_CHECK(twai_stop());
    ESP_ERROR_CHECK(twai_driver_uninstall());
}


void config_detect(detect_config_t *t_config){
    t_config->protocol_t = ISO15765_11bit;
    t_config->speed = BPS_500K;
    t_config->statu = ISO15765_11bit_500K;
}


void config_io(int rx_port,int tx_port,int can_num){
    if (can_num == FIRST)
    {
        g_config.tx_io = tx_port;
        g_config.rx_io = rx_port;
    }else
    {
        g_config1.tx_io = tx_port;
        g_config1.rx_io = rx_port;
    }
}