#ifndef TRADER_H
#define TRADER_H

#include "custom_types.h"
#include "queue_data.h"

struct LinkState {
    bool8 master_handshaked;
    bool8 timer_running;
    bool8 is_trading_screen;
    bool8 save_state;
    uint8_t part_sent_index;
    uint16_t slot_selected;
    uint16_t trade_state;
    uint16_t state;
    uint16_t last_state;
    uint16_t link_state;
    uint16_t block_req;
    uint16_t in_data;
    uint16_t out_data;
    uint16_t read_party_counter;
    Queue q;
    QueueData queue_data;
    bool8 has_queue;
    uint8_t block_index;
    uint16_t received_block[8];
    void (*Init)(void);
    void (*Run)(void);
};


extern struct PokemonParty playerParty;
extern struct LinkState gLink;

static void trd_initialize();
static void trd_run();
static void trd_process_command();
static void trd_start_timer();
static void trd_wait_for_master();
static void trd_update_link_state();
static void trd_send_slot_select();
static void trd_check_callback();
static void trd_trade_action();
static void trd_send_ready_finish();
static void trd_close_link();
static void trd_swap_pokemon_to_slot();
static void trd_read_player_party();


void trd_end_link();
void trd_timeout_callback();


#endif