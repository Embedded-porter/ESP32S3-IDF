#include "bsp_fat.h"

static const char *TAG = "FATFS";

/* 挂载文件系统路径 */
const char *base_path = "/FAT";

/* 负载均衡句柄 */
static wl_handle_t s_wl_handle = WL_INVALID_HANDLE;

/* 操作文件的路径 */
const char *file_path = "/FAT/data.txt";

void mount_flash_partition(void)
{
    esp_err_t err;
    ESP_LOGI(TAG, "Mounting FAT filesystem");

    const esp_vfs_fat_mount_config_t mount_config = {
        .max_files = 4,                 /* 挂载后的文件系统最大可以开启的文件数*/
        .format_if_mount_failed = true, /* 如果挂载失败，格式化磁盘 */
        .allocation_unit_size = CONFIG_WL_SECTOR_SIZE};

    /*
    base_path: 磁盘挂载路径
    partition_label: 挂在分区表位置,需要在分区表中添加storage
    mount_config: 磁盘挂载配置
    &s_wl_handle: 负载均衡句柄
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
    FILE *fp = fopen(file_path, "wb"); /* 写权限，以二进制的方式打开文件 */
    if (fp == NULL)
    {
        ESP_LOGE(TAG, "打开失败");
        return;
    }
    int a[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    fwrite(a, sizeof(int), sizeof(a) / sizeof(int), fp);
    fclose(fp);

    // 再次打开"/data/test.txt"文件，读取内容
    fp = fopen(file_path, "rb");
    if (fp == NULL)
    {
        ESP_LOGE(TAG, "打开失败");
        return;
    }
    int b[10];
    fread(b, sizeof(int), 10, fp);
    fclose(fp);
    for (int i = 0; i < 10; i++)
    {
        printf("%d\n", b[i]);
    }
    /* 卸载文件系统 */
    unmount_flash_partition();
}