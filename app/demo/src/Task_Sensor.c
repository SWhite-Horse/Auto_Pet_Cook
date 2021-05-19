#include "Task_Sensor.h"

int Sensor_num = 0;
hi_u16 Pressed_data = 0; // 


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


/**  
 * @brief 压力传感器（或者还有其它）信息处理任务
 * @param  void
 * @attention
 * @retval void
**/
void *Task_Sensor(void * param){
    hi_unuse_param(param);
    hi_io_set_func(HI_IO_NAME_GPIO_7, HI_IO_FUNC_GPIO_7_GPIO);
    hi_gpio_set_dir(HI_GPIO_IDX_7, HI_GPIO_DIR_IN);
    // hi_u32 ret = 0;

    for(;;){
        Sensor_num++;
        printf("Task_Sensor_Working!\n");

        if(hi_adc_read(HI_ADC_CHANNEL_3, &Pressed_data, HI_ADC_EQU_MODEL_4, HI_ADC_CUR_BAIS_DEFAULT, 0) != HI_ERR_SUCCESS){
            printf("ADC READ FAILED !\n");
            return HI_NULL;
        }
        printf("Pressed data:%d\r\n", Pressed_data);
        hi_io_set_func(HI_IO_NAME_GPIO_7, HI_IO_FUNC_GPIO_7_GPIO);
        hi_gpio_set_dir(HI_GPIO_IDX_7, HI_GPIO_DIR_IN);        

        hi_sleep(500);
    }
    if (hi_task_delete(task_sensor_id) != HI_ERR_SUCCESS) {
        printf("Failed to delete task sensor\r\n");
    }
}

