#include "obd2.h"

ebyte_config_t my_ebyte_config = {
    .spi_id = EBYTE_HOST,
    .radio_mode = RADIO_MODE_LORA,
    .spi_speed_hz = 10 * 1000 * 1000,
    .miso_io = PIN_NUM_MISO,
    .mosi_io = PIN_NUM_MOSI,
    .sclk_io = PIN_NUM_CLK,
    .cs_io = PIN_NUM_CS,
    .busy_io = PIN_NUM_BUSY,
    .rst_io = PIN_NUM_RST,
    .dio1_io = -1,
    .rxen_io = -1,
    .txen_io = -1,
};

void Ebyte_FIFO_Init(Ebyte_FIFO_t* queue) {
    queue->front = -1;
    queue->rear = -1;
    queue->size = 0;
}

bool Ebyte_FIFO_IsEmpty(Ebyte_FIFO_t* queue) {
    return queue->size == 0;
}

bool Ebyte_FIFO_IsFull(Ebyte_FIFO_t* queue) {
    return queue->size == EBYTE_MAX_SIZE;
}

int Ebyte_FIFO_Write(Ebyte_FIFO_t* queue, uint8_t* pData, int len) {
    if (Ebyte_FIFO_IsEmpty(queue)) queue->front = 0;
    int count = 0;

    for(int i = 0; i < len; ++i){
        if (Ebyte_FIFO_IsFull(queue)) {
            break;
        }

        queue->rear = (queue->rear + 1) % EBYTE_MAX_SIZE;
        queue->data[queue->rear] = pData[i];
        queue->size++;
        count++;
    }
    return count;
}

int Ebyte_FIFO_Read(Ebyte_FIFO_t* queue, uint8_t* pData, int len) {
    int count = 0;

    for (int i = 0; i < len; i++) {
        if (Ebyte_FIFO_IsEmpty(queue)) {
            break;
        }

        pData[i] = queue->data[queue->front];
        queue->front = (queue->front + 1) % EBYTE_MAX_SIZE;
        queue->size--;
        count++;
    }

    return count;
}

void fifo_init( Ebyte_FIFO_t *my_fifo){
    
    for(int i = 0; i < 2; ++i){
        printf("start: %d\n", i);
        ets_delay_us(1000);
    }

    Ebyte_FIFO_Init(my_fifo);
}

ebyte_config_t* get_ebyte_config(){
    return &my_ebyte_config;
}

uint8_t speed;

uint8_t app_obd_get_speed()
{
    return speed;
}

//task1  get speed 1/s
void app_obd_speed_task(void *pvParameters) 
{
    obd_protocol_handle obd_handle = (obd_protocol_handle)pvParameters;
    SemaphoreHandle_t obd_api_mux = xSemaphoreCreateMutex();

    while (1) {
        BaseType_t mux_ret = xSemaphoreTake(obd_api_mux, pdMS_TO_TICKS(200));
        if (mux_ret != pdPASS) {
            printf("take mux fail\n");
            continue;
        }
        speed = obd_get_engine_speed_val(obd_handle);
        mux_ret =  xSemaphoreGive(obd_api_mux);
        if (mux_ret != pdPASS) {
            printf("give mux fail\n");
        }
        printf("speed is %d\n", speed);
        vTaskDelay(pdMS_TO_TICKS(1000)); // 延时 1500 毫秒
    }
}

void app_obd_init()
{
    //odb init
    TaskHandle_t speed_task;
    obd_protocol_handle obd_handle = obd_create(TX_GPIO_NUM, RX_GPIO_NUM);

    xTaskCreate(app_obd_speed_task, "app_obd_speed_task", 4096, obd_handle, 5, &speed_task);
}