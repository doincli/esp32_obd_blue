#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/spi_master.h"
#include "ebyte_core.h"
#include "esp32c3/rom/ets_sys.h"

typedef void* ebyte_handle_t;

ebyte_handle_t Ebyte_Init( ebyte_config_t ebyte_config );
int Ebyte_Send( ebyte_handle_t handle, uint8_t *payload, uint8_t size, uint32_t timeout_ms );
int Ebyte_Receive( ebyte_handle_t handle, uint8_t *payload, uint32_t timeout_ms );
void Ebyte_DeInit( ebyte_handle_t handle );
int8_t Ebyte_getRSSI(ebyte_handle_t handle);
char Ebyte_GetLoraPacketStatus(ebyte_handle_t handle );