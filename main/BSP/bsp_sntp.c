#include "bsp_sntp.h"

/*
*********************************************************************************************************
*	函 数 名: time_update_callback
*	功能说明:  SNTP时间校准回调函数
*	形    参:
*	返 回 值: 无
*********************************************************************************************************
*/
static void time_update_callback(struct timeval *tv)
{
    /* 设置时区 */
    setenv("TZ", "CST-8", 1);
    tzset();
}

/*
*********************************************************************************************************
*	函 数 名: bsp_GetNetworkTime
*	功能说明: SNTP时间校准,需要连接网络之后才能使用
*	形    参: buad_rate 波特率
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_GetNetworkTime(void)
{
    /* SNTP校时 */
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "ntp.aliyun.com");
    sntp_set_time_sync_notification_cb(time_update_callback);
    sntp_init(); // 启动校时
}
