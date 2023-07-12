#include "app.h"
#include "obd2.h"
#include <stdlib.h>

uint8_t rec_seq[100] = {0};
ebyte_status_t my_status;
void app_subg_init(){
    Ebyte_FIFO_t my_fifo;
    ebyte_config_t my_ebyte_config = get_ebyte_config();
    frame_handle frame = get_frame();
    frame_init(frame);
    //fifo  ebyte init
    fifo_init(&my_fifo);
    
    Ebyte_Init( my_ebyte_config, &my_status );
}

ebyte_status_t app_get_status(){
    return my_status;
}

void app_subg_send_and_recv(uint32_t ticks_to_wait,uint16_t data)
{
    // printf("step 1\n");
    uint8_t send_buf[3] = {0};
    uint8_t seq = rand() % 256;
    for (int i = 0; i < Frame_len - 1; i++)
    {
        send_buf[Frame_len-i - 2] = (uint8_t) data;
        data = (data>>8);
    }
    send_buf[Frame_len-1] = seq;
    
    ebyte_status_t my_status = app_get_status();
    
    Ebyte_Send( &my_status, send_buf, Frame_len, 0 );
    while (1)
    {   
        Ebyte_Receive(&my_status, rec_seq, 2000);
         printf("rec seq is %d\n",rec_seq[0]);
        if (rec_seq[0] == seq)
        {
            printf("rec right\n");
            return;
        }else
        {
            Ebyte_Send( &my_status, send_buf, Frame_len, 0 );
        }
        
    }   
}

