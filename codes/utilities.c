#include "utilities.h"

void clear_array(uint16_t *array, size_t size) {
    memset(array, 0, size * sizeof(uint16_t));
}

uint16_t final_checksum(uint16_t master, uint16_t slave) {
    return master + slave;
}

uint16_t checksum(const uint16_t *src)
{
    uint16_t chksum = 0;
    for (uint16_t i = 0; i < 8; i++)
        chksum += src[i];

    return chksum;
}

void copy_data(uint16_t *des, uint16_t *src, size_t sz) {
    memcpy(des, src, sz);
}

uint16_t checksum_command(const uint16_t *src, uint16_t command, uint8_t size) {
    if (size > 8) return 0;
    
    uint16_t chksum = command;
    for (uint16_t i = 0; i < size; i++)
        chksum += src[i];
    
    return chksum;
}

void print_data(const uint8_t *src, size_t sz) {
    for(int i=0; i < sz; i++)
        printf("%02X ", src[i]);
    printf("\n");
}