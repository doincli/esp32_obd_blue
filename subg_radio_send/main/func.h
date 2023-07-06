#ifndef FUNC_H
#define FUNC_H

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "sdkconfig.h"
#include "ebyte.h"
#include "math.h"
#include "OBD_detect.h"
#include "gatts.h"
#include "esp_system.h"
#include "nvs_flash.h"

#define TX_GPIO_NUM 19
#define RX_GPIO_NUM 20

#define EBYTE_HOST      SPI2_HOST

#define PIN_NUM_MISO    5
#define PIN_NUM_MOSI    6
#define PIN_NUM_CLK     7
#define PIN_NUM_CS      0

#define PIN_NUM_BUSY    2
#define PIN_NUM_RST     3

#define Frame_len  3


#define EBYTE_MAX_SIZE 256

//ebyte的结构体
typedef struct {
    uint8_t data[EBYTE_MAX_SIZE];
    int front;
    int rear;
    int size;
} Ebyte_FIFO_t;


//重传的结构体
typedef struct data
{
    uint8_t data[Frame_len];
    uint8_t seq;
    uint8_t old_data[Frame_len];
    uint8_t old_seq;
}frame_data;
typedef frame_data * frame_handle;


//数据更新
void data_refresh(frame_handle frame,uint16_t data);


//序列更新
void seq_refresh(frame_handle frame);

//fifo init
void fifo_init( Ebyte_FIFO_t *my_fifo);

void Ebyte_FIFO_Init(Ebyte_FIFO_t* queue);

bool Ebyte_FIFO_IsEmpty(Ebyte_FIFO_t* queue);

bool Ebyte_FIFO_IsFull(Ebyte_FIFO_t* queue);

int Ebyte_FIFO_Write(Ebyte_FIFO_t* queue, uint8_t* pData, int len) ;

int Ebyte_FIFO_Read(Ebyte_FIFO_t* queue, uint8_t* pData, int len) ;

#endif // FUNC_H
