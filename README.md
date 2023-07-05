# OBD + GATTS + RADIO





## 整体思路：

一共使用3块ESP32 （以下都用简称了）

第一块ESP32 称为ESP_1  用于OBD检测 通过GATTS发送给第二块ESP32 通过射频发送给第三块ESP32

第二块ESP32 称为ESP_2  用于GATTS传输，可以认为是手机终端

第三块ESP32称为ESP_3 用于射频模块  可以认为是车库等终端



ESP_1用于OBD检测，并且实时将数据发送给另一块ESP_2（可以当成手机端）

当车速大于120km/h时候，手机端的ESP_2发送一个命令 我设置的是AC11

用于检测的ESP_1收到命令后通过射频传输给ESP_3（可以当做车库等终端）。



## 相关程序

gatt_client 是ESP_2的程序

gatt_server是ESP_1的程序

subg_radio_rev是ESP_3的程序



## 相比于第一版的改进

1. 加入丢包处理的逻辑（处理逻辑在另一个md文件中）
2. 不使用全局变量，修改speed的接口函数
3. 蓝牙传回的数据在存储进入一个消息队列，解决蓝牙和射频模块速度不一致的问题，当队列为空的时候，射频模块不发数据
4. 修改相关的CMAKE，把OBD检测放在组件里面



## 实验效果



### obd检测端

![image-20230705105231846](https://image-1302263000.cos.ap-nanjing.myqcloud.com/img/image-20230705105231846.png)



### 手机端

![image-20230703173948712](https://image-1302263000.cos.ap-nanjing.myqcloud.com/img/image-20230703173948712.png)

# 



### 远程通信的接收端

远程接收不丢包了

![image-20230705105203869](https://image-1302263000.cos.ap-nanjing.myqcloud.com/img/image-20230705105203869.png)

