#ifndef SELF_TEST_H
#define SELF_TEST_H

#include "esp_err.h"
#include "obd_detect.h"
#define SELF_TEST  1
//#include "ebyte.h"
/**
  * @brief  test accuracy is work normal
  *  
  * @return
  *     - ESP_OK: succeed
  *     - others: fail
  */
esp_err_t accuracy_test();

/**
  * @brief  test can bus is work normal
  *  
  * @return
  *     - ESP_OK: succeed
  *     - others: fail
  */
esp_err_t can_test();

/**
  * @brief  test spi bus is work normal
  *  
  * @return
  *     - ESP_OK: succeed
  *     - others: fail
  */
esp_err_t spi_test();

/**
  * @brief  test driver is work normal
  */
esp_err_t self_test();

/**
  * @brief  test  init
  */
void self_test_init();

/**
  * @brief  gpio init
  */
void gpio_int();

/**
  * @brief  test blue is work normal
  *  
  * @return
  *     - ESP_OK: succeed
  *     - others: fail
  */
esp_err_t blue_test();

#endif // SELF_TEST_H