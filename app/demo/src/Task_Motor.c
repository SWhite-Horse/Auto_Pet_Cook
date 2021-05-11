#include "Task_Motor.h"

int Motor_num = 1; // Debug
hi_u8 reg; // Debug
hi_gpio_dir dir; // Debug
extern Motor_status_enum Motor_Left_Status, Motor_Right_Status, Motor_Up_Status;


/**  
 * @brief 创建电机控制任务
 * @param  void
 * @attention
 * @retval void
**/
void Task_Motor_Create(void){
    hi_task_attr motor_attr = {0};
    motor_attr.stack_size = 1024;
    motor_attr.task_prio = 25;
    motor_attr.task_name = (char*)"Task_Motor";
    if (hi_task_create(&task_motor_id, &motor_attr, Task_Motor, HI_NULL) != HI_ERR_SUCCESS) {
        printf("Failed to create motor_task\r\n");
        return HI_ERR_FAILURE;
    }
    return HI_ERR_SUCCESS;
}



/**  
 * @brief 电机控制任务
 * @param  void
 * @attention PWM的控制方式 三个舵机 手动给的方波
 * @retval void
**/
void *Task_Motor(void * param){
    hi_unuse_param(param);
    
    for(;;){
        Motor_num++;
        /* 右侧电机控制 */
        if(Motor_Right_Status == MOTOR_DOWN_CAT_BACK)
            Motor_control(Motor_CW, Motor_Right);
        else
            Motor_control(Motor_CCW, Motor_Right);
        /* 左侧电机控制 */
        if(Motor_Left_Status == MOTOR_DOWN_DOG_BACK)
            Motor_control(Motor_CW, Motor_Left);
        else
            Motor_control(Motor_CCW, Motor_Left);
        /* UP电机控制 */
        if(Motor_Up_Status == MOTOR_UP_DOG)
            Motor_control(Motor_CW, Motor_Up);
        else if(Motor_Up_Status == MOTOR_UP_CAT)
            Motor_control(Motor_CCW, Motor_Up);
        else 
            Motor_control(Motor_Stop, Motor_Up);
        hi_sleep(20);
    }
    if (hi_task_delete(task_motor_id) != HI_ERR_SUCCESS) {
        printf("Failed to delete task motor\r\n");

    }
}


/**  
 * @brief GPIO 初始化
 * @param  void
 * @attention 注意不要和其它功能口 类如UART的GPIO脚冲突
 * @retval void
**/
void Motor_GPIO_Init(){
    // ******* 自定义GPIO口的初始化 ******** //

    // **** GPIO2 初始化 **** //
    
    // **** GPIOX 初始化 **** //
}

/**  
 * @brief 电机控制选择
 * @param  status 电机转向，CW 顺时针 CCW 逆时针
 * @param id 电机选择
 * @attention
 * @attention 外设扩展版上， 舵机1 --- GPIO6
 * @attention 舵机2 --- GPIO7  --- 人体红外 OUT
 * @attention 舵机3 --- GPIO9  --- 光敏 DO
 * @attention 舵机4 --- GPIO10
 * @attention LED1  --- GPIO2
 * @attention LED2  --- GPIO8
 * @attention LED3  --- GPIO11 --- 红外测距
 * @attention LED4  --- GPIO12 --- 超声波 
 * @retval void
**/
hi_u8 Motor_control(Motor_status_dir_enum motor_status, Motor_ID_enum motor_id){
    switch (motor_id)
    {
    case Motor_Left:
        Motor_Enable(motor_status, HI_IO_NAME_GPIO_2, HI_IO_FUNC_GPIO_2_GPIO, HI_GPIO_IDX_2);
        break;
    case Motor_Right:
        Motor_Enable(motor_status, HI_IO_NAME_GPIO_10, HI_IO_FUNC_GPIO_10_GPIO, HI_GPIO_IDX_10);
        break;
    case Motor_Up:
        Motor_Enable(motor_status, HI_IO_NAME_GPIO_6, HI_IO_FUNC_GPIO_6_GPIO, HI_GPIO_IDX_6);
        break;
    default:
        return 0;
        break;
    }
    return 1;
}


/**  
 * @brief 电机转动控制
 * @param status 转动方向  
 * @param name GPIO口的Name
 * @param func GPIO口输出模式
 * @param id GPIO口的ID
 * @attention
 * @retval void
**/
void Motor_Enable(Motor_status_dir_enum motor_status, hi_io_name motor_name, hi_u8 func, hi_gpio_idx motor_id){
    if(motor_status == Motor_Stop){
        hi_io_set_func(motor_name, func); // 设置此GPIO的功能
        hi_gpio_set_dir(motor_id, HI_GPIO_DIR_OUT); // 设置输出 为 out 模式

        /* 此下两个delay分别是高和低电平延迟的时间 */
        hi_gpio_set_ouput_val(motor_id, HI_GPIO_VALUE1);
        hi_udelay(1300); 
        hi_gpio_set_ouput_val(motor_id, HI_GPIO_VALUE0);      
        hi_udelay(20000 - 1300);
    }
    else if(motor_status == Motor_CW){
        hi_io_set_func(motor_name, func); // 设置此GPIO的功能
        hi_gpio_set_dir(motor_id, HI_GPIO_DIR_OUT); // 设置输出 为 out 模式

        /* 此下两个delay分别是高和低电平延迟的时间 */
        hi_gpio_set_ouput_val(motor_id, HI_GPIO_VALUE1);
        hi_udelay(2100); 
        hi_gpio_set_ouput_val(motor_id, HI_GPIO_VALUE0);      
        hi_udelay(20000 - 2100);

    }
    else if(motor_status == Motor_CCW){
        hi_io_set_func(motor_name, func); // 设置此GPIO的功能
        hi_gpio_set_dir(motor_id, HI_GPIO_DIR_OUT); // 设置输出 为 out 模式

        hi_gpio_set_ouput_val(motor_id, HI_GPIO_VALUE1);
        hi_udelay(500);
        hi_gpio_set_ouput_val(motor_id, HI_GPIO_VALUE0);
        hi_udelay(20000 - 500);
        printf("CCW\n");

    }
}


