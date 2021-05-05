
#ifndef __APP_DEMO_UART_H__
#define __APP_DEMO_UART_H__

#include "Task_Init.h"
/*
#define WRITE_BY_INT
#define UART_DEMO_TASK_STAK_SIZE 2048
#define UART_DEMO_TASK_PRIORITY  25
#define DEMO_UART_NUM            HI_UART_IDX_1*/
#define UART_BUFF_SIZE           4

void *Task_Uart(void *param);
void Task_Uart_Create(void);
void Uart_Init(hi_uart_idx id);

#endif