#include "Task_Sensor.h"

int Sensor_num = 0;

hi_void *Task_Sensor(hi_void * param){
    hi_unuse_param(param);
    for(;;){
        Sensor_num++;
        dprintf("Task_Sensor_Working!/n");
        hi_sleep(20);
    }
    if (hi_task_delete(task_sensor_id) != HI_ERR_SUCCESS) {
        printf("Failed to delete task sensor\r\n");
    }
}

hi_void Task_Sensor_Create(hi_void){
    hi_task_attr sensor_attr = {0};
    sensor_attr.stack_size = 1024;
    sensor_attr.task_prio = 23;
    sensor_attr.task_name = (hi_char*)"Task_Sensor";
    if (hi_task_create(&task_sensor_id, &sensor_attr, Task_Sensor, HI_NULL) != HI_ERR_SUCCESS) {
        printf("Failed to create sensor_task\r\n");
        return HI_ERR_FAILURE;
    }
    return HI_ERR_SUCCESS;
}


