#include "Task_LED.h"

int LED_num = 0; // 暂时用来监视任务执行情况

hi_void *Task_LED(hi_void *param){
    
    hi_unuse_param(param);
    for(;;){
        LED_num++;
        dprintf("Task_LED_Working!/n");
        hi_sleep(20);
    }
    if (hi_task_delete(task_led_id) != HI_ERR_SUCCESS) {
        printf("Failed to delete led control demo task\r\n");
    }
}

hi_void Task_LED_Create(hi_void){
    hi_task_attr led_attr = {0};
    led_attr.stack_size = 1024;
    led_attr.task_prio = 26;
    led_attr.task_name = (hi_char*)"Task_LED";
    if (hi_task_create(&task_led_id, &led_attr, Task_LED, HI_NULL) != HI_ERR_SUCCESS) {
        printf("Failed to create motor_task\r\n");
        return HI_ERR_FAILURE;
    }
    return HI_ERR_SUCCESS;
}
