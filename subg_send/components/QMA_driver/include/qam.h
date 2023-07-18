#ifndef QMA_H
#define QMA_H
#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "sdkconfig.h"
#include <string.h>

#define I2C_MASTER_SCL_IO 8                                   /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO 2                                   /*!< gpio number for I2C master data  */
#define I2C_MASTER_NUM 0                                      /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ 100000                             /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE 0                           /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0                           /*!< I2C master doesn't need buffer */

#define QMA7981_ADDR  0x12
#define QMA7981_DEVICE_ADDR           0x00 
#define QMA7981_MAX_VALUE 0x3FFF							   /*!< QMA7981 range */

#define QMA7981_DXM_ADDR 0x01								/*!< QMA7981 register X-axis acceleration address */
#define QMA7981_DYM_ADDR 0x03								/*!< QMA7981 register Y-axis acceleration address */
#define QMA7981_DZM_ADDR 0x05								/*!< QMA7981 register Z-axis acceleration address */
#define QMA7981_MODE_ADDR         0x11                      /*!< Register addresses of mode register */
#define QMA7981_RANGE_ADDR      0x10                        /*!< Register addresses of range register */    

#define QMA7981_MODE_100K_cmd 0x83					 /*!< The command to set QMA7981 to active mode 100k */    
#define QMA7981_MODE_500K_cmd 0x80					
#define QMA7981_RAG_2g_cmd  0x01                     /*!< The command to set QMA7981 torange 2g */   
#define QMA7981_RAG_4g_cmd   0x02
#define QMA7981_RAG_8g_cmd   0x04
#define QMA7981_RAG_16g_cmd   0x08
#define QMA7981_RAG_32g_cmd   0x0f

/**
* @brief init i2c master
*
* @return
*     - ESP_OK   Success
*     - ESP_FAIL Error
*/
esp_err_t qma7981_master_init(void);

/**
* @brief read data from the register of QMA7981
*
* @param reg_addr  the address of the register of QMA7981
* @param data      the point of buffer
* @param len       the length of buffer
*
* @return
*     - ESP_OK   Success
*     - ESP_FAIL Error
*/
esp_err_t qma7981_register_read(uint8_t reg_addr, uint8_t *data, size_t len);

/**
* @brief write cmd to the register of QMA7981
*
* @param reg_addr  the address of the register of QMA7981
* @param cmd       the commond 
*
* @return
*     - ESP_OK   Success
*     - ESP_FAIL Error
*/
esp_err_t qma7981_register_write_byte(uint8_t reg_addr, uint8_t cmd);

/**
* @brief set the QMA7981 speed and activity
*
* @param cmd       the commond 
*
* @return
*     - ESP_OK   Success
*     - ESP_FAIL Error
*/
esp_err_t qma7981_setMode(uint8_t cmd);

/**
* @brief set the QMA7981 accuracy
*
* @param cmd       the commond 
*
* @return
*     - ESP_OK   Success
*     - ESP_FAIL Error
*/
esp_err_t qma7981_setAcc(uint8_t cmd);

/**
* @brief read the acceleration of X axis
*
* @return
*     - the acceleration of X axis
*/
float qma7981_read_DXM();

/**
* @brief read the acceleration of Y axis
*
* @return
*     - the acceleration of Y axis
*/
float qma7981_read_DYM();

/**
* @brief read the acceleration of Z axis
*
* @return
*     - the acceleration of Z axis
*/
float qma7981_read_DZM();
#endif