#include "Task_Communication.h"

Com_3516_TO_3861_struct comu_uart;
extern hi_bool  Uart_Receive_Flag;
extern hi_u8    Uart_Rec_Buffer[UART_BUFF_SIZE];
extern hi_s32   Uart_Rec_Buff_len;

int Communication_num = 0; // Debug


/**  
 * @brief 创建与3516通信的Task
 * @param  void
 * @attention
 * @retval void
**/
void Task_Communication_Create(void){

    hi_task_attr communication_attr = {0};
    communication_attr.stack_size = 2048;
    communication_attr.task_prio = 24;
    communication_attr.task_name = (char*)"Task_Communication";
    if (hi_task_create(&task_communication_id, &communication_attr, Task_Communication, HI_NULL) != HI_ERR_SUCCESS) {
        printf("Failed to create task communication\r\n");
        return HI_ERR_FAILURE;
    }
    return HI_ERR_SUCCESS;
}


/**  
 * @brief 通信任务
 * @param  void
 * @attention
 * @retval void
**/
void *Task_Communication(void * param){
    hi_unuse_param(param);
    for(;;){
        if(Uart_Receive_Flag){
            Receive_UART_From_Taurus(Uart_Rec_Buffer, Uart_Rec_Buff_len);

            /* 重置等待下一次接收 */
            Uart_Receive_Flag = HI_FALSE;
            memset_s(Uart_Rec_Buffer, UART_BUFF_SIZE, 0x0, UART_BUFF_SIZE); // 重置数组
        }
        
        Communication_num++;
        printf("Task_Communication_Working! %d\n", Communication_num);
        hi_sleep(20);
    }
    if (hi_task_delete(task_communication_id) != HI_ERR_SUCCESS) {
        printf("Failed to delete task communication\r\n");
    }
    task_communication_id = 0;

}


/**  
 * @brief 接收任务
 * @param buff Uart read进来的存储器， 
 * @param len 数据长度
 * @attention
 * @retval void
**/
hi_u8 Receive_UART_From_Taurus(hi_u8* buff, hi_s32 len){
    if(buff == HI_NULL && len > 0) {
        printf("received buf is null");
        return 0;
    }

    comu_uart.SOF = Uart_Rec_Buffer[0];
    comu_uart.EOF = Uart_Rec_Buffer[2];

    if(comu_uart.SOF == COMMUNICATION_SOF && comu_uart.EOF == COMMUNICATION_EOF){
        comu_uart.Alive = Uart_Rec_Buffer[1];
    } 

    return 1;
}
