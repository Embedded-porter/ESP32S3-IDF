#include "bsp_tca9554.h"

/* Touch settings */
#define TCA9554_I2C_NUM (0)

/* LCD touch pins */
#define TCA9554_I2C_SCL (GPIO_NUM_18)
#define TCA9554_I2C_SDA (GPIO_NUM_17)

bool is_tca9554_init = false;
static bool i2c_initialized = false;

static const char *tca9554_tag = "TCA9554";
static const char *i2c_tag = "I2C";

esp_io_expander_handle_t io_expander = NULL;

/*
*********************************************************************************************************
*	函 数 名: bsp_I2CInit
*	功能说明: tca9954扩展芯片I2C总线初始化
*	形    参: 无
*	返 回 值: 初始化是否成功-> ESP_OK表示成功  ESP_FAIL 表示失败
*********************************************************************************************************
*/
static esp_err_t bsp_I2CInit(void)
{
    /* I2C was initialized before */
    if (i2c_initialized)
    {
        return ESP_OK;
    }

    /* Initilize I2C */
    const i2c_config_t i2c_conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = TCA9554_I2C_SDA,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = TCA9554_I2C_SCL,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 400000};

    ESP_RETURN_ON_ERROR(i2c_param_config(TCA9554_I2C_NUM, &i2c_conf), i2c_tag, "I2C configuration failed");
    ESP_RETURN_ON_ERROR(i2c_driver_install(TCA9554_I2C_NUM, i2c_conf.mode, 0, 0, 0), i2c_tag, "I2C initialization failed");
    i2c_initialized = true;
    return ESP_OK;
}

/*
*********************************************************************************************************
*	函 数 名: tca9554_init
*	功能说明: tca9954扩展芯片初始化
*	形    参: 无
*	返 回 值: 初始化是否成功-> 1表示成功  0表示失败
*********************************************************************************************************
*/
uint16_t tca9554_init(void)
{
    esp_err_t ret;
    ret = bsp_I2CInit();
    if (ret != ESP_OK)
    {
        return false;
    }
    if (is_tca9554_init)
    {
        return true;
    }
    // if (ESP_OK == (esp_io_expander_new_i2c_tca9554(0, ESP_IO_EXPANDER_I2C_TCA9554_ADDRESS_000, &io_expander)))
    // {
    //     ESP_LOGI(tca9554_tag, "设备探测成功，无需额外操作");
    // }
    if (ESP_OK == (esp_io_expander_new_i2c_tca9554(0, ESP_IO_EXPANDER_I2C_TCA9554A_ADDRESS_000, &io_expander)))
    {
        ESP_LOGI(tca9554_tag, "tca9554_tag 初始化成功");
    }
    else
    {
        ESP_LOGI(tca9554_tag, "设备探测失败");
        return false;
    }
    esp_io_expander_set_dir(io_expander, IO_EXPANDER_PIN_NUM_7, IO_EXPANDER_OUTPUT);
    esp_io_expander_set_level(io_expander, IO_EXPANDER_PIN_NUM_7, 0);
    is_tca9554_init = true;
    return true;
}