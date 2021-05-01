#include "Task_Sensor.h"

int Sensor_num = 0;


/**  
 * @brief 压力传感器（或者还有其它）信息处理任务
 * @param  void
 * @attention
 * @retval void
**/
void *Task_Sensor(void * param){
    hi_unuse_param(param);
    for(;;){
        Sensor_num++;
        printf("Task_Sensor_Working!\n");
        hi_sleep(20);
    }
    if (hi_task_delete(task_sensor_id) != HI_ERR_SUCCESS) {
        printf("Failed to delete task sensor\r\n");
    }
}


/**  
 * @brief 创建压力传感器信号处理任务
 * @param  void
 * @attention
 * @retval void
**/
void Task_Sensor_Create(void){
    hi_task_attr sensor_attr = {0};
    sensor_attr.stack_size = 1024;
    sensor_attr.task_prio = 23;
    sensor_attr.task_name = (char*)"Task_Sensor";
    if (hi_task_create(&task_sensor_id, &sensor_attr, Task_Sensor, HI_NULL) != HI_ERR_SUCCESS) {
        dprintf("Failed to create sensor_task\r\n");
        return HI_ERR_FAILURE;
    }
    return HI_ERR_SUCCESS;
}


