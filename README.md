# 目前的解决方法

## 主要思路

1. 可以实例化2个 IO口，目前只用到1个
2. 实例化一个can总线结构体 包括协议和速度  初始化
3. 打开总线
4. 自动搜索协议，并且保存在结构体中
5. 获取速度



```c
void app_main(void)
{   
    //实例化io口 ，可以有两个
    config_io(RX_GPIO_NUM,TX_GPIO_NUM,FIRST);

    //实例化一个can总线结构体 包括协议和速度
    detect_config_t t_config;

    //初始化结构体
    config_detect(&t_config);

    //打开总线
    OBD_twai_init(&t_config,FIRST);
    
    //自动搜索协议，并且保存在结构体中
	Task_detectpro(&t_config);
  
    //获取速度
    for (int i = 0; i < 10; i++)
    {
        uint32_t speed = OBD_get_engine_speed_val_protocol(&t_config);
        printf("speed is %ld\n",speed);
    }
    
    OBD_twai_deinit();
}
```



## 具体方案

### IO口思路

写两个全局变量，然后可以在主函数中利用函数调用，修改相关配置

```c
static const twai_timing_config_t t_config_250 = TWAI_TIMING_CONFIG_250KBITS();
static const twai_timing_config_t t_config_500 = TWAI_TIMING_CONFIG_500KBITS();
static const twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

//gpio 
#define TX_GPIO 6
#define RX_GPIO 8

extern 

//配置IO
twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(TX_GPIO, RX_GPIO, TWAI_MODE_NORMAL);

twai_general_config_t g_config1 = TWAI_GENERAL_CONFIG_DEFAULT(TX_GPIO, RX_GPIO, TWAI_MODE_NORMAL);

//函数调用
void config_io(int rx_port,int tx_port,int can_num){
    if (can_num == FIRST)
    {
        g_config.tx_io = tx_port;
        g_config.rx_io = rx_port;
    }else
    {
        g_config1.tx_io = tx_port;
        g_config1.rx_io = rx_port;
    }
}
```



#### 问题：

目前只能开两组IO口，如果想要开任意的，要怎么解决



### CAN协议结构体思路

结构体包含三个变量，分别是目前的协议，目前的搜索状态，目前的速度



```c
typedef enum 
{
    ISO15765_11bit_500K=1, 
    ISO15765_11bit_250K, 
    ISO15765_29bit_500K, 
    ISO15765_29bit_250K
}protocol;



typedef struct 
{
    uint8_t    protocol_t;
    uint8_t    statu ; //对应上面的结构体，表明现在的搜索状态
    uint8_t    speed ;
    //uint32_t   msg_id;   //11 bit standard format ID
}detect_config_t;


//初始化结构体
void config_detect(detect_config_t *t_config){
    t_config->protocol_t = ISO15765_11bit;
    t_config->speed = BPS_500K;
    t_config->statu = ISO15765_11bit_500K;
}
```



初始化可以随便初始化，因为后面会自动搜索相关协议，并且写回





### 打开总线

选择不同的协议和IO口来打开相应的总线

```c
void OBD_twai_init(detect_config_t  *t_config,int can_num)
{   
    if (t_config->speed == BPS_250K )
    {
        if (can_num == FIRST)
        {
            ESP_ERROR_CHECK(twai_driver_install(&g_config, &t_config_250, &f_config));
        }else{
            ESP_ERROR_CHECK(twai_driver_install(&g_config1, &t_config_250, &f_config));
        }      
    }else
    {
        if (can_num == FIRST)
        {
            ESP_ERROR_CHECK(twai_driver_install(&g_config, &t_config_500, &f_config));
        }else{
            ESP_ERROR_CHECK(twai_driver_install(&g_config1, &t_config_500, &f_config));
        }  
    }
    ESP_ERROR_CHECK(twai_start());
}

```



#### 问题：

1. 协议只有两种，总线IO也只有两个 还可以写，如果特别多该怎么写？



### 自动检查协议并且写回

#### 检查协议对错

```c
//检测这个协议情况是否正确
bool detect_get_protocol(detect_config_t *t_config){
    // uint8_t data_len_rel;
    // uint32_t engine_speed = 0;  
   
    twai_message_t tx_msg ={.flags = TWAI_MSG_FLAG_NONE, .identifier = MSG_ID, .data_length_code = 8, .data = {0x02, 0x01, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00}};
   
    if (t_config->protocol_t == ISO15765_29bit)
    {
        tx_msg.flags = TWAI_MSG_FLAG_EXTD;
        tx_msg.identifier = MSG_ID_EXP;
    }
    twai_message_t rx_msg;

     int tra = twai_transmit(&tx_msg, pdMS_TO_TICKS(1000));
    printf("tra =  %d\n",tra);

    int flag_rec = twai_receive(&rx_msg, pdMS_TO_TICKS(1000));

    printf("flag_rec =  %d\n",flag_rec);
     if (flag_rec == ESP_ERR_TIMEOUT )
    {
        printf("protocol error!!\n");
        return false;
    }
    
    if (t_config->protocol_t == ISO15765_11bit)
    {
        // OBD模拟器回复的数据帧id为0x7e8
        if (rx_msg.identifier != 0x7e8)
        {
            printf("Get CAN frame id error!!\n");
           // t_config.protocol_t = ISO15765_29bit;
            return false;
        }
    }else
    {
         if (rx_msg.identifier != 0x18daf110)
        {
            printf("Get CAN frame id error!!\n");
           // t_config.protocol_t = ISO15765_11bit;
            return false;
        }
    }
    return true;

}

```



#### 轮询查找正确的协议

这块后面打算使用多线程来实现，搞一个条件变量，匹配好了开始读数据

目前是测试状态

```c
//自动匹配正确的协议以及速率
void Task_detectpro(detect_config_t *t_config){
    int  i = 0;
    bool  right_protocol = detect_get_protocol(t_config);
    if (!right_protocol){
        OBD_twai_deinit();
        while (1)
        {   

            uint8_t protocol_cur = t_config->statu;
            printf("protocol_cur %d\n",protocol_cur);
            //状态机实现轮询探测
            switch (protocol_cur)
            {
            case ISO15765_11bit_500K:
                printf("ISO15765_11bit_500K start\n");
                //配置这个协议得TWAI总线
                OBD_twai_init(t_config,FIRST);
                //获取车速，获取失败则进入下一个协议继续探测，剩下同理
                    right_protocol = detect_get_protocol(t_config);
                if (!right_protocol)
                {
                    t_config->statu = ISO15765_11bit_250K;
                    t_config->protocol_t = ISO15765_11bit;
                    t_config->speed = BPS_250K;
                    printf("next protocol is ISO15765_11bit_250K\n");
                    OBD_twai_deinit();
                    // 
                }else{
                    printf("right protocol is ISO15765_11bit_500K\n");
                    OBD_twai_deinit();
                }
                
            break;

            case ISO15765_11bit_250K:
                printf("ISO15765_11bit_250K start\n");
                
                OBD_twai_init(t_config,FIRST);
                right_protocol = detect_get_protocol(t_config);
                if (!right_protocol)
                {
                    t_config->statu = ISO15765_29bit_500K;
                    t_config->protocol_t = ISO15765_11bit;
                    t_config->speed = BPS_500K;
                    printf("next protocol is ISO15765_29bit_500K\n");
                    OBD_twai_deinit();
                }else{
                    printf("right protocol is ISO15765_11bit_250K\n");
                    OBD_twai_deinit();
                }
                
            break;

            case ISO15765_29bit_500K:
                printf("ISO15765_29bit_500K start\n");
                //t_config.speed_config = (twai_timing_config_t)TWAI_TIMING_CONFIG_250KBITS();
                OBD_twai_init(t_config,FIRST);
                right_protocol = detect_get_protocol(t_config);
                if (!right_protocol)
                {
                    t_config->statu = ISO15765_29bit_250K;
                    t_config->protocol_t = ISO15765_29bit;
                    t_config->speed = BPS_250K;
                    printf("next protocol is ISO15765_29bit_250K\n");
                    OBD_twai_deinit();
                }else{
                    printf("right protocol is ISO15765_29bit_500K\n");
                    OBD_twai_deinit();
                }
                
            break;

            case ISO15765_29bit_250K:
                printf("ISO15765_29bit_250K start\n");
                //t_config.speed_config = (twai_timing_config_t)TWAI_TIMING_CONFIG_500KBITS();
                OBD_twai_init(t_config,FIRST);
                right_protocol = detect_get_protocol(t_config);
                if (!right_protocol)
                {
                    t_config->statu = ISO15765_11bit_500K;
                    t_config->protocol_t = ISO15765_29bit;
                    t_config->speed = BPS_500K;
                    printf("next protocol is ISO15765_11bit_500K\n");
                    OBD_twai_deinit();
                }else{
                    printf("right protocol is ISO15765_29bit_250K\n");
                    OBD_twai_deinit();
                }
                // OBD_twai_deinit();
            break;

            default:
                printf("event error\n");
                break;
            }

        //test
            i++;
            if (i >2)
            {
                OBD_twai_init(t_config,FIRST);
                break;
            }
            
            vTaskDelay(200);
        }   
    }
}

```



#### 获得车速

没啥可说的，很简单

```c
//得到车速的值
uint32_t OBD_get_engine_speed_val_protocol(detect_config_t *t_config)
{
    uint8_t data_len_rel;
    uint32_t engine_speed = 0;
    

    twai_message_t tx_msg ={.flags = TWAI_MSG_FLAG_NONE, .identifier = MSG_ID, .data_length_code = 8, .data = {0x02, 0x01, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00}};
   
    if (t_config->protocol_t == ISO15765_29bit)
    {
        tx_msg.flags = TWAI_MSG_FLAG_EXTD;
        tx_msg.identifier = MSG_ID_EXP;
    }
    
    
    // twai_message_t tx_msg = {.flags = TWAI_MSG_FLAG_NONE, .identifier = MSG_ID, .data_length_code = 8, .data = {0x02, 0x01, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00}};
    twai_message_t rx_msg;

    twai_transmit(&tx_msg, pdMS_TO_TICKS(1000));

    twai_receive(&rx_msg, pdMS_TO_TICKS(1000));
    //printf("t_config->protocol_t is %d \n",t_config->protocol_t);
    if (t_config->protocol_t == ISO15765_11bit)
    {
        // OBD模拟器回复的数据帧id为0x7e8
        if (rx_msg.identifier != 0x7e8)
        {
            printf("Get CAN frame id error!!\n");
           // t_config.protocol_t = ISO15765_29bit;
            return -1;
        }
    }else
    {
         if (rx_msg.identifier != 0x18daf110)
        {
            printf("Get CAN frame id error!!\n");
           // t_config.protocol_t = ISO15765_11bit;
            return -1;
        }
    }

    // data[0]代表接下来7个数据字节有效的字节数
    data_len_rel = rx_msg.data[0];
    if (data_len_rel < 2 || data_len_rel > 7)
    {
        printf("Get data rel len error!!\n");
        return -1;
    }

    // receive data[1]为send data[1] + 0x40    receive data[2]等于send data[2]
    if (rx_msg.data[1] != tx_msg.data[1] + 0x40 || rx_msg.data[2] != tx_msg.data[2])
    {
        printf("Get data return message error!!\n");
        return -1;
    }

    
    for (int i = 3; i < data_len_rel+1; i++)
    {
        engine_speed = engine_speed*16 + rx_msg.data[i];
    }
    

    return engine_speed ;
}

```





## 总问题：

1. 目前的方案都是基于IO口 协议数比较少的情况，不具有通用性，明天讨论一下
2. 条件变量和多线程怎么加
3. 多线程情况，匹配协议和传输数据同时进行，总线占用情况
4. 按照代码规范修改代码
