/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include "sdkconfig.h"
#include "ebyte_kfifo.h"
#include "ebyte.h"

#define EBYTE_HOST      SPI2_HOST

#define PIN_NUM_MISO    5
#define PIN_NUM_MOSI    6
#define PIN_NUM_CLK     7
#define PIN_NUM_CS      0

#define PIN_NUM_BUSY    2
#define PIN_NUM_RST     3
#define frame_len       3

uint8_t BufferPing[5] = {'p', 'i', 'n', 'g'};
uint8_t BufferPong[5] = {'p', 'o', 'n', 'g'};
Ebyte_FIFO_t my_fifo;

void app_main(void)
{
  //  uint8_t pingLength = 0;
    uint8_t rev_data[EBYTE_MAX_SIZE];
    for(int i = 0; i < 3; ++i){
        printf("start: %d\n", i);
        ets_delay_us(1000);
    }

    Ebyte_FIFO_Init(&my_fifo);

    ebyte_status_t my_status;

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

    uint8_t rec_seq = 1;  //期待接收的序列
    //uint8_t data_pro = 1;
    while(1){
        Ebyte_Receive( &my_status, rev_data, 0 );
       
        uint8_t seq = rev_data[frame_len-1];
        Ebyte_Send(&my_status, &seq, 1,0);
        if (seq != rec_seq)
        {
            printf("rec right\n");
            printf("seq is %d \n", rev_data[frame_len-1]);
            for (int i = 0; i < frame_len; i++)
            {
                printf("%x ",rev_data[i]);
            }
            printf("\n");
            rec_seq = seq;
        }
        
        ets_delay_us(500);


        // if(recv_len != -1 ){
        //     printf("len is %d:  " ,recv_len);
        //     for(int i = 0; i < recv_len; ++i){
        //         printf("%d ", rev_data[i]);
        //     }
        //     printf("\n");
        // }
        // else{
        //     printf("rx timeout\n");
        // }
    }

    Ebyte_DeInit( &my_status );
}


