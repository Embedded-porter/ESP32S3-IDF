#include "bsp_lcd.h"

static const char *TAG = "ESP32_LCD_TOUCH";

/* LCD IO and panel */
static esp_lcd_panel_io_handle_t lcd_io = NULL;
static esp_lcd_panel_handle_t lcd_panel = NULL;
static esp_lcd_touch_handle_t touch_handle = NULL;

/* LVGL display and touch */
lv_disp_t *lvgl_disp = NULL;
static lv_indev_t *lvgl_touch_indev = NULL;

/*
*********************************************************************************************************
*	函 数 名: bsp_LcdInit
*	功能说明: 初始化LCD屏
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static esp_err_t bsp_lcd_init(void)
{
    esp_err_t ret = ESP_OK;

    /* LCD backlight */
    gpio_config_t bk_gpio_config = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << LCD_GPIO_BL
    };
    ESP_ERROR_CHECK(gpio_config(&bk_gpio_config));


    /* LCD initialization SPI总线初始化*/
    ESP_LOGD(TAG, "Initialize SPI bus");
    const spi_bus_config_t buscfg = {
        .sclk_io_num = LCD_GPIO_SCLK,                                           // 连接 LCD SCK（SCL） 信号的 IO 编号
        .mosi_io_num = LCD_GPIO_MOSI,                                           // 主机发送数据到从机
        .miso_io_num = GPIO_NUM_NC,                                             // 从机发送数据到主机,不需要可以设置为'-1'
        .quadwp_io_num = GPIO_NUM_NC,                                           // 写保护信号。只用于 4 位 (qio/qout) 传输。在 Octal/OPI 模式下也表示为 data2 信号。
        .quadhd_io_num = GPIO_NUM_NC,                                           // 保持信号。只用于 4 位 (qio/qout) 传输。在 Octal/OPI 模式下也表示为 data3 信号。
        .max_transfer_sz = LCD_H_RES * LCD_DRAW_BUFF_HEIGHT * sizeof(uint16_t),            // 表示 SPI 单次传输允许的最大字节数上限，通常设为全屏大小即可
    };
    ESP_RETURN_ON_ERROR(spi_bus_initialize(LCD_SPI_NUM, &buscfg, SPI_DMA_CH_AUTO), TAG, "SPI init failed");

    ESP_LOGD(TAG, "Install panel IO");
    const esp_lcd_panel_io_spi_config_t io_config = {
        .dc_gpio_num = LCD_GPIO_DC,
        .cs_gpio_num = LCD_GPIO_CS,
        .pclk_hz = LCD_PIXEL_CLK_HZ,
        .lcd_cmd_bits = LCD_CMD_BITS,
        .lcd_param_bits = LCD_PARAM_BITS,
        .spi_mode = 0,
        .trans_queue_depth = 10,
    };
    ESP_GOTO_ON_ERROR(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_SPI_NUM, &io_config, &lcd_io), err, TAG, "New panel IO failed");

    ESP_LOGD(TAG, "Install LCD driver");
    const esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = LCD_GPIO_RST,
        .color_space = LCD_COLOR_SPACE,
        .bits_per_pixel = LCD_BITS_PER_PIXEL,
    };
    ESP_GOTO_ON_ERROR(esp_lcd_new_panel_st7789(lcd_io, &panel_config, &lcd_panel), err, TAG, "New panel failed");
    esp_lcd_panel_reset(lcd_panel);
    esp_lcd_panel_init(lcd_panel);
    esp_lcd_panel_mirror(lcd_panel, true, true);
    esp_lcd_panel_disp_on_off(lcd_panel, true);
    /* LCD backlight on */
    ESP_ERROR_CHECK(gpio_set_level(LCD_GPIO_BL, LCD_BL_ON_LEVEL));

    return ret;

err:
    if (lcd_panel)
    {
        esp_lcd_panel_del(lcd_panel);
    }
    if (lcd_io)
    {
        esp_lcd_panel_io_del(lcd_io);
    }
    spi_bus_free(LCD_SPI_NUM);
    return ret;
}

static esp_err_t bsp_i2c_device_probe(uint8_t addr)
{
    esp_err_t ret = ESP_ERR_NOT_FOUND;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (addr << 1) | I2C_MASTER_WRITE, true);
    i2c_master_stop(cmd);
    if (i2c_master_cmd_begin(TOUCH_I2C_NUM, cmd, 1000) == ESP_OK)
    {
        ret = ESP_OK;
    }
    i2c_cmd_link_delete(cmd);
    return ret;
}

static esp_err_t bsp_touch_init(void)
{
    /* Initialize touch HW */
    const esp_lcd_touch_config_t tp_cfg = {
        .x_max = LCD_H_RES,
        .y_max = LCD_V_RES,
        .rst_gpio_num = GPIO_NUM_NC, // Shared with LCD reset
        .int_gpio_num = TOUCH_GPIO_INT,
        .levels = {
            .reset = 0,
            .interrupt = 0,
        },
        .flags = {
            .swap_xy = 0,
            .mirror_x = 1,
            .mirror_y = 1,
        },
    };
    esp_lcd_panel_io_handle_t tp_io_handle = NULL;
    esp_lcd_panel_io_i2c_config_t tp_io_config = ESP_LCD_TOUCH_IO_I2C_GT911_CONFIG();
    if (ESP_OK == bsp_i2c_device_probe(ESP_LCD_TOUCH_IO_I2C_GT911_ADDRESS_BACKUP))
    {
        tp_io_config.dev_addr = ESP_LCD_TOUCH_IO_I2C_GT911_ADDRESS_BACKUP;
    }

    ESP_RETURN_ON_ERROR(esp_lcd_new_panel_io_i2c((esp_lcd_i2c_bus_handle_t)TOUCH_I2C_NUM, &tp_io_config, &tp_io_handle), TAG, "");
    return esp_lcd_touch_new_i2c_gt911(tp_io_handle, &tp_cfg, &touch_handle);
}

static esp_err_t app_lvgl_init(void)
{
    /* Initialize LVGL */
    const lvgl_port_cfg_t lvgl_cfg = {
        .task_priority = 4,       /* LVGL task priority */
        .task_stack = 4096,       /* LVGL task stack size */
        .task_affinity = -1,      /* LVGL task pinned to core (-1 is no affinity) */
        .task_max_sleep_ms = 500, /* Maximum sleep in LVGL task */
        .timer_period_ms = 5      /* LVGL timer tick period in ms */
    };
    ESP_RETURN_ON_ERROR(lvgl_port_init(&lvgl_cfg), TAG, "LVGL port initialization failed");

    /* Add LCD screen */
    ESP_LOGD(TAG, "Add LCD screen");
    const lvgl_port_display_cfg_t disp_cfg = {
        .io_handle = lcd_io,
        .panel_handle = lcd_panel,
        .buffer_size = LCD_H_RES * LCD_DRAW_BUFF_HEIGHT * sizeof(uint16_t),
        .double_buffer = LCD_DRAW_BUFF_DOUBLE,
        .hres = LCD_H_RES,
        .vres = LCD_V_RES,
        .monochrome = false,
        /* Rotation values must be same as used in esp_lcd for initial settings of the screen */
        .rotation = {
            .swap_xy = false,
            .mirror_x = true,
            .mirror_y = true,
        },
        .flags = {
            .buff_dma = true,
        }};
    lvgl_disp = lvgl_port_add_disp(&disp_cfg);

    /* Add touch input (for selected screen) */
    const lvgl_port_touch_cfg_t touch_cfg = {
        .disp = lvgl_disp,
        .handle = touch_handle,
    };
    lvgl_touch_indev = lvgl_port_add_touch(&touch_cfg);

    return ESP_OK;
}

void bsp_display(void)
{
    /* LCD HW initialization */
    ESP_ERROR_CHECK(bsp_lcd_init());

    /* Touch initialization */
    ESP_ERROR_CHECK(bsp_touch_init());

    // /* Touch initialization */
     ESP_ERROR_CHECK(app_lvgl_init());
}