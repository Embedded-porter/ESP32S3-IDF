#ifndef _BSP_H
#define _BSP_H

/*硬件头文件*/
/***********************系统*****************************/
#include "sdkconfig.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_log.h"
#include "esp_task_wdt.h"
#include "esp_mac.h"
#include "esp_wifi.h"
#include "esp_adc/adc_cali.h"
#include "esp_check.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "driver/spi_master.h"
#include "driver/uart.h"

/***********************BSP******************************/
#include "bsp_softap.h"
#include "bsp_station.h"

/* ADC BUTTON */
#include "iot_button.h"
#include "bsp_button.h"
/* TCA9554*/
#include "esp_io_expander_tca9554.h"
#include "esp_io_expander.h"
#include "bsp_tca9554.h"
/* LCD显示屏 */
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lvgl_port.h"
#include "lv_examples.h"
#include "esp_lcd_touch_gt911.h"
#include "esp_lcd_touch.h"
#include "bsp_lcd.h"
/*串口*/
#include "bsp_uart.h"
#include "bsp_command.h"

/*SNTP*/
#include "bsp_sntp.h"
#include "esp_sntp.h"
#include "esp_netif.h"

/*LVGL_UI */
#include "bsp_ui.h"
#include "gui_guider.h"

/*FAT文件系统 */
#include "bsp_fat.h"
#include "esp_vfs.h"
#include "esp_vfs_fat.h"

/* 网络 */
#include <sys/socket.h>

/* MQTT */
#include "mqtt_client.h"
#include "bsp_mqtt_client.h"

/*MODBUS */


void bsp_Init(void);

#endif
