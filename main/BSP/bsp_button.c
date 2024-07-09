#include "bsp_button.h"


/* 按键消息队列 */
extern  QueueHandle_t ButtonQueueHandle;

static button_handle_t g_btns[BUTTON_NUM] = {0};

/*
*********************************************************************************************************
*	函 数 名: get_btn_index
*	功能说明: 获取按键索引
*	形    参: btn 按键索引值
*	返 回 值: 返回索引值
*********************************************************************************************************
*/
static int get_btn_index(button_handle_t btn)
{
    for (size_t i = 0; i < BUTTON_NUM; i++)
    {
        if (btn == g_btns[i])
        {
            return i;
        }
    }
    return -1;
}

/*
*********************************************************************************************************
*	函 数 名: button_press_down_cb
*	功能说明: 获取按键索引,并且将值发送到按键消息队列
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
static void button_press_down_cb(void *arg, void *data)
{  
    int btn_index = get_btn_index((button_handle_t)arg);
    xQueueSend(ButtonQueueHandle, &btn_index, 0);
}

/*
*********************************************************************************************************
*	函 数 名: bsp_adc_button
*	功能说明: 初始化按键
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_adc_button(void)
{
    const uint16_t vol[6] = {380, 820, 1180, 1570, 1980, 2410};

    adc_oneshot_unit_handle_t adc1_handle;
    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = ADC_UNIT_1,
    };
    adc_oneshot_new_unit(&init_config, &adc1_handle);

    button_config_t cfg = {0};
    cfg.type = BUTTON_TYPE_ADC;
    cfg.long_press_time = CONFIG_BUTTON_LONG_PRESS_TIME_MS;     
    cfg.short_press_time = CONFIG_BUTTON_SHORT_PRESS_TIME_MS;
    for (size_t i = 0; i < 6; i++)
    {
        cfg.adc_button_config.adc_handle = &adc1_handle;
        cfg.adc_button_config.adc_channel = 4,
        cfg.adc_button_config.button_index = i;
        if (i == 0)
        {
            cfg.adc_button_config.min = (0 + vol[i]) / 2;
        }
        else
        {
            cfg.adc_button_config.min = (vol[i - 1] + vol[i]) / 2;
        }

        if (i == 5)
        {
            cfg.adc_button_config.max = (vol[i] + 3000) / 2;
        }
        else
        {
            cfg.adc_button_config.max = (vol[i] + vol[i + 1]) / 2;
        }

        g_btns[i] = iot_button_create(&cfg);
        iot_button_register_cb(g_btns[i], BUTTON_PRESS_DOWN, button_press_down_cb, NULL);
    }
}