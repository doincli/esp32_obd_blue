// Copyright 2015-2019 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#ifndef OBD_H
#define OBD_H

#include <stddef.h>
#include "esp_err.h"
#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <driver/twai.h>
#include "func.h"

//检测这个协议情况是否正确
bool detect_get_protocol(detect_config_t *t_config);

//自动匹配正确的协议
void Task_detectpro(detect_config_t *t_config);


uint32_t OBD_get_engine_speed_val_protocol(detect_config_t *t_config);





#endif // OBD_H
