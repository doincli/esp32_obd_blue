#ifndef FUNC_H
#define FUNC_H

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "sdkconfig.h"
#include "ebyte.h"
#include "math.h"
#include "obd_detect.h"
#include "gatts.h"
#include "esp_system.h"
#include "nvs_flash.h"

//obd 引脚
#define TX_GPIO_NUM 19
#define RX_GPIO_NUM 18


//spi 引脚
#define EBYTE_HOST      SPI2_HOST

//C3
#define PIN_NUM_MISO    4
#define PIN_NUM_MOSI    5
#define PIN_NUM_CLK     7
#define PIN_NUM_CS      6

#define PIN_NUM_BUSY    1
#define PIN_NUM_RST     0

/*
SPI 速率设置在subg.c文件里面
*/

//C6
// #define PIN_NUM_MISO    5
// #define PIN_NUM_MOSI    6
// #define PIN_NUM_CLK     7
// #define PIN_NUM_CS      0

// #define PIN_NUM_BUSY    2
// #define PIN_NUM_RST     3

#define Frame_len  3


#define EBYTE_MAX_SIZE 256

uint8_t app_obd_get_speed();

/**
  * @brief  get the ebyte_config
  */
ebyte_config_t* get_ebyte_config();


#endif // FUNC_H