#include "ebyte.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

/* !
 * @brief  init ebyte radio chip and install driver
 *
 * @param  ebyte_config   ebyte config struct
 * 
 * @return the handler of ebyte
 */
ebyte_handle_t Ebyte_Init( ebyte_config_t ebyte_config )
{
    ebyte_status_t* handle = (ebyte_status_t*) calloc(1, sizeof(ebyte_status_t));
    Ebyte_RF.Install( ebyte_config, handle );
    xSemaphoreTake( handle->xmutex, portMAX_DELAY );
    Ebyte_RF.Init( handle );
    xSemaphoreGive( handle->xmutex );
    return (ebyte_handle_t) handle;
}

/* !
 * @brief  send data with ebyte radio chip once
 *
 * @param  handle         the handler of ebyte
 * @param  payload        the data will be sent
 * @param  size           send data len
 * @param  timeout_ms     the waiting time for sending data, if it times out, a timeout interrupt will be triggered
 *                        max num is 262000 (262s)
 *                        (if set to 0, radio chip will stay in TX status until data is sent)
 * @return the data len has sent
 */
int Ebyte_Send( ebyte_handle_t handle, uint8_t *payload, uint8_t size, uint32_t timeout_ms )
{
    ebyte_status_t *ebyte_status = (ebyte_status_t *)handle;
   // printf("1.1\n");
    if( timeout_ms > 262000 )
    {
        ets_printf("Set too large timeout\n");
        return -1;
    }

    uint32_t timeout = timeout_ms * 1000 / 15;
   // printf("1.2\n");
    xSemaphoreTake( ebyte_status->xmutex, portMAX_DELAY );
  
    uint16_t irq_return = Ebyte_RF.Send( ebyte_status, payload, size, timeout );
   // printf("1.3\n");
    xSemaphoreGive( ebyte_status->xmutex );
   // printf("1.4\n");
    if((irq_return & 0x0001) ==  0x0001 ) {
        return size;
    }
    else{
        return -1;
    }
}

/* !
 * @brief  receive data with ebyte radio chip once
 *
 * @param  handle         the handler of ebyte
 * @param  payload        array of received data
 * @param  timeout_ms     the waiting time for receive data, if it times out, a timeout interrupt will be triggered
 *                        max num is 262000 (262s)
 *                        (if set to 0, radio chip will stay in RX status until data is received)
 * @return the data len has received
 */
int Ebyte_Receive( ebyte_handle_t handle, uint8_t *payload, uint32_t timeout_ms )
{
    ebyte_status_t *ebyte_status = (ebyte_status_t *)handle;
   // printf("2.1\n");
    if( timeout_ms > 262000 )
    {
        ets_printf("Set too large timeout\n");
        return -1;
    }
   // printf("2.2\n");
    uint32_t timeout = timeout_ms * 1000 / 15;
    uint16_t irq_return;

    xSemaphoreTake( ebyte_status->xmutex, portMAX_DELAY );
    // printf("2.3\n");
    Ebyte_RF.EnterReceiveMode( ebyte_status, timeout );
    // printf("2.4\n");
    do {
        irq_return = Ebyte_RF.StartPollTask(ebyte_status, payload);
        vTaskDelay( 10 );
    }
    while( !irq_return );
    // printf("2.5\n");
    xSemaphoreGive( ebyte_status->xmutex );
   //  printf("2.6\n");
    if((irq_return & 0x0002) ==  0x0002 ) {
        return ebyte_status->recv_len;
    }
    else{
        return -1;
    }
}

/* !
 * @brief  deinit ebyte radio chip and unstall driver
 *
 * @param  handle         the handler of ebyte
 */
void Ebyte_DeInit( ebyte_handle_t handle )
{
    ebyte_status_t *ebyte_status = (ebyte_status_t *)handle;

    Ebyte_RF.Unstall(ebyte_status);

    free(ebyte_status);
}
