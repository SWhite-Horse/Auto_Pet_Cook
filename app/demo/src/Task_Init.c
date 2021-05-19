#include "Task_Init.h"

hi_u32 task_led_id;
hi_u32 task_motor_id;
hi_u32 task_sensor_id;
hi_u32 task_communication_id;
hi_u32 task_uart_id;

int Init_sum = 0;


/**  
 * @brief main调用的函数，在此完成外设以及部分端口的初始化，创建所有 Task 以供调度
 * @param void
 * @attention 注意任务锁
 * @retval void
**/
void Task_Init(void){
// ******* 外设初始化区域 ******** //
    //Uart_Init(HI_UART_IDX_1); // 初始化 UART2 与3516通信
    hi_gpio_init();     // 底层 io 口初始化

// ******* 任务初始化区域 ******** //
    hi_task_lock();

    Task_LED_Create();
    Task_Motor_Create();
    Task_Sensor_Create();
    //hisignaling_msg_task();
    //uart_demo();
    //Task_Uart_Create();
    //Task_Communication_Create();

    Init_sum ++;
    hi_task_unlock();

}
