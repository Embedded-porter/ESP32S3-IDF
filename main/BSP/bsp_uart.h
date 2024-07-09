#ifndef _BSP_UART_H
#define _BSP_UART_H 

#include "includes.h"

#define EX_UART_NUM UART_NUM_0

#define PATTERN_CHR_NUM (1) /* 检测串口特定字符的数量*/

#define BUF_SIZE (1024)

#define RD_BUF_SIZE (BUF_SIZE)

/*
*********************************************************************************************************
*	函 数 名: bsp_Uart0Init
*	功能说明: 串口初始化
*	形    参: buad_rate 波特率
*	返 回 值: 无
*********************************************************************************************************
*/
extern void bsp_Uart0Init(uint32_t buad_rate);

#endif 