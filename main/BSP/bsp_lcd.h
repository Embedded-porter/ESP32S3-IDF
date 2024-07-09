#ifndef _BSP_LCD_H
#define _BSP_LCD_H

#include "bsp.h"

#define LCD_H_RES   (240)           
#define LCD_V_RES   (320)
/* LCD settings */
#define LCD_SPI_NUM         (SPI3_HOST)
#define LCD_PIXEL_CLK_HZ    (40 * 1000 * 1000)
#define LCD_CMD_BITS        (8)
#define LCD_PARAM_BITS      (8)
#define LCD_COLOR_SPACE     (ESP_LCD_COLOR_SPACE_RGB)
#define LCD_BITS_PER_PIXEL  (16)
#define LCD_DRAW_BUFF_DOUBLE (1)
#define LCD_DRAW_BUFF_HEIGHT (50)
#define LCD_BL_ON_LEVEL     (1)

/* LCD pins */
#define LCD_GPIO_SCLK       (GPIO_NUM_1)
#define LCD_GPIO_MOSI       (GPIO_NUM_0)
#define LCD_GPIO_RST        (-1)
#define LCD_GPIO_DC         (GPIO_NUM_2)
#define LCD_GPIO_CS         (GPIO_NUM_46)
#define LCD_GPIO_BL         (GPIO_NUM_45)

/* Touch settings */
#define TOUCH_I2C_NUM       (0)
#define TOUCH_I2C_CLK_HZ    (100000)

/* LCD touch pins */
#define TOUCH_I2C_SCL       (GPIO_NUM_18)
#define TOUCH_I2C_SDA       (GPIO_NUM_17)
#define TOUCH_GPIO_INT      (-1)

extern void bsp_display(void);

#endif