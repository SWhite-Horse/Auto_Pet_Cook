#include "Task_Communication.h"

int Communication_num = 0;


/**  
 * @brief 通信任务
 * @param  void
 * @attention
 * @retval void
**/
void *Task_Communication(void * param){
    hi_unuse_param(param);
    for(;;){

        Communication_num++;
        printf("Task_Communication_Working!\n");
        hi_sleep(20);
    }
    if (hi_task_delete(task_communication_id) != HI_ERR_SUCCESS) {
        printf("Failed to delete task communication\r\n");
    }
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

