#ifndef _BSP_UART_H
#define _BSP_UART_H 

#include "includes.h"

#define EX_UART_NUM UART_NUM_0

#define PATTERN_CHR_NUM (1) /* ��⴮���ض��ַ�������*/

#define BUF_SIZE (1024)

#define RD_BUF_SIZE (BUF_SIZE)

/*
*********************************************************************************************************
*	�� �� ��: bsp_Uart0Init
*	����˵��: ���ڳ�ʼ��
*	��    ��: buad_rate ������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
extern void bsp_Uart0Init(uint32_t buad_rate);

#endif 