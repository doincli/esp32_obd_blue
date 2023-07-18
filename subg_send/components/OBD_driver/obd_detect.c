// Copyright 2015-2019 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "obd_detect.h"

static const twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();
static const char* TAG = "obd_detect";

obd_protocol_handle obd_create(uint8_t tx_port,uint8_t rx_port)
{
    //malloc memory
    obd_protocol_handle obd_handle = malloc(sizeof(obd_protocol_handle));
    obd_handle->io_port =  malloc(sizeof(obd_io));
    obd_handle->g_config = malloc(sizeof(twai_general_config_t));
    obd_handle->t_config = malloc(sizeof(twai_timing_config_t));

    //init struct
    obd_handle->io_port->rx_port = rx_port;
    obd_handle->io_port->tx_port = tx_port;
    obd_handle->protocol_t = ISO15765_11bit;
    obd_handle->speed = BPS_500K;
    obd_handle->statu = ISO15765_11bit_500K;
    *obd_handle->g_config = (twai_general_config_t)TWAI_GENERAL_CONFIG_DEFAULT(tx_port, rx_port, TWAI_MODE_NORMAL);
    *obd_handle->t_config = (twai_timing_config_t)TWAI_TIMING_CONFIG_500KBITS();

    //open bus
    if (obd_handle->speed == BPS_500K){
        *obd_handle->t_config = (twai_timing_config_t)TWAI_TIMING_CONFIG_500KBITS();
    }else if (obd_handle->speed == BPS_250K){
        *obd_handle->t_config = (twai_timing_config_t)TWAI_TIMING_CONFIG_250KBITS();
    }
        
    ESP_ERROR_CHECK(twai_driver_install(obd_handle->g_config, obd_handle->t_config, &f_config));

    ESP_ERROR_CHECK(twai_start());    

    //match protocol
    obd_detect_match(obd_handle);

    return obd_handle; 
}

uint8_t obd_get_engine_speed_val(obd_protocol_handle obd_handle)
{
    uint8_t data_len_rel;
    uint8_t engine_speed = 0;
    
    twai_message_t tx_msg = {
        .flags = TWAI_MSG_FLAG_NONE, 
        .identifier = MSG_ID, 
        .data_length_code = 8, 
        .data = {0x02, 0x01, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00},
        };
   
    if (obd_handle->protocol_t == ISO15765_29bit){
        tx_msg.flags = TWAI_MSG_FLAG_EXTD;
        tx_msg.identifier = MSG_ID_EXP;
    }
     
    twai_message_t rx_msg;

    twai_transmit(&tx_msg, pdMS_TO_TICKS(1000));

    twai_receive(&rx_msg, pdMS_TO_TICKS(1000));

    if (obd_handle->protocol_t == ISO15765_11bit){
        // The data frame id returned by the OBD simulator is 0x7e8
        if (rx_msg.identifier != 0x7e8){
            ESP_LOGI(TAG,"Get CAN frame id error!!\n");
            return -1;
        }
    }else{
        if (rx_msg.identifier != 0x18daf110){
            ESP_LOGI(TAG,"Get CAN frame id error!!\n");
            return -1;
        }
    }

    // data[0] represents the effective number of bytes in the next 7 data bytes
    data_len_rel = rx_msg.data[0];
    if (data_len_rel < 2 || data_len_rel > 7)  {
        ESP_LOGI(TAG,"Get data rel len error!!\n");
        return -1;
    }

    // receive data[1] is send data[1] + 0x40    receive data[2] equal send data[2]
    if (rx_msg.data[1] != tx_msg.data[1] + 0x40 || rx_msg.data[2] != tx_msg.data[2]) {
        ESP_LOGI(TAG,"Get data return message error!!\n");
        return -1;
    }

    for (int i = 3; i < data_len_rel+1; i++){
        engine_speed = engine_speed * 16 + rx_msg.data[i];
    }
    
    return engine_speed;
}

esp_err_t obd_delete(obd_protocol_handle obd_handle)
{
    obd_twai_deinit();
    free(obd_handle->g_config);
    free(obd_handle->t_config);
    free(obd_handle->io_port);
    free(obd_handle);
    obd_handle = NULL;
    return ESP_OK;
}

esp_err_t obd_twai_deinit(void)
{
    ESP_ERROR_CHECK(twai_stop());
    ESP_ERROR_CHECK(twai_driver_uninstall());
    return ESP_OK;
}

esp_err_t obd_twai_modifed(obd_protocol_handle obd_handle)
{
    if (obd_handle->speed == BPS_500K){
        *obd_handle->t_config = (twai_timing_config_t)TWAI_TIMING_CONFIG_500KBITS();
    }else if (obd_handle->speed == BPS_250K){
        *obd_handle->t_config = (twai_timing_config_t)TWAI_TIMING_CONFIG_250KBITS();
    }
        
    ESP_ERROR_CHECK(twai_driver_install(obd_handle->g_config, obd_handle->t_config, &f_config));

    ESP_ERROR_CHECK(twai_start());    
    return ESP_OK;
}

esp_err_t obd_detect(obd_protocol_handle obd_handle)
{  
    twai_message_t tx_msg ={.flags = TWAI_MSG_FLAG_NONE, .identifier = MSG_ID, .data_length_code = 8, .data = {0x02, 0x01, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00}};
   
    if (obd_handle->protocol_t == ISO15765_29bit){
        tx_msg.flags = TWAI_MSG_FLAG_EXTD;
        tx_msg.identifier = MSG_ID_EXP;
    }
    twai_message_t rx_msg;

    twai_transmit(&tx_msg, pdMS_TO_TICKS(1000));

    int flag_rec = twai_receive(&rx_msg, pdMS_TO_TICKS(1000));

    if (flag_rec == ESP_ERR_TIMEOUT ){
        ESP_LOGE(TAG,"protocol error!!\n");
        return ESP_FAIL;
    }
    
    if (obd_handle->protocol_t == ISO15765_11bit){
        if (rx_msg.identifier != 0x7e8){
            ESP_LOGI(TAG,"Get CAN frame id error!!\n");
            return ESP_FAIL;
        }
    }else {
        if (rx_msg.identifier != 0x18daf110) {
            ESP_LOGI(TAG,"Get CAN frame id error!!\n");
            return ESP_FAIL;
        }
    }
    return ESP_OK;
}

esp_err_t obd_detect_match(obd_protocol_handle obd_handle)
{
    esp_err_t  right_protocol = obd_detect(obd_handle);
    if (right_protocol == ESP_FAIL){
        obd_twai_deinit();
        while (1){   
            uint8_t protocol_cur = obd_handle->statu;
            //State machine  polling detection
            switch (protocol_cur){
            case ISO15765_11bit_500K:
                ESP_LOGI(TAG,"ISO15765_11bit_500K start\n");
                obd_twai_modifed(obd_handle);
                //Obtain the vehicle speed, if the acquisition fails, enter the next protocol to continue detection
                right_protocol = obd_detect(obd_handle);
                if (right_protocol == ESP_FAIL) {
                    obd_handle->statu = ISO15765_11bit_250K;
                    obd_handle->protocol_t = ISO15765_11bit;
                    obd_handle->speed = BPS_250K;
                    ESP_LOGI(TAG,"next protocol is ISO15765_11bit_250K\n");
                    vTaskDelay(100);
                    obd_twai_deinit();
                    // 
                }else{
                    ESP_LOGI(TAG,"right protocol is ISO15765_11bit_500K\n");
                    return ESP_OK;
                }
                
            break;

            case ISO15765_11bit_250K:
                ESP_LOGI(TAG,"ISO15765_11bit_250K start\n");
                
                obd_twai_modifed(obd_handle);
                right_protocol = obd_detect(obd_handle);
                if (right_protocol == ESP_FAIL){
                    obd_handle->statu = ISO15765_29bit_500K;
                    obd_handle->protocol_t = ISO15765_29bit;
                    obd_handle->speed = BPS_500K;
                    ESP_LOGI(TAG,"next protocol is ISO15765_29bit_500K\n");
                    obd_twai_deinit();
                }else{
                    ESP_LOGI(TAG,"right protocol is ISO15765_11bit_250K\n");
                    return ESP_OK;
                }
                
            break;

            case ISO15765_29bit_500K:
                ESP_LOGI(TAG,"ISO15765_29bit_500K start\n");
                obd_twai_modifed(obd_handle);
                right_protocol = obd_detect(obd_handle);
                if (right_protocol == ESP_FAIL){
                    obd_handle->statu = ISO15765_29bit_250K;
                    obd_handle->protocol_t = ISO15765_29bit;
                    obd_handle->speed = BPS_250K;
                    ESP_LOGI(TAG,"next protocol is ISO15765_29bit_250K\n");
                    obd_twai_deinit();
                }else{
                    ESP_LOGI(TAG,"right protocol is ISO15765_29bit_500K\n");
                    return ESP_OK;
                }
                
            break;

            case ISO15765_29bit_250K:
                ESP_LOGI(TAG,"ISO15765_29bit_250K start\n");
                obd_twai_modifed(obd_handle);
                right_protocol = obd_detect(obd_handle);
                if (right_protocol == ESP_FAIL) {
                    obd_handle->statu = ISO15765_11bit_500K;
                    obd_handle->protocol_t = ISO15765_11bit;
                    obd_handle->speed = BPS_500K;
                    ESP_LOGI(TAG,"next protocol is ISO15765_11bit_500K\n");
                    obd_twai_deinit();
                }else{
                    ESP_LOGI(TAG,"right protocol is ISO15765_29bit_250K\n");
                    return ESP_OK;
                }
            break;

            default:
                ESP_LOGI(TAG,"event error\n");
                break;
            }
        }   
    }
    return ESP_OK;
}

