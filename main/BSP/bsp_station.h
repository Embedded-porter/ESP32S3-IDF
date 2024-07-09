#ifndef _BSP_STATION_H
#define _BSP_STATION_H

#include "includes.h"

/* ��ʾ���ӳɹ���־λ */
#define WIFI_CONNECTED_BIT BIT0
/* ��ʾ����ʧ�ܱ�־λ */
#define WIFI_FAIL_BIT BIT1

/*wifi ����*/
#define ESP_STA_WIFI_SSID  "USA"  
/*wifi ����*/
#define ESP_STA_WIFI_PASS "123456789"
/* Station����APʱ���ԵĴ���*/ 
#define ESP_MAXIMUM_RETRY 5  
/*
*********************************************************************************************************
*	�� �� ��: bsp_WifiSTAConnect
*	����˵��: STAģʽ���Ӻ���
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_WifiSTAConnect(void);

#endif