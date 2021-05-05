
#ifndef __TASK_COMMUNICATION_H__
#define __TASK_COMMUNICATION_H__

#include "Task_Init.h"

#define COMMUNICATION_UART_SIZ 3
#define COMMUNICATION_SOF ((hi_u8)0xCC)
#define COMMUNICATION_EOF ((hi_u8)0xEE)


typedef struct 
{
    hi_u8 SOF; // 头帧
    hi_u8 Alive; // 视野范围内有无 猫猫 \ 狗狗， 0 代表无， 1代表猫猫， 2代表 蠢狗
    hi_u8 EOF; //尾帧
}Com_3516_TO_3861_struct;


void *Task_Communication(void *);
hi_u8 Receive_UART_From_Taurus(hi_u8* buff, hi_s32 len);

#endif
