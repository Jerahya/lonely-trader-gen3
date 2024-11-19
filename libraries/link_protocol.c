#include "link_protocol.h"
#include "hardware/clocks.h"
#include "gbacom.pio.h"

struct PIOService pioService = {
    .exchangeData   = pio_exchange_data,
    .getBlockIndex  = pio_get_block_index,
    .pio            = pio0,
    .sm             = 0
};

void pio_initialize(uint timeout, void (*timoutCallback)(void)) {
    pioService.timeout = (timeout <= 0 ? 1000 : timeout) * 1000;
    pioService.timoutCallback = timoutCallback;

    gpio_init(gbacom_SC_PIN);
    gpio_set_dir(gbacom_SC_PIN, GPIO_IN);
    gpio_pull_up(gbacom_SC_PIN);

    gpio_init(gbacom_SO_PIN);
    gpio_set_dir(gbacom_SO_PIN, GPIO_IN);
    gpio_pull_up(gbacom_SO_PIN);

    pioService.offset = pio_add_program(pioService.pio, &gbacom_program);
    gbacom_program_init(pioService.pio, pioService.sm, pioService.offset, gbacom_SD_PIN, CLK_DIV);
    
    printf("Loaded program at %u on pio %u\n", pioService.offset, PIO_NUM(pioService.pio));
}

uint16_t pio_exchange_data(uint16_t out_data) {
    pio_sm_put(pioService.pio, pioService.sm, pio_construct_send_data(out_data));
    while (pio_sm_is_tx_fifo_empty(pioService.pio, pioService.sm)) tight_loop_contents();
    uint timout_counter = 0;
    while (pio_sm_is_rx_fifo_empty(pioService.pio, pioService.sm)) {
        tight_loop_contents();
        if (pioService.connected)  {
            if (timout_counter++ < pioService.timeout) continue;
            pioService.timer = 0;
            if (pioService.timoutCallback != NULL) {
                pioService.timoutCallback();
            }
        } else {
            //TODO: implement disconnected time out
        }
    };
    pioService.timer += 1;
    return pio_sm_get(pioService.pio, pioService.sm) >> 16;
}

static uint32_t pio_construct_send_data(uint16_t num) {
    if (num == 0)
        return 0b00000000000000100000000000000000;

    uint32_t result = (num << 1) | (1 << 17) ;
    return result;
}

static uint8_t pio_get_block_index() {
    return pioService.timer % 9;
}


