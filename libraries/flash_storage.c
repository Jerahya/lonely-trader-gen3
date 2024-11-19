#include "pico/stdlib.h"
#include <hardware/flash.h>
#include <stdio.h>
#include "flash_storage.h"
#include "utilities.h"
#include "pico/critical_section.h"

bool flash_read_data(uint16_t *des, size_t max_size) {
    uint16_t *data = NULL;
    bool found = true;

    for (int i = 0; i < USED_PAGE_COUNT; i++) {
        uint32_t *p = (uint32_t *)(MEMORY_OFFSET + (i * FLASH_PAGE_SIZE));

        if (p[0] == 0xFFFFFFFF && p[1] == 0xFFFFFFFF) {
            found = (i > 0);
            break;
        }

        data = (uint16_t *)p;
    }

    if (found && data != NULL) {
        printf("found saved data\n");
        size_t copy_size = (max_size < FLASH_PAGE_SIZE) ? max_size : FLASH_PAGE_SIZE;
        memcpy(des, data, copy_size);
    }

    return found;
}

bool flash_save_data(uint8_t *data, size_t sz) {
    int currentSector = 0;
    int sectorIndex = 1;
    for(int sec = 0; sec < USED_SECTOR; sec ++) {
        uint32_t *p;
        p = (uint32_t *)(MEMORY_OFFSET + (sec * FLASH_SECTOR_SIZE) + (FLASH_SECTOR_SIZE - FLASH_PAGE_SIZE)); //check last value of each sector;
        if (p[0] == 0xFFFFFFFF && p[1] == 0xFFFFFFFF) {
            currentSector = sec; //use who's last page is still empty
            for(int i = 0; i < USED_PAGE_COUNT; i++) {
                p = (uint32_t *)(MEMORY_OFFSET + ((sec * i) * FLASH_PAGE_SIZE));
                if (p[0] == 0xFFFFFFFF && p[1] == 0xFFFFFFFF) {
                    sectorIndex = i;
                    break;
                }
            }
            break;
        } else {
            if (sec == USED_SECTOR - 1)
                currentSector = 0;//use 1st sector if all sectors are used
        }
    }
    if (currentSector == 0 && sectorIndex == 0) {
        flash_erase_used_sectors();
    }

    int target_addr = FLASH_TARGET_OFFSET + (currentSector * FLASH_SECTOR_SIZE) + (sectorIndex * FLASH_PAGE_SIZE);
    uint8_t buffer[FLASH_PAGE_SIZE] = {0};

    memcpy(buffer, data, sz);

    uint32_t ints = save_and_disable_interrupts();
    flash_range_program(target_addr, (uint8_t *)buffer, FLASH_PAGE_SIZE);
    restore_interrupts (ints);
    
    printf("data saved.\n");
}

void flash_erase_used_sectors() {
    printf("Erasing used flash sector...\n");
    flash_range_erase(FLASH_TARGET_OFFSET, USED_SECTOR_SIZE);
    printf("Flash erase complete.\n");
}

void flash_print_saved_data(size_t sz) {
    uint8_t data[sz];
    memset(data, 0x0, sizeof(data));
    if (flash_read_data((uint16_t *)data, sz)) {
        print_data(data, sz);
    }
}
