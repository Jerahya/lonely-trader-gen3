#ifndef LINKPROTOCOL_H
#define LINKPROTOCOL_H

#include "hardware/pio.h"
#include <stdio.h>

//handles communication with the gba in multiplayer mode
//set custom pin in gbacom.pio
//.define public SC_PIN 1
//.define public SO_PIN 2
//.define public SD_PIN 3

#define CLK_DIV 1092 / 8 //found this to work fine :S

struct PIOService {
    PIO pio;
    uint offset;
    uint sm;
    uint timeout;
    uint32_t timer;
    bool connected;
    void (*timoutCallback)(void);
    uint16_t (*exchangeData)(uint16_t out);
    uint8_t (*getBlockIndex)();
};

extern struct PIOService pioService;

void pio_initialize(uint timeout, void (*timoutCallback)(void));
uint16_t pio_exchange_data(uint16_t out_data);
static uint8_t pio_get_block_index();
static uint32_t pio_construct_send_data(uint16_t num);

#endif