#include "Task_Init.h"

hi_u32 task_led_id;
hi_u32 task_motor_id;
hi_u32 task_sensor_id;
hi_u32 task_communication_id;

hi_void Task_Init(hi_void){
// ******* 外设初始化区域 ******** //


// ******* 任务初始化区域 ******** //
    hi_task_lock();

    Task_LED_Create();
    Task_Motor_Create();
    Task_Sensor_Create();
    Task_Communication_Create();

    hi_task_unlock();

}
