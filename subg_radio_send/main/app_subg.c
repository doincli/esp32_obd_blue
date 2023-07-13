#include "app.h"

uint8_t rec_seq[100] = {0};
ebyte_handle_t my_ebyte;
void app_subg_init(){

    Ebyte_FIFO_t my_fifo;
    ebyte_config_t* my_ebyte_config = get_ebyte_config();
    fifo_init(&my_fifo);
    my_ebyte = Ebyte_Init(*my_ebyte_config);
}

void app_subg_send_and_recv(uint32_t ticks_to_wait,uint16_t data,uint8_t retry)
{
    uint8_t send_buf[3] = {0};
    uint8_t seq = rand() % 256;
    for (int i = 0; i < Frame_len - 1; i++)
    {
        send_buf[Frame_len-i - 2] = (uint8_t) data;
        data = (data>>8);
    }
    send_buf[Frame_len-1] = seq;
    printf("step1\n");
    int times = 0;
    Ebyte_Send( my_ebyte, send_buf, Frame_len, ticks_to_wait );
     printf("step2\n");
    while (1)
    {   
        Ebyte_Receive(my_ebyte, rec_seq, ticks_to_wait);
         printf("rec seq is %d\n",rec_seq[0]);
        if (rec_seq[0] == seq)
        {
            printf("rec right\n");
            return;
        }else
        {
            printf("rec error\n");
            Ebyte_Send( my_ebyte, send_buf, Frame_len, 0 );
            times++;
            if (times == retry)
            {
                printf("retry error\n");
                return;
            }
        }
    }   
}

