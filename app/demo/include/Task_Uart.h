
#ifndef __TASK_UART_H__
#define __TASK_UART_H__

#include "Task_Init.h"

#define WRITE_BY_INT
#define UART_DEMO_TASK_STAK_SIZE 2048
#define UART_DEMO_TASK_PRIORITY  25
#define DEMO_UART_NUM            HI_UART_IDX_1
#define UART_BUFF_SIZE           32

extern hi_bool  Uart_Receive_Flag;
extern hi_u8    Uart_Rec_Buffer[UART_BUFF_SIZE];
extern hi_s32   Uart_Rec_Buff_len;

void *Task_Uart(void *param);
void Task_Uart_Create(void);
void Uart_Init(hi_uart_idx id);

#endif