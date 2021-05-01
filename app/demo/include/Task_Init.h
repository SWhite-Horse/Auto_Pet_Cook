
#ifndef __TASK_INIT_H__
#define __TASK_INIT_H__

#include <hi_types_base.h>
#include <hi_gpio.h>
#include <hi_early_debug.h>
#include <hi_io.h>
#include <hi_watchdog.h>
#include <hi_task.h>
#include <hi_uart.h>
#include <hi_pwm.h>
#include <hi_hrtimer.h>
#include <hi_systick.h>
#include <hi_task.h>
#include <hi_time.h>
#include <hi_timer.h>

#include "Task_LED.h"
#include "Task_Motor.h"
#include "Task_Communication.h"
#include "Task_Sensor.h"

#define hi_unuse_param(p)           ((p) = (p))


extern hi_u32 task_led_id;
extern hi_u32 task_motor_id;
extern hi_u32 task_sensor_id;
extern hi_u32 task_communication_id;

hi_void Task_LED_Create(hi_void);
hi_void Task_Motor_Create(hi_void);
hi_void Task_Communication_Create(hi_void);
hi_void Task_Sensor_Create(hi_void);

hi_void Task_Init(hi_void);

#endif