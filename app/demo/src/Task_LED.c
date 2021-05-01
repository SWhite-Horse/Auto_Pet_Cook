#include "Task_LED.h"

int LED_num = 0; // 暂时用来监视任务执行情况


/**  
 * @brief LED任务
 * @param  void
 * @attention 此任务只用来判断程序是否跑死
 * @retval void
**/
void *Task_LED(void *param){
    
    hi_unuse_param(param);
    for(;;){
        LED_num++;
        printf("Task_LED_Working!\n");
        hi_sleep(20);
    }
    if (hi_task_delete(task_led_id) != HI_ERR_SUCCESS) {
        printf("Failed to delete led control demo task\r\n");
    }
}


/**  
 * @brief 创建LED任务
 * @param  void
 * @attention
 * @retval void
**/
void Task_LED_Create(void){
    hi_task_attr led_attr = {0};
    led_attr.stack_size = 1024;
    led_attr.task_prio = 11;
    led_attr.task_name = (char*)"Task_LED";
    if (hi_task_create(&task_led_id, &led_attr, Task_LED, HI_NULL) != HI_ERR_SUCCESS) {
        printf("Failed to create motor_task\r\n");
        return HI_ERR_FAILURE;
    }
    return HI_ERR_SUCCESS;
}
