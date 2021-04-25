#include "Task_Communication.h"

int Communication_num = 0;

hi_void *Task_Communication(hi_void * param){
    hi_unuse_param(param);
    for(;;){
        Communication_num++;
        dprintf("Task_Communication_Working!/n");
        hi_sleep(20);
    }
    if (hi_task_delete(task_communication_id) != HI_ERR_SUCCESS) {
        printf("Failed to delete task communication\r\n");
    }
}

hi_void Task_Communication_Create(hi_void){
    hi_task_attr communication_attr = {0};
    communication_attr.stack_size = 1024;
    communication_attr.task_prio = 24;
    communication_attr.task_name = (hi_char*)"Task_Communication";
    if (hi_task_create(&task_communication_id, &communication_attr, Task_Communication, HI_NULL) != HI_ERR_SUCCESS) {
        printf("Failed to create task communication\r\n");
        return HI_ERR_FAILURE;
    }
    return HI_ERR_SUCCESS;
}

