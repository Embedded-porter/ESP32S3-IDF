#include "bsp_sntp.h"

/*
*********************************************************************************************************
*	�� �� ��: time_update_callback
*	����˵��:  SNTPʱ��У׼�ص�����
*	��    ��:
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void time_update_callback(struct timeval *tv)
{
    /* ����ʱ�� */
    setenv("TZ", "CST-8", 1);
    tzset();
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_GetNetworkTime
*	����˵��: SNTPʱ��У׼,��Ҫ��������֮�����ʹ��
*	��    ��: buad_rate ������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_GetNetworkTime(void)
{
    /* SNTPУʱ */
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "ntp.aliyun.com");
    sntp_set_time_sync_notification_cb(time_update_callback);
    sntp_init(); // ����Уʱ
}
