#ifndef _BSP_FAT_H
#define _BSP_FAT_H

#include "bsp.h"

/*
*********************************************************************************************************
*	�� �� ��: mount_flash_partition
*	����˵��: ����FAT�ļ�ϵͳ
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void mount_flash_partition(void);

/*
*********************************************************************************************************
*	�� �� ��: unmount_flash_partition
*	����˵��: ж��FAT�ļ�ϵͳ
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void unmount_flash_partition(void);


/*
*********************************************************************************************************
*	�� �� ��: bsp_fat_hander
*	����˵��: FAT�ļ�ϵͳ��������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_fat_hander(void);

#endif