/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "func.h"

//车速
uint8_t speed;
SemaphoreHandle_t mux;
QueueHandle_t my_que;
ebyte_status_t my_status;
uint16_t my_buff;
frame_data frame1 = {
    .data = {0},
    .seq = 1,
    .old_data = {0},
    .old_seq = 0,
};

//task1  get speed 1/s
void get_speed(void *pvParameters) {
    obd_protocol_handle my_protocol = (obd_protocol_handle)pvParameters;
  while (1) {
   BaseType_t mux_ret = xSemaphoreTake(mux,pdMS_TO_TICKS(200));
   if (mux_ret != pdPASS)
   {
    printf("take mux fail\n");
    continue;
   }
   speed = obd_get_engine_speed_val(my_protocol);
   mux_ret =  xSemaphoreGive(mux);
   if (mux_ret != pdPASS)
   {
    printf("give mux fail\n");
   }
    printf("speed is %d\n",speed);
    vTaskDelay(pdMS_TO_TICKS(1000)); // 延时 1500 毫秒
  }
}

//task2  radio_communicate
void radio_communicate(void *pvParameters){
    
    uint8_t rec_seq[100] = {0};
    while (1)
    {   
         Ebyte_Receive(&my_status, rec_seq, 2000);
        //  printf("seq  is %d\n",rec_seq[0]);
        //  printf("frame 1 seq  is %d\n",frame1.seq);
        if (abs(rec_seq[0] - frame1.old_data[Frame_len-1]) > 1)
        {
            frame1.seq = rec_seq[0];
            frame1.data[Frame_len-1] = rec_seq[0];
        }
         if (frame1.seq == rec_seq[0])
         {
            //接收正确
            printf("rec right\n");
            BaseType_t task2_ret = esp_gatts_queue_rec(&my_buff);
            if (task2_ret == pdPASS){
              //  printf("task2_ret is right \n");
                data_refresh(&frame1,my_buff);
                Ebyte_Send( &my_status, frame1.data, Frame_len, 0 );
                seq_refresh(&frame1);
           //     printf("rec send right\n");
            }else if (task2_ret == errQUEUE_EMPTY)
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
        }else{
            //接收错误
            printf("send error\n");
             for (int i = 0; i < Frame_len; i++)
            {
                printf("%d, ",frame1.old_data[i]);
            }
            printf("\n");
            Ebyte_Send( &my_status, frame1.old_data, Frame_len, 0 );
        }
        ets_delay_us(1000);
    }
}

void app_main(void)
{   
    
    //配置参数
    esp_err_t ret;
   // BaseType_t com_ret;
    Ebyte_FIFO_t my_fifo;
    my_que = xQueueCreate(100,sizeof(uint16_t));

    mux = xSemaphoreCreateMutex();
    set_queue(my_que);

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

        //radio

    frame1.data[Frame_len-1] = 1;



    //blue init
    ret = blue_init();
    if (ret != ESP_OK)
    {
        printf("init fail\n");
        return ;
    }

    //odb init
    TaskHandle_t speed_task;
    obd_protocol_handle protocol_status = obd_create(TX_GPIO_NUM,RX_GPIO_NUM);
    esp_gatts_set_obd(protocol_status);
    

    //fifo  ebyte init
    fifo_init(&my_fifo);
    Ebyte_Init( my_ebyte_config, &my_status );

    printf("init done\n");

    // task
    xTaskCreate(get_speed,"task_speed",4096,protocol_status,tskIDLE_PRIORITY+1,&speed_task);
    xTaskCreate(radio_communicate,"task_radio",4096,NULL,tskIDLE_PRIORITY+1,NULL);


    while(1){

    }

    Ebyte_DeInit( &my_status );
}


