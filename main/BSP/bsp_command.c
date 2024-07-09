#include "includes.h"

static uint8_t s_ucCardNum;

static uint8_t SV[] = "01.01.00";

static uint8_t HV[] = "01.00.00";

typedef enum Command_ID
{
    CMD01,
    CMD02,
    CMD03,

    /*ʼ�շ�������棬��λ����յ�*/
    CMDHELP,
    CMDHELPALL,
    CMDOVER,
} Command_ID;
/* <C01_[RESUME]> */
static uint16_t CMD01_Proc(uint8_t *rec_ptr, uint8_t *send_ptr, uint32_t *send_len)
{
    *send_len += sprintf(send_ptr, "[RESUME_OK]");
    return 0;
}

/* <C01_[SV_?]> */
static uint16_t CMD02_Proc(uint8_t *rec_ptr, uint8_t *send_ptr, uint32_t *send_len)
{
    *send_len += sprintf(send_ptr, "[SV_%s]", SV);
    return 0;
}

/* <C01_[HV_?]> */
static uint16_t CMD03_Proc(uint8_t *rec_ptr, uint8_t *send_ptr, uint32_t *send_len)
{
    *send_len += sprintf(send_ptr, "[HV_%s]", HV);
    return 0;
}

// �������
const FBCmdSt_t cmd_tbl[] =
    {
        {CMD01, "[RESUME]", (1 << 0), "�ָ�����ȱʡ����", CMD01_Proc},
        {CMD02, "[SV_?]", (1 << 0), "��ѯ����汾", CMD02_Proc},
        {CMD03, "[HV_?]", (1 << 0), "��ѯӲ���汾", CMD03_Proc},
        /*ʼ�շ�������棬��λ����յ�*/
        {CMDHELP, "[HELP_?]", (1 << 1), "help document", CmdHelp_Proc},
        {CMDHELPALL, "[HELPALL_?]", (1 << 2), "�����ڲ������ѯ", CmdHelpAll_Proc},
        {CMDOVER, "", 0, "", NULL},
};
/* *******************************
��������
��������ӡ�����ĵ�����������������ڲ�ֱ�Ӵ�ӡ������װ��buff,��Ϊ���ݷ��࣬ռ��RAM
���룺
�����
���أ�
����������
***********************************/
static uint16_t CmdHelp_Proc(uint8_t *rec_ptr, uint8_t *send_ptr, uint32_t *send_len)
{
    uint8_t cmd_ID = 0;

    // ��ӡ����
    printf("HELP:\r\n");
    for (cmd_ID = 0; cmd_ID < (sizeof(cmd_tbl) / sizeof(FBCmdSt_t) - 1); cmd_ID++)
    {
        if (cmd_tbl[cmd_ID].ATTR & 0x0004 != 0) /*���������ӡ*/
        {
            printf("%s", (const char *)cmd_tbl[cmd_ID].st_cmdstr);
            // ��ӡð��
            printf(": ");
            printf("%s", (const char *)cmd_tbl[cmd_ID].help_str);
            printf("\r\n");
        }
    }
    *send_len += sprintf(send_ptr, "[HELP]");
    return 0;
}

/* *******************************
��������
��������ӡ�����ĵ�����������������ڲ�ֱ�Ӵ�ӡ������װ��buff,��Ϊ���ݷ��࣬ռ��RAM
    ��ӡ������������û������������
���룺
�����
���أ�
����������
********************************** */
static uint16_t CmdHelpAll_Proc(uint8_t *rec_ptr, uint8_t *send_ptr, uint32_t *send_len)
{
    uint8_t cmd_ID = 0;

    // ��ӡ����
    printf("HELP:\r\n");
    for (cmd_ID = 0; cmd_ID < (sizeof(cmd_tbl) / sizeof(FBCmdSt_t) - 1); cmd_ID++)
    {

        if (cmd_tbl[cmd_ID].ATTR) /*���������ӡ*/
        {
            printf("%s", (const char *)cmd_tbl[cmd_ID].st_cmdstr);
            // ��ӡð��
            printf(": ");
            printf("%s", (const char *)cmd_tbl[cmd_ID].help_str);
            printf("\r\n");
        }
    }
    *send_len += sprintf(send_ptr, "[HELP_ALL]");
    return 0;
}

/* *******************************
��������
����������ƥ�亯��
���룺rec_ptr���������ݰ�buff,send_ptr�������ݰ�buff,StartCharƥ������ַ������֣�
�����Ӧ���ַ�������ָ��
���أ�>=0,�����ַ���������  <0,ƥ��ʧ�ܣ�
����������
********************************** */
static int16_t FB_CmdRecognition(uint8_t *rec_ptr, uint8_t *send_ptr, uint32_t *send_len)
{
    uint8_t ID = 0;
    for (ID = 0; ID < (sizeof(cmd_tbl) / sizeof(FBCmdSt_t) - 1); ID++) // forѭ����������ṹ��
    {
        if (FB_StrCmp(rec_ptr, cmd_tbl[ID].st_cmdstr) > 0) // �ַ���ƥ��ɹ�
        {
            if (cmd_tbl[ID].Proc != NULL)
            {
                return cmd_tbl[ID].Proc(rec_ptr, send_ptr, send_len); // ����һ������ָ��
            }
        }
    }
    return -1;
}

/***********************************************************
 * �������ƣ�cmdcmp
 * ���룺�����Ƚ��ַ���
 * �����1:�ַ����Ƚϳɹ�  0���ַ����Ƚ�ʧ��
 ****************************************************************/
static int FB_StrCmp(char *buf1, char *buf)
{
    int i, ret;

    ret = 1;

    for (i = 0; i < 200; i++)
    {
        if (buf[i] == 0)
        { // �ַ��������˳��Ƚ�
            break;
        }
        else if (buf[i] == '*')
        { //* �ַ����Ƚ�
            continue;
        }
        else if (buf[i] != buf1[i])
        { // �Ƚ��ַ�����ȷ���0
            ret = 0;
            break;
        }
    }
    return ret;
}

/* *******************************
��������
������ƥ�䴦������������յ�����Ϣrec_ptr
���룺rec_ptr���������ݰ�buff,send_ptr�������ݰ�buff��send_len���͵��ַ�������ָ��
�����
    send_ptr ����õ��ַ���
    send_len�ַ������ȳ���
���أ���
����������
********************************** */
static void Cmd_all_receive(uint8_t *rec_ptr, uint8_t *send_ptr, uint32_t *send_len)
{
    int16_t i, j;
    int16_t count_len;

    send_ptr[0] = '<';
    memcpy(send_ptr + 1, rec_ptr + 1, 4); // ��"<C01_"�ĸ��ַ����Ƶ�send_ptr
    *send_len = 5;
    j = 5;
    while (rec_ptr[j] != '>' && rec_ptr[j] != 0) // ����0������'>'����
    {
        if (rec_ptr[j] != '[')
        {
            j++; // �����ж�rec_ptr[j]�Ƿ�����ַ�'['����������ڣ���j��1��������һ��ѭ����
        }
        else
        {
            count_len = FB_CmdRecognition(&rec_ptr[j], (send_ptr + *send_len), send_len); // �ӵ�������ʼ����
            // δƥ��ʱ
            if (count_len < 0)
            {
                count_len = 0;

                memcpy(send_ptr + *send_len, "[CMD_ER]", 8);
                count_len = 8;
            }
            *send_len += count_len;
            j++;
        }
    }
    send_ptr[*send_len] = '>';
    *send_len += 1;
    send_ptr[*send_len] = 0;
}

/*********************************************************************************************************
** ��������: receive_command
** ��������: ���մ���I2C����
** �䡡��: 	rec_ptr:ָ���������ݶ���
**         	len:��Ч���ݳ���
** �䡡��:	send_ptr:ָ��������ݶ���
**			send_len��������ݳ���
**
** ȫ�ֱ���: ��
** ����ģ��: ��
********************************************************************************************************/

void receive_command(uint8_t *rec_ptr, uint8_t *send_ptr, uint32_t *send_len)
{
    int16_t len; /*��������ĺ���*/

    *send_len = 0;
    if ((rec_ptr[0] == '<') && (rec_ptr[1] == 'C') && (rec_ptr[4]) == '_')
    {
        s_ucCardNum = (rec_ptr[2] - '0') * 10 + rec_ptr[3] - '0'; // �ж������ſ�

        if (s_ucCardNum == 1 || s_ucCardNum == 2)
        {
            Cmd_all_receive(rec_ptr, send_ptr, send_len);
        }
        else
        {
            *send_len = 0;
        }
    }
    else
    {
        len = FB_CmdRecognition(rec_ptr, send_ptr, send_len);
        // ƥ��ʧ��
        if (len < 0)
        {
            len = 0;
            memcpy(send_ptr, "<CMD_ER>", 8);
            len = 8;
        }
        *send_len = len;
    }
}
