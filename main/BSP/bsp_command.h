#ifndef _OLP_COMMAND_H
#define _OLP_COMMAND_H

#include "includes.h"

/**********************
 * ����ƥ��ṹ��
 ***********************/
typedef struct FB_CmdSt
{
    int16_t id;
    uint8_t *st_cmdstr;                                                        // �����ַ�
    uint16_t ATTR;                                                             // ���ԣ� 0����ʹ�ã�1�ͻ�ʹ��,2����ʹ��
    uint8_t *help_str;                                                         // �����ַ�
    uint16_t (*Proc)(uint8_t *rec_ptr, uint8_t *send_ptr, uint32_t *send_len); // ����ָ�뺯��
} FBCmdSt_t;
/* *******************************
��������
��������ӡ�����ĵ�����������������ڲ�ֱ�Ӵ�ӡ������װ��buff,��Ϊ���ݷ��࣬ռ��RAM
********************************** */
static uint16_t CmdHelp_Proc(uint8_t *rec_ptr, uint8_t *send_ptr, uint32_t *send_len);
static uint16_t CmdHelpAll_Proc(uint8_t *rec_ptr, uint8_t *send_ptr, uint32_t *send_len);

/*******************************
�������ַ���������
***********************************/
/* �ַ����ԱȺ��� */
static int FB_StrCmp(char *buf1, char *buf);

/* ����ƥ�亯�� */
static int16_t FB_CmdRecognition(uint8_t *rec_ptr, uint8_t *send_ptr, uint32_t *send_len);

/* ƥ�䴦���� */
static void Cmd_all_receive(uint8_t *rec_ptr, uint8_t *send_ptr, uint32_t *send_len);

/* ����ָ��� */
void receive_command(uint8_t *rec_ptr, uint8_t *send_ptr, uint32_t *send_len);

#endif // !_OLP_COMMAND_H