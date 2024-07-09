#include "bsp_fat.h"

static const char *TAG = "FATFS";

/* �����ļ�ϵͳ·�� */
const char *base_path = "/FAT";

/* ���ؾ����� */
static wl_handle_t s_wl_handle = WL_INVALID_HANDLE;

/* �����ļ���·�� */
const char *file_path = "/FAT/data.txt";

void mount_flash_partition(void)
{
    esp_err_t err;
    ESP_LOGI(TAG, "Mounting FAT filesystem");

    const esp_vfs_fat_mount_config_t mount_config = {
        .max_files = 4,                 /* ���غ���ļ�ϵͳ�����Կ������ļ���*/
        .format_if_mount_failed = true, /* �������ʧ�ܣ���ʽ������ */
        .allocation_unit_size = CONFIG_WL_SECTOR_SIZE};

    /*
    base_path: ���̹���·��
    partition_label: ���ڷ�����λ��,��Ҫ�ڷ����������storage
    mount_config: ���̹�������
    &s_wl_handle: ���ؾ�����
    */
    err = esp_vfs_fat_spiflash_mount(base_path, "storage", &mount_config, &s_wl_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to mount FATFS (%s)", esp_err_to_name(err));
    }
}

void unmount_flash_partition(void)
{
    esp_err_t err = esp_vfs_fat_spiflash_unmount(base_path, s_wl_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to unmount FATFS (%s)", esp_err_to_name(err));
        return;
    }
    else
    {
        ESP_LOGI(TAG, "Unmount FATFS OK");
    }
}

void bsp_fat_hander(void)
{
    FILE *fp = fopen(file_path, "wb"); /* дȨ�ޣ��Զ����Ƶķ�ʽ���ļ� */
    if (fp == NULL)
    {
        ESP_LOGE(TAG, "��ʧ��");
        return;
    }
    int a[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    fwrite(a, sizeof(int), sizeof(a) / sizeof(int), fp);
    fclose(fp);

    // �ٴδ�"/data/test.txt"�ļ�����ȡ����
    fp = fopen(file_path, "rb");
    if (fp == NULL)
    {
        ESP_LOGE(TAG, "��ʧ��");
        return;
    }
    int b[10];
    fread(b, sizeof(int), 10, fp);
    fclose(fp);
    for (int i = 0; i < 10; i++)
    {
        printf("%d\n", b[i]);
    }
    /* ж���ļ�ϵͳ */
    unmount_flash_partition();
}