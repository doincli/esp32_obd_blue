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
#include "math.h"
#include "OBD_detect.h"
#include "gatts.h"


#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include "sdkconfig.h"
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

uint8_t speed;
uint16_t my_common;

typedef struct data
{
    uint8_t data[Frame_len];
    uint8_t seq;
    uint8_t old_data[Frame_len];
    uint8_t old_seq;
}frame_data;

typedef frame_data * frame_handle;

void data_refresh(frame_handle frame,uint16_t data){
   
    for (int i = 0; i < Frame_len-1; i++)
    {
        frame->data[Frame_len-2-i] = (uint8_t)data;
        frame->old_data[i] = frame->data[i];
        data = (data >> 8);
    }

}

void seq_refresh(frame_handle frame){
    frame->old_seq = frame->seq;
    frame->old_data[Frame_len-1] = frame->old_seq;
    frame->seq++;
    frame->data[Frame_len-1] = frame->seq;
}


void app_main(void)
{   

    //ble init 
    uint16_t my_buff;
    esp_err_t ret;
    BaseType_t com_ret;
    ret = blue_init();
    if (ret != ESP_OK)
    {
        printf("init fail\n");
        return ;
    }

    obd_protocol_handle protocol_status = obd_create(TX_GPIO_NUM,RX_GPIO_NUM);
    esp_gatts_set_obd(protocol_status);



    //radio
    frame_data frame1 = {
        .data = {0},
        .seq = 1,
        .old_data = {0},
        .old_seq = 0,
    };
    frame1.data[Frame_len-1] = 1;

    Ebyte_FIFO_t my_fifo;

    for(int i = 0; i < 3; ++i){
        printf("start: %d\n", i);
        ets_delay_us(10000);
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

    printf("init done\n");


   


    while (1)
    {
        com_ret = esp_gatts_queue_rec(&my_buff);
        if (com_ret == pdPASS)
        {
            data_refresh(&frame1,my_buff);
            Ebyte_Send( &my_status, frame1.data, Frame_len, 0) ;
            seq_refresh(&frame1);
            for (int i = 0; i < Frame_len; i++)
            {
                printf("%d, ",frame1.data[i]);
            }
            printf("\n");
            break;
        }
    }


    

  
    
    
    uint8_t rec_seq[100] = {0};
    // printf("frame seq = %d\n",frame1.seq);
    // printf("data seq = %d\n",frame1.data[4]);

    while(1){
        Ebyte_Receive(&my_status, rec_seq, 2000);
        printf("seq  is %d\n",rec_seq[0]);
        printf("frame 1 seq  is %d\n",frame1.seq);
        if (abs(rec_seq[0] - frame1.old_data[Frame_len-1]) > 1)
        {
            printf("step 1\n");
            frame1.seq = rec_seq[0];
            frame1.data[Frame_len-1] = rec_seq[0];
        }
        
        if (frame1.seq == rec_seq[0])
        {
            //接收正确
           // printf("rec right\n");
            com_ret = esp_gatts_queue_rec(&my_buff);
            if (com_ret == pdPASS){
                data_refresh(&frame1,my_buff);
                Ebyte_Send( &my_status, frame1.data, Frame_len, 0 );
                seq_refresh(&frame1);
           //     printf("rec send right\n");
            }else if (com_ret == errQUEUE_EMPTY)
            {
                vTaskDelay(1000);
            }
            
            
            for (int i = 0; i < Frame_len; i++)
            {
                printf("%x, ",frame1.data[i]);
            }
            printf("\n");
            
            // printf("frame seq = %d\n",frame1.seq);
            // printf("data seq = %d\n",frame1.data[4]);
        }else
        {
            //接收错误
            printf("send error\n");
             for (int i = 0; i < Frame_len; i++)
            {
                printf("%d, ",frame1.old_data[i]);
            }
            printf("\n");
            Ebyte_Send( &my_status, frame1.old_data, Frame_len, 0 );
        }
        ets_delay_us(1500);
    }
    Ebyte_DeInit( &my_status );
}


