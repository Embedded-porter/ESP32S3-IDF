#ifndef _BSP_SOFTAP_H
#define _BSP_SOFTAP_H

#include "includes.h"

#define ESP_WIFI_SSID "USART"     /* WIFI�ȵ������ */
#define ESP_WIFI_PASS "123456789" /* WIFI�ȵ������ */
#define ESP_WIFI_CHANNEL 1        /* WIFI�ȵ���ŵ� */
#define MAX_STA_CONN 4            /* MAX_STA_CONN */

/*
*********************************************************************************************************
*	�� �� ��: bsp_WifiAPConnect
*	����˵��: APģʽ���Ӻ���
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_WifiAPConnect(void);

#endif