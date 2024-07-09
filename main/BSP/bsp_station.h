#ifndef _BSP_STATION_H
#define _BSP_STATION_H

#include "includes.h"

/* 表示连接成功标志位 */
#define WIFI_CONNECTED_BIT BIT0
/* 表示连接失败标志位 */
#define WIFI_FAIL_BIT BIT1

/*wifi 名称*/
#define ESP_STA_WIFI_SSID  "USA"  
/*wifi 密码*/
#define ESP_STA_WIFI_PASS "123456789"
/* Station连接AP时重试的次数*/ 
#define ESP_MAXIMUM_RETRY 5  
/*
*********************************************************************************************************
*	函 数 名: bsp_WifiSTAConnect
*	功能说明: STA模式连接函数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_WifiSTAConnect(void);

#endif