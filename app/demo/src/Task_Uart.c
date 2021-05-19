#include "Task_Uart.h"

hi_bool  Uart_Receive_Flag = HI_FALSE;  // 接受标志位，FALSE 表示未接收到数据，可以read，TRUE 表示进行数据处理而不read
hi_u8    Uart_Rec_Buffer[UART_BUFF_SIZE] = {0}; // 全局定义接收存储器
hi_s32   Uart_Rec_Buff_len = 0; // 记录一次帧长度


/**  
 * @brief 创建UART控制任务
 * @param  void
 * @attention 波特率 115200 UART1
 * @retval void
**/
void Task_Uart_Create(void){

    hi_uart_attribute uart_attr = {
        .baud_rate = 115200, /* baud_rate: 115200 */
        .data_bits = 8,      /* data_bits: 8bits */
        .stop_bits = 1,
        .parity = 0,
    };
    hi_u32 temp = hi_uart_init(HI_UART_IDX_1, &uart_attr, HI_NULL); // UART2
    if (temp != HI_ERR_SUCCESS) {
        printf("Failed to init uart! Err code = %d\n", temp);
        return;
    }

    hi_task_attr uart_task_attr = {0};
    uart_task_attr.stack_size = 2048;
    uart_task_attr.task_prio = 25;
    uart_task_attr.task_name = (hi_char*)"task_uart";
    if (hi_task_create(&task_uart_id, &uart_task_attr, Task_Uart, HI_NULL) != HI_ERR_SUCCESS) {
        printf("Falied to create uart demo task!\n");
    }
}


/**  
 * @brief UART接收任务
 * @param  void
 * @attention UART_BUFF_SIZE 32
 * @retval void
**/
void *Task_Uart(void *param)
{
    /* 定义一个缓存数组，以免无效数据顶替上一帧有效数据，造成波动 */
    hi_u8 Uart_Buff_Temp[UART_BUFF_SIZE] = {0};
    hi_unref_param(param);

    for (;;) {
        /* 串口读，记录帧长度 */
        Uart_Rec_Buff_len = hi_uart_read(HI_UART_IDX_1, Uart_Buff_Temp, UART_BUFF_SIZE);
        // for(int t=0;t<11;t++)
        //     printf(Uart_Buff_Temp[t], "\n");
        if (Uart_Rec_Buff_len > 0) {  // 有数据进来
                //printf("Entry Uart *****  %d *****\n ", Uart_Rec_Buff_len, "\n");

            if (Uart_Receive_Flag == HI_FALSE) {
                memcpy_s(Uart_Rec_Buffer, Uart_Rec_Buff_len, Uart_Buff_Temp, Uart_Rec_Buff_len);
                Uart_Receive_Flag = HI_TRUE;
            }
        }
        hi_sleep(20); /* sleep 20ms */
    }

    hi_task_delete(task_uart_id);
    task_uart_id = 0;

    return HI_NULL;
}


/**  
 * @brief Uart的初始化
 * @param hi_uart_idx
 * @attention 波特率
 * @retval void
**/
void Uart_Init(hi_uart_idx id){
    hi_uart_attribute uart_attr = {
        .baud_rate = 115200, /* baud_rate: 115200 */
        .data_bits = 8,      /* data_bits: 8bits */
        .stop_bits = 1,
        .parity = 0,
    };
    hi_u32 temp = hi_uart_init(id, &uart_attr, HI_NULL); // UART2
    if (temp != HI_ERR_SUCCESS) {
        printf("Failed to init uart! Err code = %d\n", temp);
        return;
    }
}
