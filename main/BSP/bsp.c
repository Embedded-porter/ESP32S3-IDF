#include "bsp.h"

static const char *TAG = "BSP";

/*
*********************************************************************************************************
*	函 数 名: bsp_Init
*	功能说明: 初始化所有的硬件设备。该函数配置CPU寄存器和外设的寄存器并初始化一些全局变量。只需要调用一次
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_Init(void)
{
    uint16_t ret;
    /* 设置当前wifi为AP模式 */
    // bsp_WifiAPConnect();
    /* 设置当前wifi为STA模式 */
    bsp_WifiSTAConnect();
    /* 初始化按键 */
    bsp_adc_button();
    /* 初始化pca9557扩展芯片 */
    ret = tca9554_init();
    if (ret != true)
    {
        ESP_LOGE(TAG, "bsp_Pca9557Init failed\r\n");
    }
    /* 初始化LCD屏 */
    bsp_display();
    /*初始化LVGL_UI*/
    bsp_ui_init();
    /* 初始化串口0 */
    bsp_Uart0Init(115200);

    /* USB_CDC初始化 */
     cdc_acm_init1(0, 0x60080000);
}
