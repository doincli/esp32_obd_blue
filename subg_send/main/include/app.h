
#ifndef APP_H
#define APP_H

#include "esp_err.h"
#include "gatts.h"
#include "obd2.h"
#include "app_qam.h"
#include "stdio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "rfm300_driver.h"
#include "cmt2300_regs.h"
#include "esp_log.h"

// #define SUBG_CMT2300 1
//ebyte的结构体
typedef struct {
    uint8_t data[EBYTE_MAX_SIZE];
    int front;
    int rear;
    int size;
} Ebyte_FIFO_t;

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
int app_obd_get_speed();

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

/**
  * @brief  get subg_hanle
  */
ebyte_handle_t get_ebyte();

/**
  * @brief  get rssi value
  * @return the value of rssi
  */
int8_t get_rssi();

/**
  * @brief   cmt2300 init
  */
void app_subg_cmt2300_init();

/**
  * @brief  send and recevice the data within subg cmt2300
  * 
  * @param  ticks_to_wait  overtime time
  * @param  data      the data need to send
  * @param  retry      the number of retransmissions
  */
void app_subg_cmt2300_send_and_recv(uint32_t ticks_to_wait, uint16_t data, uint8_t retry);

/**
  * @brief   cmt2300 tx test
  */
void app_subg_cmt2300_tx_test();

/**
  * @brief   cmt2300 rx test
  */
void app_subg_cmt2300_rx_test(void* arg);

/**
  * @brief   get subg point
  */
void * get_subg_handle();

#endif