#include "Task_Motor.h"

int Motor_num = 0;

hi_void *Task_Motor(hi_void * param){
    hi_unuse_param(param);
    for(;;){
        Motor_num++;
        dprintf("Task_Motor_Working!/n");
        hi_sleep(20);
    }
    if (hi_task_delete(task_motor_id) != HI_ERR_SUCCESS) {
        printf("Failed to delete task motor\r\n");
    }
}

hi_void Task_Motor_Create(hi_void){
    hi_task_attr motor_attr = {0};
    motor_attr.stack_size = 1024;
    motor_attr.task_prio = 25;
    motor_attr.task_name = (hi_char*)"Task_Motor";
    if (hi_task_create(&task_motor_id, &motor_attr, Task_Motor, HI_NULL) != HI_ERR_SUCCESS) {
        printf("Failed to create motor_task\r\n");
        return HI_ERR_FAILURE;
    }
    return HI_ERR_SUCCESS;
}

