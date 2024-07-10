#include "bsp.h"

static const char *TAG = "BSP";

/*
*********************************************************************************************************
*	�� �� ��: bsp_Init
*	����˵��: ��ʼ�����е�Ӳ���豸���ú�������CPU�Ĵ���������ļĴ�������ʼ��һЩȫ�ֱ�����ֻ��Ҫ����һ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_Init(void)
{
    uint16_t ret;
    /* ���õ�ǰwifiΪAPģʽ */
    // bsp_WifiAPConnect();
    /* ���õ�ǰwifiΪSTAģʽ */
    bsp_WifiSTAConnect();
    /* ��ʼ������ */
    bsp_adc_button();
    /* ��ʼ��pca9557��չоƬ */
    ret = tca9554_init();
    if (ret != true)
    {
        ESP_LOGE(TAG, "bsp_Pca9557Init failed\r\n");
    }
    /* ��ʼ��LCD�� */
    bsp_display();
    /*��ʼ��LVGL_UI*/
    bsp_ui_init();
    /* ��ʼ������0 */
    bsp_Uart0Init(115200);

    /* USB_CDC��ʼ�� */
     cdc_acm_init1(0, 0x60080000);
}
