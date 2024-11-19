#ifndef UTILITIES_H
#define UTILITIES_H
#include <stdio.h>
#include "pico/stdlib.h"
#include <string.h>

void clear_array(uint16_t *array, size_t size);
uint16_t final_checksum(uint16_t master, uint16_t slave);
uint16_t checksum(const uint16_t *src);
void copy_data(uint16_t *des, uint16_t *src, size_t sz);
uint16_t checksum_command(const uint16_t *src, uint16_t command, uint8_t size);
void print_data(const uint8_t *src, size_t sz);

#endif