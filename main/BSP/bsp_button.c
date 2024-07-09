#include "bsp_button.h"


/* ������Ϣ���� */
extern  QueueHandle_t ButtonQueueHandle;

static button_handle_t g_btns[BUTTON_NUM] = {0};

/*
*********************************************************************************************************
*	�� �� ��: get_btn_index
*	����˵��: ��ȡ��������
*	��    ��: btn ��������ֵ
*	�� �� ֵ: ��������ֵ
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
*	�� �� ��: button_press_down_cb
*	����˵��: ��ȡ��������,���ҽ�ֵ���͵�������Ϣ����
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
static void button_press_down_cb(void *arg, void *data)
{  
    int btn_index = get_btn_index((button_handle_t)arg);
    xQueueSend(ButtonQueueHandle, &btn_index, 0);
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_adc_button
*	����˵��: ��ʼ������
*	��    ��: ��
*	�� �� ֵ: ��
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