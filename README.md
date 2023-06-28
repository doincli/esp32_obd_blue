# OBD_detect组件说明

## 公共参数说明：

### 定义的参数

protocol_t表示协议到种类，有两种 ISO15765_11bit 和 ISO15765_29bit

speed  表示CAN总线的速度  有两种BPS_500K和BPS_250K

MSG_ID 根据协议不同有两种  MSG_ID和MSG_ID_EXP

```c
#define ISO15765_11bit 1        /*!< protocol_t   */
#define ISO15765_29bit 0

#define BPS_500K   1            /*!< speed   */
#define BPS_250K   0

#define MSG_ID 0x7DF            /*!< MSG_ID the head of message of protocol   */
#define MSG_ID_EXP 0x18DB33F1 
```



### 枚举参数

表示不同的四种情况，分别对应不同的轮询搜索状态

```c
typedef enum 
{
    ISO15765_11bit_500K=0,       /*!< Show current search mode is ISO15765_11bit,speed is 500KB */
    ISO15765_11bit_250K,         /*!< Show current search mode is ISO15765_11bit,speed is 250KB */
    ISO15765_29bit_500K,         /*!< Show current search mode is ISO15765_29bit,speed is 500KB */
    ISO15765_29bit_250K          /*!< Show current search mode is ISO15765_29bit,speed is 250KB */
}protocol;
```



### 结构体参数

有一个IO结构体，包含两个传输的引脚

还有一个detect_config_t 包含协议种类，轮询状态，CAN总线速度，以及相关的TWAI总线配置

```c
typedef struct 
{
    uint8_t  tx_port;            /*!< IO port */
    uint8_t  rx_port;
}obd_io;

typedef obd_io * obd_io_handle;

typedef struct 
{
    uint8_t    protocol_t;          /*!< protocol can be defined as  ISO15765_11bit and ISO15765_29bit */
    uint8_t    statu ;              /*!< current search mode */
    uint8_t    speed ;              /*!< speed can be defined as  BPS_500K and BPS_250K */
    obd_io_handle io_port;
    twai_general_config_t * g_config;
    twai_timing_config_t *  t_config;
}detect_config_t;

typedef detect_config_t* obd_protocol_handle;
```



## 函数说明：

1.表示创造一个can总线句柄由于接下来的OBD检测操作,OBD初始化，绑定输出引脚以及基础的通信协议初始化

```c
obd_protocol_handle obd_create(uint8_t TX_GPIO_NUM,uint8_t RX_GPIO_NUM);
```

3.获取OBD模拟器的车速

```c
uint32_t obd_get_engine_speed_val(obd_protocol_handle protocol_status );
```

4.删除can总线句柄

```c
esp_err_t obd_delete(obd_protocol_handle obd_handle);
```

5.关闭CAN总线 （本组件内部调用）

```c
esp_err_t obd_twai_deinit();
```

6.更改CAN总线配置（本组件内部调用）

```c
esp_err_t obd_twai_modifed(obd_protocol_handle obd_handle);
```

7.判断当前协议是否正确（本组件内部调用）

```c
esp_err_t obd_detect(obd_protocol_handle obd_handle);
```

8.自动匹配到正确的协议（本组件内部调用）

```c
esp_err_t obd_detect_match(obd_protocol_handle obd_handle);
```



## 测试流程：

总体分为4步

1. obd_create 创造一个操作句柄
2. obd_get_engine_speed_val 获得车速信息
3. obd_delete  删除句柄

```c
#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "OBD_detect.h"


#define TX_GPIO_NUM 2
#define RX_GPIO_NUM 3

void app_main(void)
{   
    obd_protocol_handle protocol_status = obd_create(TX_GPIO_NUM,RX_GPIO_NUM);

    for (int i = 0; i < 10; i++)
    {
        uint32_t speed = obd_get_engine_speed_val(protocol_status);
        printf("speed is %ld\n",speed);
    }
    
    obd_delete(protocol_status);
}
```



## 
