#include "OBD_detect.h"
#include "gatts.h"

#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include "sdkconfig.h"
#include "nvs_flash.h"
#include "ebyte_kfifo.h"
#define TX_GPIO_NUM 19
#define RX_GPIO_NUM 20

#define EBYTE_HOST      SPI2_HOST

#define PIN_NUM_MISO    5
#define PIN_NUM_MOSI    6
#define PIN_NUM_CLK     7
#define PIN_NUM_CS      0

#define PIN_NUM_BUSY    2
#define PIN_NUM_RST     3



uint8_t speed;
ebyte_status_t my_status;
Ebyte_FIFO_t my_fifo;
void app_main(void)
{


    uint8_t pingLength = 0;
    //uint8_t rev_data[EBYTE_MAX_SIZE];
    for(int i = 0; i < 2; ++i){
        printf("start: %d\n", i);
        ets_delay_us(1000000);
    }

    Ebyte_FIFO_Init(&my_fifo);


    ebyte_config_t my_ebyte_config = {
        .spi_id = EBYTE_HOST,
        .radio_mode = RADIO_MODE_LORA,
        .spi_speed_hz = 10 * 1000 * 1000,
        .miso_io = PIN_NUM_MISO,
        .mosi_io = PIN_NUM_MOSI,
        .sclk_io = PIN_NUM_CLK,
        .cs_io = PIN_NUM_CS,
        .busy_io = PIN_NUM_BUSY,
        .rst_io = PIN_NUM_RST,
        .dio1_io = -1,
        .rxen_io = -1,
        .txen_io = -1,
    };

    Ebyte_Init( my_ebyte_config, &my_status );

    printf("init done\n");


    esp_err_t ret;

    ret = blue_init();
    if (ret != ESP_OK)
    {
        printf("init fail\n");
        return ;
    }

    obd_protocol_handle protocol_status = obd_create(TX_GPIO_NUM,RX_GPIO_NUM);
  
    while (1)
    {   
        vTaskDelay(190);
        speed = obd_get_engine_speed_val(protocol_status);
        printf("speed is %d\n",speed);
    }
    
    obd_delete(protocol_status);
    return;
}
/*

*/