#include "app.h"
uint8_t old_seq = 0;
uint8_t rec_seq[100] = {0};
ebyte_handle_t my_ebyte;
static const char* TAG = "subg";

ebyte_config_t my_ebyte_config = {
    .spi_id = EBYTE_HOST,
    .radio_mode = RADIO_MODE_LORA,
    .spi_speed_hz = 5 * 1000 * 1000,
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

void app_subg_init()
{
    Ebyte_FIFO_t my_fifo;
    fifo_init(&my_fifo);
    my_ebyte = Ebyte_Init(my_ebyte_config);
}

void app_subg_send_and_recv(uint32_t ticks_to_wait,uint16_t data,uint8_t retry)
{
    uint8_t send_buf[3] = {0};
    uint8_t seq = rand() % 256;
    while (seq == old_seq)
    {
       seq = rand() % 256;     
       ESP_LOGI(TAG, "seq is %d\n",seq);
    }
    old_seq = seq;
    for (int i = 0; i < Frame_len - 1; i++)
    {
        send_buf[Frame_len-i - 2] = (uint8_t) data;
        data = (data>>8);
    }
    send_buf[Frame_len-1] = seq;

    int times = 0;
    int size1 = Ebyte_Send( my_ebyte, send_buf, Frame_len, ticks_to_wait );
    ESP_LOGI(TAG, "send size is %d,seq is %d\n",size1,seq);
    while (1)
    {   
       int size = Ebyte_Receive(my_ebyte, rec_seq, ticks_to_wait);
        ESP_LOGI(TAG, "rec seq is %d,size is %d\n",rec_seq[0],size);
        if (rec_seq[0] == seq)
        {
            ESP_LOGI(TAG, "rec right\n");
            return;
        }else
        {
            ESP_LOGE(TAG, "rec error\n");
            Ebyte_Send( my_ebyte, send_buf, Frame_len, 0 );
            times++;
            if (times == retry)
            {
                ESP_LOGE(TAG, "retry error\n");
                return;
            }
        }
    }   
}

void Ebyte_FIFO_Init(Ebyte_FIFO_t* queue)
{
    queue->front = -1;
    queue->rear = -1;
    queue->size = 0;
}

void fifo_init( Ebyte_FIFO_t *my_fifo)
{
    for(int i = 0; i < 2; ++i){
        ESP_LOGI(TAG, "start: %d\n", i);
        ets_delay_us(1000);
    }

    Ebyte_FIFO_Init(my_fifo);
}

ebyte_config_t* get_ebyte_config()
{
    return &my_ebyte_config;
}


