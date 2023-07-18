
#ifndef APP_H
#define APP_H

#include "esp_err.h"
#include "gatts.h"
#include "obd2.h"
#include "QMA.h"

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
  * @brief  init bluetooth and create a task to transmit the common of app to the other subg
  *  
  * @return
  *     - ESP_OK: succeed
  *     - others: fail
  */
esp_err_t app_ble_init();

/**
  * @brief  get the speed of obd
  *  
  * @return speed 
  */
uint8_t app_obd_get_speed();

/**
  * @brief  init obd detect and creat a task to get speed of the car 
  */
void app_obd_init();

/**
  * @brief  send and recevice the data within subg
  * 
  * @param  ticks_to_wait  overtime time
  * @param  data      the data need to send
  * @param  retry      the number of retransmissions
  */
void app_subg_send_and_recv(uint32_t ticks_to_wait,uint16_t data,uint8_t retry);

/**
  * @brief  init subg
  */
void app_subg_init();

void master_init();

float get_QAM_x_acc();
#endif