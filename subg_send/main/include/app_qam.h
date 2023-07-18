#ifndef  APP_QAM_H
#define APP_QAM_H

#include "qam.h"

/**
  * @brief  init i2c
  */
void app_master_init();

/**
  * @brief  get accuracy of x axis
  * 
  * @return accuracy of x axis
  */
float app_get_qam_x_acc();

#endif // APP_QAM_H
