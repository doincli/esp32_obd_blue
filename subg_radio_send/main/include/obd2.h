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


#define EBYTE_HOST      SPI2_HOST

#define PIN_NUM_MISO    4
#define PIN_NUM_MOSI    5
#define PIN_NUM_CLK     7
#define PIN_NUM_CS      6

#define PIN_NUM_BUSY    1
#define PIN_NUM_RST     0

#define Frame_len  3


#define EBYTE_MAX_SIZE 256

//ebyte的结构体
typedef struct {
    uint8_t data[EBYTE_MAX_SIZE];
    int front;
    int rear;
    int size;
} Ebyte_FIFO_t;


//fifo init
void fifo_init( Ebyte_FIFO_t *my_fifo);

void Ebyte_FIFO_Init(Ebyte_FIFO_t* queue);

bool Ebyte_FIFO_IsEmpty(Ebyte_FIFO_t* queue);

bool Ebyte_FIFO_IsFull(Ebyte_FIFO_t* queue);

int Ebyte_FIFO_Write(Ebyte_FIFO_t* queue, uint8_t* pData, int len) ;

int Ebyte_FIFO_Read(Ebyte_FIFO_t* queue, uint8_t* pData, int len) ;


/**
  * @brief  get the ebyte_config
  */
ebyte_config_t* get_ebyte_config();


#endif // FUNC_H
