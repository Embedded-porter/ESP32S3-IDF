#include "includes.h"
/*
*********************************************************************************************************
*                                 					软件版本
命名规则：major(无法向下兼容时增加).minor(新增加功能时增加).patch(修复BUG时增加)
                                    V01.00.00 2024-05-28：开始搭建ESP32软件框架,基于IDF
                                    V01.01.00 2024-06-11：添加串口指令


*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                 任务优先级，数值越小优先级越高(2~max)
*********************************************************************************************************
*/
#define APP_CFG_TASK_USB_CDC_PRIO 7
#define APP_CFG_TASK_TCP_PRIO 6
#define APP_CFG_TASK_UART0_PRIO 5
#define APP_CFG_TASK_BUTTON_PRIO 4
#define APP_CFG_TASK_UI_PRIO 3
#define APP_CFG_TASK_LED_PRIO 2
/*
*********************************************************************************************************
*                                    任务栈大小，单位字节
*********************************************************************************************************
*/
#define APP_CFG_TASK_USB_CDC_STK_SIZE 4096
#define APP_CFG_TASK_TCP_STK_SIZE 4096
#define APP_CFG_TASK_UART0_STK_SIZE 4096
#define APP_CFG_TASK_BUTTON_STK_SIZE 4096
#define APP_CFG_TASK_LED_STK_SIZE 4096
#define APP_CFG_TASK_UI_STK_SIZE 4096

/*
*********************************************************************************************************
*                                       任务句柄变量
*********************************************************************************************************
*/
static TaskHandle_t xHandleTaskUSB_CDC = NULL;
static TaskHandle_t xHandleTaskTCP = NULL;
static TaskHandle_t xHandleTaskUART0 = NULL;
static TaskHandle_t xHandleTaskBUTTON = NULL;
static TaskHandle_t xHandleTaskUI = NULL;
static TaskHandle_t xHandleTaskLED = NULL;
/*
*********************************************************************************************************
*                                      系统函数声明(不用动)
*********************************************************************************************************
*/
/* 互斥信号量保护printf */
static void App_Printf(const char *fmt, ...);
/* 线程创建函数 */
static void AppTaskCreate(void);
/* 线程资源创建函数 */
static void AppObjCreate(void);
/* 输出系统运行状态 */
static void vTaskTaskRunInfo(void);
/* 获取当地时间 */
static void AppGetTime(void);

/*
*********************************************************************************************************
*                                      线程函数声明(在下面添加)
*********************************************************************************************************
*/
static void vTaskUSB_CDC(void *pvParameters);
static void vTaskTCP(void *pvParameters);
static void vTaskUART0(void *pvParameters);
static void vTaskBUTTON(void *pvParameters);
static void vTaskUI(void *pvParameters);
static void vTaskLED(void *pvParameters);

/*
*******************************************************************************************************
*                                         RTOS资源变量
*******************************************************************************************************
*/
static SemaphoreHandle_t AppPrintfSemp; /* 用于printf互斥 */

QueueHandle_t ButtonQueueHandle = NULL; /* 定义一个按键消息队列 */
#define ButtonQueueSzie 10              /* 按键消息队列长度 */

extern QueueHandle_t Uart0QueueHandle; /* 定义一个串口0消息队列 */

extern EventGroupHandle_t s_wifi_event_group; /* 定义一个wifi事件组 */

/*
*******************************************************************************************************
*                                          普通全局变量
*******************************************************************************************************
*/
static const char *TAG = "Info";
static const char *RTOS_TAG = "RTOS";
static const char *UART0_TAG = "UART0";
static const char *TCP_TAG = "TCP";

uint8_t g_SendBuf[1024]; /* 串口发送数组 */

/*
*********************************************************************************************************
*	函 数 名: app_main
*	功能说明: 操作系统函数入口
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void app_main(void)
{
    /* 硬件初始化 */
    bsp_Init();

    /* 创建线程资源 */
    AppObjCreate();

    /* 创建任务 */
    AppTaskCreate();
}

/*
*********************************************************************************************************
*	函 数 名: AppObjCreate
*	功能说明: 创建线程资源函数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void AppObjCreate(void)
{
    /* 创建App_Printf互斥信号量 */
    AppPrintfSemp = xSemaphoreCreateMutex();
    if (AppPrintfSemp == NULL)
    {
        ESP_LOGE(RTOS_TAG, "AppPrintfSemp creation failed\r\n");
    }
    /* 创建ADC按键消息队列 */
    ButtonQueueHandle = xQueueCreate(ButtonQueueSzie, sizeof(int));
    if (ButtonQueueHandle == NULL)
    {
        ESP_LOGE(RTOS_TAG, "ButtonQueueHandle creation failed\r\n");
    }
}

/*
*********************************************************************************************************
*	函 数 名: AppTaskCreate
*	功能说明: 创建应用任务(任务优先级从高到低创建)
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void AppTaskCreate(void)
{
    // xTaskCreate(vTaskUSB_CDC,                  /* 任务函数  */
    //             "vTaskUSB_CDC",                /* 任务名    */
    //             APP_CFG_TASK_USB_CDC_STK_SIZE, /* 任务栈大小，单位word，也就是4字节 */
    //             NULL,                          /* 任务参数  */
    //             APP_CFG_TASK_USB_CDC_PRIO,     /* 任务优先级*/
    //             &xHandleTaskUSB_CDC);          /* 任务句柄  */

    xTaskCreate(vTaskTCP,                  /* 任务函数  */
                "vTaskTCP",                /* 任务名    */
                APP_CFG_TASK_TCP_STK_SIZE, /* 任务栈大小，单位word，也就是4字节 */
                NULL,                      /* 任务参数  */
                APP_CFG_TASK_TCP_PRIO,     /* 任务优先级*/
                &xHandleTaskTCP);          /* 任务句柄  */

    xTaskCreate(vTaskUART0,                  /* 任务函数  */
                "vTaskUART0",                /* 任务名    */
                APP_CFG_TASK_UART0_STK_SIZE, /* 任务栈大小，单位word，也就是4字节 */
                NULL,                        /* 任务参数  */
                APP_CFG_TASK_UART0_PRIO,     /* 任务优先级*/
                &xHandleTaskUART0);          /* 任务句柄  */

    xTaskCreate(vTaskBUTTON,                  /* 任务函数  */
                "vTaskBUTTON",                /* 任务名    */
                APP_CFG_TASK_BUTTON_STK_SIZE, /* 任务栈大小，单位word，也就是4字节 */
                NULL,                         /* 任务参数  */
                APP_CFG_TASK_BUTTON_PRIO,     /* 任务优先级*/
                &xHandleTaskBUTTON);          /* 任务句柄  */

    xTaskCreate(vTaskUI,                  /* 任务函数  */
                "vTaskUI",                /* 任务名    */
                APP_CFG_TASK_UI_STK_SIZE, /* 任务栈大小，单位word，也就是4字节 */
                NULL,                     /* 任务参数  */
                APP_CFG_TASK_UI_PRIO,     /* 任务优先级*/
                &xHandleTaskUI);          /* 任务句柄  */

    xTaskCreate(vTaskLED,                  /* 任务函数  */
                "vTaskLED",                /* 任务名    */
                APP_CFG_TASK_LED_STK_SIZE, /* 任务栈大小，单位word，也就是4字节 */
                NULL,                      /* 任务参数  */
                APP_CFG_TASK_LED_PRIO,     /* 任务优先级*/
                &xHandleTaskLED);          /* 任务句柄  */
}

/*
*********************************************************************************************************
*	函 数 名: vTaskUSB_CDC
*	功能说明: USB_CDC线程
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*********************************************************************************************************
*/
static void vTaskUSB_CDC(void *pvParameters)
{
    while (1)
    {
        vTaskDelay((100) / portTICK_PERIOD_MS);
    }
}


/*
*********************************************************************************************************
*	函 数 名: vTaskTCP
*	功能说明: TCP_CLIENT线程
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*********************************************************************************************************
*/
#define HOST_IP_ADDR "192.168.184.22"
#define port 4001
static const char *payload = "Message from ESP32 ";

void vTaskTCP(void *pvParameters)
{
    /* 等待wifi连接成功才创建TCP CLIENT */
    xEventGroupWaitBits(s_wifi_event_group, WIFI_CONNECTED_BIT, pdFALSE, pdFALSE, portMAX_DELAY);
    char rx_buffer[128];
    int addr_family = 0;
    int ip_protocol = 0;
    char host_ip[] = HOST_IP_ADDR;
    struct sockaddr_in dest_addr;

    inet_pton(AF_INET, host_ip, &dest_addr.sin_addr); /* 将点分十进制的ip地址转化为用于网络传输的数值格式 */
    dest_addr.sin_family = AF_INET;                   /* 选择协议族 */
    dest_addr.sin_port = htons(port);                 /* 端口号 */
    addr_family = AF_INET;                            /* 选择协议族 */
    ip_protocol = IPPROTO_TCP;                        /* 选择TCP协议 */

    /* 创建TCP SOCKET */
    int sock = socket(addr_family, SOCK_STREAM, ip_protocol);
    if (sock < 0)
    {
        ESP_LOGE(TCP_TAG, "Unable to create socket: errno %d", errno);
    }
    ESP_LOGI(TCP_TAG, "Socket created, connecting to %s:%d", host_ip, port);

    /* 连接服务器 */
    int err = connect(sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if (err != 0)
    {
        ESP_LOGE(TCP_TAG, "Socket unable to connect: errno %d", errno);
    }
    ESP_LOGI(TCP_TAG, "Successfully connected");

    while (1)
    {
        /* 发送到服务器 */
        int err = send(sock, payload, strlen(payload), 0);
        if (err < 0)
        {
            ESP_LOGE(TCP_TAG, "Error occurred during sending: errno %d", errno);
            break;
        }

        /* 接收来自服务器的数据 */
        int len = recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0);
        if (len < 0)
        {
            ESP_LOGE(TCP_TAG, "recv failed: errno %d", errno);
            break;
        }
        else
        {
            rx_buffer[len] = 0; // Null-terminate whatever we received and treat like a string
            ESP_LOGI(TCP_TAG, "Received %d bytes from %s:", len, host_ip);
            ESP_LOGI(TCP_TAG, "%s", rx_buffer);
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

/*
*********************************************************************************************************
*	函 数 名: vTaskUART0
*	功能说明: 串口处理线程
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*********************************************************************************************************
*/
static void vTaskUART0(void *pvParameters)
{
    uint32_t send_len;
    int16_t pos_pattern;
    uart_event_t event;
    uint8_t *dtmp = (uint8_t *)malloc(RD_BUF_SIZE);
    while (1)
    {
        /*阻塞等待消息事件队列 */
        if (xQueueReceive(Uart0QueueHandle, (void *)&event, (TickType_t)portMAX_DELAY))
        {
            bzero(dtmp, RD_BUF_SIZE); /* 清空接收缓冲区 */
            switch (event.type)
            {
            /* 数据事件 */
            case UART_DATA:
                /* 读取接收到的数据 */
                uart_read_bytes(EX_UART_NUM, dtmp, event.size, portMAX_DELAY);
                receive_command(dtmp, g_SendBuf, &send_len);
                if (send_len != 0)
                {
                    if (!(strcmp(g_SendBuf, "<C01_[HELP]>") == 0 || strcmp(g_SendBuf, "<C01_[HELP_ALL]>") == 0))
                    {
                        uart_write_bytes(EX_UART_NUM, (const char *)g_SendBuf, send_len);
                    }
                }
                break;
            /* UART FIFO溢出事件 */
            case UART_FIFO_OVF:
                ESP_LOGI(UART0_TAG, "hw fifo overflow");
                uart_flush_input(EX_UART_NUM);
                xQueueReset(Uart0QueueHandle);
                break;
            /* UART环形缓冲区已满事件 */
            case UART_BUFFER_FULL:
                ESP_LOGI(UART0_TAG, "ring buffer full");
                uart_flush_input(EX_UART_NUM);
                xQueueReset(Uart0QueueHandle);
                break;
            /* UART中断事件 */
            case UART_BREAK:
                ESP_LOGI(UART0_TAG, "uart rx break");
                break;
                /* UART奇偶检查错误 */
            case UART_PARITY_ERR:
                ESP_LOGI(UART0_TAG, "uart parity error");
                break;
                /* UART RX帧错误事件 */
            case UART_FRAME_ERR:
                ESP_LOGI(UART0_TAG, "uart frame error");
                break;
            /*  UART 输入样式检测事件 */
            case UART_PATTERN_DET:
                /* 获取样式字符在接收到数据中的位置 */
                pos_pattern = uart_pattern_pop_pos(EX_UART_NUM);
                ESP_LOGI(UART0_TAG, "uart pattern detected at pos : %d", pos_pattern);
                if (pos_pattern == -1)
                {
                    uart_flush_input(EX_UART_NUM);
                }
                else
                {
                    /* 读取接收到的数据 */
                    uart_read_bytes(EX_UART_NUM, dtmp, event.size, portMAX_DELAY);
                    /* 写入(发送)接收到的数据 */
                    uart_write_bytes(EX_UART_NUM, (const char *)dtmp, event.size);
                }
                break;
            default:
                ESP_LOGI(UART0_TAG, "uart event type: %d", event.type);
                break;
            }
        }
    }
    free(dtmp);
    dtmp = NULL;
    vTaskDelete(NULL);
}

/*
*********************************************************************************************************
*	函 数 名: vTaskBUTTON
*	功能说明: ADC按键线程
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*********************************************************************************************************
*/
static void vTaskBUTTON(void *pvParameters)
{
    int btn_index = 0;

    while (1)
    {
        /* 阻塞等待按键信号量 */
        if (xQueueReceive(ButtonQueueHandle, &btn_index, portMAX_DELAY) == pdPASS)
        {
            switch (btn_index)
            {
            case BSP_BUTTON_REC:
                printf("Button REC\r\n");
                break;
            case BSP_BUTTON_MUTE:
                printf("Button MUTE\r\n");
                break;
            case BSP_BUTTON_SET:
                printf("Button SET\r\n");
                break;
            case BSP_BUTTON_PLAY:
                vTaskTaskRunInfo(); /*获取RTOS运行信息 */
                break;
            case BSP_BUTTON_VOLDOWN:
                AppGetTime(); /* 获取当前时间 */
                break;
            case BSP_BUTTON_VOLUP:
                esp_io_expander_print_state(io_expander); /* 获取tca9554芯片扩展引脚的状态 */
                break;
            default:
                printf("Button Unknow\r\n");
                break;
            }
        }
        btn_index = 0;
        vTaskDelay((500) / portTICK_PERIOD_MS);
    }
}

/*
*********************************************************************************************************
*	函 数 名: vTaskUI
*	功能说明: LVGL_UI界面显示线程
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*********************************************************************************************************
*/
static void vTaskUI(void *pvParameters)
{
    while (1)
    {
        vTaskDelay((500) / portTICK_PERIOD_MS);
    }
}

/*
*********************************************************************************************************
*	函 数 名: vTaskLED
*	功能说明: LED闪烁
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*********************************************************************************************************
*/
static void vTaskLED(void *pvParameters)
{
    while (1)
    {
        esp_io_expander_set_level(io_expander, IO_EXPANDER_PIN_NUM_7, 0);
        vTaskDelay((100) / portTICK_PERIOD_MS);
        esp_io_expander_set_level(io_expander, IO_EXPANDER_PIN_NUM_7, 1);
        vTaskDelay((100) / portTICK_PERIOD_MS);
    }
}

/*
*********************************************************************************************************
*	函 数 名: App_Printf
*	功能说明: 线程安全的printf方式
*	形    参: 同printf的参数。
*             在C中，当无法列出传递函数的所有实参的类型和数目时,可以用省略号指定参数表
*	返 回 值: 无
*********************************************************************************************************
*/
static void App_Printf(const char *fmt, ...)
{
    char buf_str[200 + 1]; /* 特别注意，如果printf的变量较多，注意此局部变量的大小是否够用 */
    va_list v_args;

    va_start(v_args, fmt);
    (void)vsnprintf((char *)&buf_str[0],
                    (size_t)sizeof(buf_str),
                    (char const *)fmt,
                    v_args);
    va_end(v_args);

    /* 互斥操作 */
    xSemaphoreTake(AppPrintfSemp, portMAX_DELAY); // 获取互斥信号量

    printf("%s", buf_str);

    xSemaphoreGive(AppPrintfSemp); /* 释放互斥信号量 */
}

/*
*********************************************************************************************************
*	函 数 名: vTaskTaskRunInfo
*	功能说明: 输出系统运行信息
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void vTaskTaskRunInfo(void)
{
    uint8_t CPU_RunInfo[512] = {0};
    memset(CPU_RunInfo, 0, sizeof(CPU_RunInfo));

    ESP_LOGI("RTOS_TAG", "===============================================================\r\n");
    App_Printf("任务名      任务状态 优先级   剩余栈 任务序号\r\n");
    vTaskList((char *)&CPU_RunInfo);
    App_Printf("%s\r\n", CPU_RunInfo);
    App_Printf("\r\n任务名       运行计数         使用率\r\n");
    vTaskGetRunTimeStats((char *)&CPU_RunInfo);
    App_Printf("%s\r\n", CPU_RunInfo);
    // 打印剩余ram和堆容量
    App_Printf("IDLE: ****free internal ram %d  all heap size: %d Bytes****\n", heap_caps_get_free_size(MALLOC_CAP_INTERNAL), heap_caps_get_free_size(MALLOC_CAP_8BIT));
    App_Printf("IDLE: ****free SPIRAM size: %d Bytes****\n", heap_caps_get_free_size(MALLOC_CAP_SPIRAM));
    ESP_LOGI("RTOS_TAG", "===============================================================\r\n");
}
/*
*********************************************************************************************************
*	函 数 名: AppGetTime
*	功能说明: 获取当前时间,分辨率为1s
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void AppGetTime(void)
{
    static struct tm timeinfo = {0}; // 时间寄存器
    time_t now = 0;

    time(&now);
    localtime_r(&now, &timeinfo);

    /* 打印获取到的时间 */
    char str[64];
    strftime(str, sizeof(str), "%c", &timeinfo);
    ESP_LOGI(TAG, "time updated: %s", str);

    ESP_LOGI(TAG, "%d%d:%d%d", timeinfo.tm_hour / 10, timeinfo.tm_hour % 10, timeinfo.tm_min / 10, timeinfo.tm_min % 10);
    ESP_LOGI(TAG, "%d-%d-%d", timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday);

    switch (timeinfo.tm_wday)
    {
    case 0:
        ESP_LOGI(TAG, "星期日");
        break;
    case 1:
        ESP_LOGI(TAG, "星期一");
        break;
    case 2:
        ESP_LOGI(TAG, "星期二");
        break;
    case 3:
        ESP_LOGI(TAG, "星期三");
        break;
    case 4:
        ESP_LOGI(TAG, "星期四");
        break;
    case 5:
        ESP_LOGI(TAG, "星期五");
        break;
    case 6:
        ESP_LOGI(TAG, "星期六");
        break;
    default:
        break;
    }
}