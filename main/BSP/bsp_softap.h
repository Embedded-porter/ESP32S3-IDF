#ifndef _BSP_SOFTAP_H
#define _BSP_SOFTAP_H

#include "includes.h"

#define ESP_WIFI_SSID "USART"     /* WIFI热点的名字 */
#define ESP_WIFI_PASS "123456789" /* WIFI热点的密码 */
#define ESP_WIFI_CHANNEL 1        /* WIFI热点的信道 */
#define MAX_STA_CONN 4            /* MAX_STA_CONN */

/*
*********************************************************************************************************
*	函 数 名: bsp_WifiAPConnect
*	功能说明: AP模式连接函数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_WifiAPConnect(void);

#endif