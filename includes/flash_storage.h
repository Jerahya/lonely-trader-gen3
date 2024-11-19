#ifndef FLASHM_H
#define FLASHM_H

#define USED_SECTOR         4
#define USED_SECTOR_SIZE    (FLASH_SECTOR_SIZE * USED_SECTOR)
#define USED_PAGE_COUNT     (USED_SECTOR_SIZE / FLASH_PAGE_SIZE)
#define FLASH_TARGET_OFFSET (PICO_FLASH_SIZE_BYTES - USED_SECTOR_SIZE)
#define MEMORY_OFFSET       (XIP_BASE + FLASH_TARGET_OFFSET)

bool flash_read_data(uint16_t *des, size_t max_size);
bool flash_save_data(uint8_t *data, size_t sz);
void flash_erase_used_sectors();
void flash_print_saved_data(size_t sz);

#endif