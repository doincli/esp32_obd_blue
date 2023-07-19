#include "esp_err.h"
#include "obd2.h"
#include "qam.h"
// #include "ebyte.h"
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
void self_test();
