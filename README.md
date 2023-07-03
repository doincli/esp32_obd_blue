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



## 实验效果



### obd检测端

![image-20230703174006463](https://image-1302263000.cos.ap-nanjing.myqcloud.com/img/image-20230703174006463.png)

### 手机端

![image-20230703173948712](https://image-1302263000.cos.ap-nanjing.myqcloud.com/img/image-20230703173948712.png)

### 



### 远程通信的接收端

![image-20230703174026674](https://image-1302263000.cos.ap-nanjing.myqcloud.com/img/image-20230703174026674.png)





## 目前问题以及下一步规划

1. 射频发送的数据不是一定准确的，比如发送1,2,3,4.....100,接收端可能只能接到1,2,4,5...100,中间的3没有接受到，这种情况要怎么处理
2. 学习一下CMAKE，重新调整一下程序的结构