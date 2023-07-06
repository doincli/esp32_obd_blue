#include "OBD.h"

// uint16_t my_common;

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

void data_refresh(frame_handle frame,uint16_t data){
   
    for (int i = 0; i < Frame_len-1; i++)
    {
        frame->data[Frame_len-2-i] = (uint8_t)data;
        frame->old_data[i] = frame->data[i];
        data = (data >> 8);
    }

}

void seq_refresh(frame_handle frame){
    frame->old_seq = frame->seq;
    frame->old_data[Frame_len-1] = frame->old_seq;
    frame->seq++;
    frame->data[Frame_len-1] = frame->seq;
}

void fifo_init( Ebyte_FIFO_t *my_fifo){
    
    for(int i = 0; i < 2; ++i){
        printf("start: %d\n", i);
        ets_delay_us(1000);
    }

    Ebyte_FIFO_Init(my_fifo);
}

void frame_init(frame_handle frame){
    memset(frame->data,0,Frame_len-1);
    frame->seq = 1;
    memset(frame->old_data,0,Frame_len);
    frame->old_seq = 0;
    frame->data[Frame_len-1] = 1;
    // frame->old_data[Frame_len-1] = 0;
}

ebyte_config_t get_ebyte_config(){
    return my_ebyte_config;
}