#ifndef _BSP_BUTTON_H
#define _BSP_BUTTON_H

#include "includes.h"

/* 定义按键的数量 */
#define BUTTON_NUM BSP_BUTTON_NUM

typedef enum
{

    BSP_BUTTON_VOLUP = 0,
    BSP_BUTTON_VOLDOWN,
    BSP_BUTTON_SET,
    BSP_BUTTON_PLAY,
    BSP_BUTTON_MUTE,
    BSP_BUTTON_REC,
    BSP_BUTTON_MAIN,
    BSP_BUTTON_NUM,
} bsp_button_t;

void bsp_adc_button(void);


#endif 