#ifndef _OLP_COMMAND_H
#define _OLP_COMMAND_H

#include "includes.h"

/**********************
 * 命令匹配结构体
 ***********************/
typedef struct FB_CmdSt
{
    int16_t id;
    uint8_t *st_cmdstr;                                                        // 命令字符
    uint16_t ATTR;                                                             // 属性： 0厂家使用，1客户使用,2调试使用
    uint8_t *help_str;                                                         // 帮助字符
    uint16_t (*Proc)(uint8_t *rec_ptr, uint8_t *send_ptr, uint32_t *send_len); // 命令指针函数
} FBCmdSt_t;
/* *******************************
函数名：
描述：打印帮助文档，单独输出函数，内部直接打印，无需装入buff,因为数据繁多，占用RAM
********************************** */
static uint16_t CmdHelp_Proc(uint8_t *rec_ptr, uint8_t *send_ptr, uint32_t *send_len);
static uint16_t CmdHelpAll_Proc(uint8_t *rec_ptr, uint8_t *send_ptr, uint32_t *send_len);

/*******************************
描述：字符串处理函数
***********************************/
/* 字符串对比函数 */
static int FB_StrCmp(char *buf1, char *buf);

/* 命令匹配函数 */
static int16_t FB_CmdRecognition(uint8_t *rec_ptr, uint8_t *send_ptr, uint32_t *send_len);

/* 匹配处理函数 */
static void Cmd_all_receive(uint8_t *rec_ptr, uint8_t *send_ptr, uint32_t *send_len);

/* 返回指令函数 */
void receive_command(uint8_t *rec_ptr, uint8_t *send_ptr, uint32_t *send_len);

#endif // !_OLP_COMMAND_H