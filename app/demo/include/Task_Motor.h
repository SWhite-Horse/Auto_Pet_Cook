
#ifndef __TASK_MOTOR_H__
#define __TASK_MOTOR_H__

#include "Task_Init.h"

#define MOTOR_ANGLE 2000
#define MOTOR_STOP 0


// ******** 电机 ID ********* //
typedef enum {
    Motor_Left, // 底部摄像头左下
    Motor_Right, // 底部摄像头右下
    Motor_Up // 粮仓电机
} Motor_ID_enum;

// ******** 电机转动方向状态 ********* //
typedef enum {
    Motor_Stop, // 停止状态
    Motor_CW, // 顺时针
    Motor_CCW // 逆时针
} Motor_status_dir_enum;

// ******** 电机状态 ********* //
typedef enum {
    MOTOR_DOWN_CAT_OUT, //
    MOTOR_DOWN_CAT_BACK, 
    MOTOR_DOWN_DOG_OUT, //
    MOTOR_DOWN_DOG_BACK,
    MOTOR_UP_CAT, // 
    MOTOR_UP_DOG,
    MOTOR_UP_HORIZONTAL,
} Motor_status_enum;

void *Task_Motor(void *);
hi_u8 Motor_control(Motor_status_dir_enum motor_status, Motor_ID_enum motor_id);
void Motor_Enable(Motor_status_dir_enum motor_status, hi_io_name motor_name, hi_u8 func, hi_gpio_idx motor_id);
void Motor_GPIO_Init(void);


#endif
