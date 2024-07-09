#include "bsp_uart.h"

QueueHandle_t Uart0QueueHandle;/* 定义一个串口0消息队列 */

#define Uart0QueueSzie 20  /* 串口0消息队列事件大小 */
/*
*********************************************************************************************************
*	函 数 名: bsp_Uart0Init
*	功能说明: 串口初始化
*	形    参: buad_rate 波特率
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_Uart0Init(uint32_t buad_rate)
{
    /* Configure parameters of an UART driver,
     * communication pins and install the driver */
    uart_config_t uart_config = {
        .baud_rate = buad_rate,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    // Install UART driver, and get the queue.
    uart_driver_install(EX_UART_NUM, BUF_SIZE * 2, BUF_SIZE * 2, Uart0QueueSzie, &Uart0QueueHandle, 0);
    uart_param_config(EX_UART_NUM, &uart_config);

    // Set UART pins (using UART0 default pins ie no changes.)
    uart_set_pin(EX_UART_NUM, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    // Set uart pattern detect function.
    uart_enable_pattern_det_baud_intr(EX_UART_NUM, '+', PATTERN_CHR_NUM, 9, 0, 0);
    // Reset the pattern queue length to record at most 20 pattern positions.
    uart_pattern_queue_reset(EX_UART_NUM, Uart0QueueSzie);
}
