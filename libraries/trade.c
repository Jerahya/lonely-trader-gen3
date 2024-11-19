#include <stdio.h>
#include "custom_types.h"
#include "trade.h"
#include "link_protocol.h"
#include "utilities.h"
#include "shared.h"
#include "flash_storage.h"
#include "pico/multicore.h"

struct PokemonParty playerParty = {};
struct LinkState gLink = {
    .Init = trd_initialize, 
    .Run = trd_run, 
    .out_data = 0x0000,
    .state = LINK_STATE_HANDSHAKE
};

static void trd_run() {
    while (true)
    {
        gLink.in_data = pioService.exchangeData(gLink.out_data);
        
        // printf("%04X %04X\n", gLink.in_data, gLink.out_data);
        if (!pioService.connected) {
            gLink.out_data = SLAVE_HANDSHAKE;
            trd_wait_for_master(gLink.in_data);
        } else {
            if (gLink.timer_running) {
                gLink.block_index = pioService.getBlockIndex();
                gLink.out_data = NO_DATA;

                if (gLink.block_index <= 7)
                    gLink.received_block[gLink.block_index] = gLink.in_data;

                switch (gLink.block_index)
                {
                    case 0:
                        trd_update_link_state();
                        break;
                    case 7:
                        gLink.out_data = checksum(gLink.received_block);
                            
                        if (gLink.has_queue) {
                            if (gLink.queue_data.sending) {
                                gLink.out_data = final_checksum(gLink.out_data, 
                                    checksum_command(gLink.queue_data.data + (gLink.queue_data.offset * 7), gLink.queue_data.command, 
                                    gLink.queue_data.size - gLink.queue_data.pos == 0 ? gLink.queue_data.size % 7 : 7));
                                
                                trd_read_player_party();
                                gLink.queue_data.offset++;
                                if (gLink.queue_data.pos >= gLink.queue_data.size) {
                                    gLink.has_queue = false;
                                    trd_check_callback();
                                }
                            } else {
                                if (gLink.queue_data.delay <= 1) {
                                    gLink.queue_data.sending = true;
                                } else {
                                    gLink.queue_data.delay--;
                                }
                            }
                        }

                        trd_process_command();
                        if (!gLink.has_queue && queue_not_empty(&gLink.q)) {
                            queue_dequeue(&gLink.q, &gLink.queue_data);
                            gLink.has_queue = true;
                            gLink.queue_data.sending = gLink.queue_data.delay-- <= 1;
                        }
                        break;
                    case 8:
                        clear_array(gLink.received_block, 8);
                        break;
                    default: break;
                }
                
                if (gLink.has_queue && gLink.queue_data.sending) {
                    uint8_t si = (pioService.timer + 1) % 9;
                    if (si == 0) {
                        gLink.out_data = gLink.queue_data.command;
                    } else if (si <= 7 && gLink.queue_data.pos < gLink.queue_data.size) {
                        gLink.out_data = *(gLink.queue_data.data + gLink.queue_data.pos);
                        gLink.queue_data.pos++;
                    }
                }
            }
        }
    }
}

static void trd_process_command() {
    //response to command
    if (gLink.is_trading_screen) trd_trade_action();
    if (gLink.last_state == gLink.state) return;
     
    gLink.last_state = gLink.state;

    switch (gLink.state)
    {
        case LINKCMD_SEND_LINK_TYPE:
            gLink.link_state = gLink.received_block[1];
            link_type_response.size = sizeof(link_player_block);
            link_player_block.linkPlayer.linkType = gLink.link_state;

            queue_enqueue(&gLink.q, queue_create((uint16_t *)&link_type_response, LINKCMD_INIT_BLOCK, 2, 3, NULL));
            queue_enqueue(&gLink.q, queue_create((uint16_t *)&link_player_block, LINKCMD_CONT_BLOCK, link_type_response.size / 2, 3, gLink.link_state == LINKTYPE_TRADE_DISCONNECTED ? trd_send_ready_finish : NULL));
            break;

        case LINKCMD_SEND_BLOCK_REQ:
            gLink.block_req = gLink.received_block[1];
            
            switch (gLink.block_req)
            {
                case TRAINER_CARD:
                    link_type_response.size = sizeof(trainer_card);
                    queue_enqueue(&gLink.q, queue_create((uint16_t *)&link_type_response, LINKCMD_INIT_BLOCK, 2, 3, NULL));
                    queue_enqueue(&gLink.q, queue_create((uint16_t *)&trainer_card, LINKCMD_CONT_BLOCK, link_type_response.size/2, 3, NULL));
                    break;
                
                case PARTY_SET:
                    if (gLink.part_sent_index < 3) {
                        gLink.read_party_counter = 0;
                        link_type_response.size = 200;
                        queue_enqueue(&gLink.q, queue_create((uint16_t *)&link_type_response, LINKCMD_INIT_BLOCK, 2, 3, NULL));
                        queue_enqueue(&gLink.q, queue_create(((uint16_t *)&tradingParty) + (gLink.part_sent_index * 100), LINKCMD_CONT_BLOCK, link_type_response.size/2, 3, NULL));
                        gLink.part_sent_index++;
                    }
                    break;
                    
                case MAILS:
                    link_type_response.size = 220;
                    queue_enqueue(&gLink.q, queue_create((uint16_t *)&link_type_response, LINKCMD_INIT_BLOCK, 2, 3, NULL));
                    queue_enqueue(&gLink.q, queue_create((uint16_t *)&dummyMailMsg, LINKCMD_CONT_BLOCK, link_type_response.size/2, 3, NULL));
                    break;

                case RIBBONS:
                    link_type_response.size = 40;
                    queue_enqueue(&gLink.q, queue_create((uint16_t *)&link_type_response, LINKCMD_INIT_BLOCK, 2, 3, NULL));
                    queue_enqueue(&gLink.q, queue_create((uint16_t *)&dummy_filler2, LINKCMD_CONT_BLOCK, link_type_response.size/2, 3, trd_send_slot_select));
                    
                    gLink.is_trading_screen = true;
                    break;

                default:
                    break;
            }

            break;

        case LINKCMD_READY_EXIT_STANDBY:
            queue_enqueue(&gLink.q, queue_create((uint16_t *)no_data, LINKCMD_READY_EXIT_STANDBY, 1, 1, NULL));
            gLink.state = 0; //reset state
        break;

        case LINKCMD_SEND_HELD_KEYS:
            //we only need to send LINK_KEY_CODE_READY
            //but it is better to see the other player move
            switch (gLink.received_block[1])
            {
                case KEY_UP:
                    queue_enqueue(&gLink.q, queue_create((uint16_t *)key_up, LINKCMD_SEND_HELD_KEYS, 1, 1, NULL));
                    break;
                case KEY_DOWN:
                    queue_enqueue(&gLink.q, queue_create((uint16_t *)key_down, LINKCMD_SEND_HELD_KEYS, 1, 1, NULL));
                    break;
                case KEY_LEFT:
                    queue_enqueue(&gLink.q, queue_create((uint16_t *)key_right, LINKCMD_SEND_HELD_KEYS, 1, 1, NULL));
                    break;
                case KEY_RIGHT:
                    queue_enqueue(&gLink.q, queue_create((uint16_t *)key_left, LINKCMD_SEND_HELD_KEYS, 1, 1, NULL));
                    break;
                case LINK_KEY_CODE_READY:
                    queue_enqueue(&gLink.q, queue_create((uint16_t *)key_ready, LINKCMD_SEND_HELD_KEYS, 1, 1, NULL));
                    break;
                case LINK_KEY_CODE_EXIT_ROOM:
                    queue_enqueue(&gLink.q, queue_create((uint16_t *)key_exit, LINKCMD_SEND_HELD_KEYS, 1, 1, NULL));
                    break;
                default:
                    queue_enqueue(&gLink.q, queue_create((uint16_t *)key_empty, LINKCMD_SEND_HELD_KEYS, 1, 1, NULL));
                    break;
            }

            break;

        case LINKCMD_READY_CLOSE_LINK:
            no_data[0] = gLink.received_block[1];
            queue_enqueue(&gLink.q, queue_create((uint16_t *)no_data, LINKCMD_READY_CLOSE_LINK, 1, 1, trd_close_link));
            gLink.is_trading_screen = false;
            break;

        default: break;
    }
    
    multicore_fifo_push_blocking(gLink.state);
}

static void trd_read_player_party() {
    if (gLink.state == LINKCMD_INIT_BLOCK && gLink.block_req == PARTY_SET && gLink.received_block[0] == LINKCMD_CONT_BLOCK) {
        multicore_fifo_push_blocking(COPY_DATA);
    }
}

static void trd_swap_pokemon_to_slot() {
    if (gLink.slot_selected >= 0) {
        printf("pokemon swaped to slot %d\n", gLink.slot_selected);
        switch (gLink.slot_selected)
        {
            case 0:
                memcpy(tradingParty.slot0, playerParty.slot0, sizeof(tradingParty.slot0));
                break;
            case 1:
                memcpy(tradingParty.slot0, playerParty.slot1, sizeof(tradingParty.slot0));
                break;
            case 2:
                memcpy(tradingParty.slot0, playerParty.slot2, sizeof(tradingParty.slot0));
                break;
            case 3:
                memcpy(tradingParty.slot0, playerParty.slot3, sizeof(tradingParty.slot0));
                break;
            case 4:
                memcpy(tradingParty.slot0, playerParty.slot4, sizeof(tradingParty.slot0));
                break;
            case 5:
                memcpy(tradingParty.slot0, playerParty.slot5, sizeof(tradingParty.slot0));
                break;
            
            default: break;
        }
        
        print_data((uint8_t *)&tradingParty.slot0, sizeof(tradingParty.slot0));
        flash_save_data((uint8_t *)&tradingParty.slot0, sizeof(tradingParty.slot0));
    }
}

static void trd_send_ready_finish() {
    gLink.is_trading_screen = true;
    link_type_response.size = 20;
    select_slot.cmd_trade = LINKCMD_READY_FINISH_TRADE;
    queue_enqueue(&gLink.q, queue_create((uint16_t *)&link_type_response, LINKCMD_INIT_BLOCK, 2, 1700, NULL));
    queue_enqueue(&gLink.q, queue_create((uint16_t *)&select_slot, LINKCMD_CONT_BLOCK, link_type_response.size/2, 3, NULL));
}

static void trd_trade_action() {
    if (gLink.received_block[0] == LINKCMD_CONT_BLOCK) {
        switch (gLink.received_block[1])
        {
            case LINKCMD_SET_MONS_TO_TRADE:
                gLink.slot_selected = gLink.received_block[2]; //player has selected a slot
                
                link_type_response.size = 20;
                select_slot.cmd_trade = LINKCMD_INIT_BLOCK; //also used to verify trade
                queue_enqueue(&gLink.q, queue_create((uint16_t *)&link_type_response, LINKCMD_INIT_BLOCK, 2, 200, NULL));
                queue_enqueue(&gLink.q, queue_create((uint16_t *)&select_slot, LINKCMD_CONT_BLOCK, link_type_response.size/2, 3, NULL));

                break;

            case LINKCMD_CONFIRM_FINISH_TRADE:
                gLink.is_trading_screen = false;
                gLink.save_state = false;
                break;

            case LINKCMD_READY_FINISH_TRADE: break;

            case LINKCMD_INIT_BLOCK: break;
            
            case LINKCMD_START_TRADE: 
                gLink.save_state = true;
                break;

            case LINKCMD_PLAYER_CANCEL_TRADE: break;

            case LINKCMD_REQUEST_CANCEL:
                link_type_response.size = 20;
                
                select_slot.cmd_trade = LINKCMD_REQUEST_CANCEL;
                queue_enqueue(&gLink.q, queue_create((uint16_t *)&link_type_response, LINKCMD_INIT_BLOCK, 2, 100, NULL));
                queue_enqueue(&gLink.q, queue_create((uint16_t *)&select_slot, LINKCMD_CONT_BLOCK, link_type_response.size/2, 3, NULL));
                
                break;
            case LINKCMD_BOTH_CANCEL_TRADE:
                gLink.slot_selected = -1;
                gLink.last_state = 0;
                gLink.state = 0;
                gLink.is_trading_screen = false;
                break;

            default: break;
        }
        
        gLink.trade_state = gLink.received_block[1];
    }

    gLink.last_state = 0; //continues states
}

static void trd_check_callback() {
    if (gLink.queue_data.callBack != NULL) {
        gLink.queue_data.callBack();
        gLink.queue_data.callBack = NULL;
    }
}

static void trd_send_slot_select() {
    link_type_response.size = 20;
    select_slot.cmd_trade = LINKCMD_READY_TO_TRADE;
    queue_enqueue(&gLink.q, queue_create((uint16_t *)&link_type_response, LINKCMD_INIT_BLOCK, 2, 10, NULL));
    queue_enqueue(&gLink.q, queue_create((uint16_t *)&select_slot, LINKCMD_CONT_BLOCK, link_type_response.size/2, 3, NULL));
    queue_enqueue(&gLink.q, queue_create((uint16_t *)no_data, LINKCMD_READY_EXIT_STANDBY, 1, 8, NULL));
}

static void trd_update_link_state() {
    switch (gLink.in_data)
    {
        case LINKCMD_SEND_LINK_TYPE: 
            gLink.last_state = gLink.state;
            gLink.state = LINKCMD_SEND_LINK_TYPE;
            break;
        
        case LINKCMD_INIT_BLOCK: 
            gLink.last_state = gLink.state;
            gLink.state = LINKCMD_INIT_BLOCK;
            break;

        case LINKCMD_SEND_BLOCK_REQ: 
            gLink.last_state = gLink.state;
            gLink.state = LINKCMD_SEND_BLOCK_REQ;
            break;

        case LINKCMD_READY_EXIT_STANDBY: 
            gLink.last_state = gLink.state;
            gLink.state = LINKCMD_READY_EXIT_STANDBY;
            break;

        case LINKCMD_SEND_HELD_KEYS: 
            gLink.last_state = 0; //keep updating
            gLink.state = LINKCMD_SEND_HELD_KEYS;
            break;

        case LINKCMD_READY_CLOSE_LINK: 
            gLink.last_state = gLink.state;
            gLink.state = LINKCMD_READY_CLOSE_LINK;
            break;

        default: break;
    }
}

static void trd_wait_for_master() {
    if (gLink.in_data == MASTER_HANDSHAKE) {
        gLink.master_handshaked = true;
    } else if (gLink.master_handshaked) {
        gLink.master_handshaked = false;
        pioService.connected = true;
        gLink.state = LINK_STATE_CONN_ESTABLISHED;
        gLink.out_data = NO_DATA;
        trd_start_timer();
    }
}

static void trd_start_timer() {
    pioService.timer = -1;
    gLink.timer_running = true;
}

static void trd_close_link() {
    pioService.connected = false;
    gLink.timer_running = false;
    gLink.state = SLAVE_HANDSHAKE;
    gLink.part_sent_index = 0;
    if (gLink.save_state) {
        //I find this the best time to save things
        trd_swap_pokemon_to_slot();
    }
}

void trd_timeout_callback()
{
    printf("Connection timed out!\n");
    pioService.connected = false;
    gLink.master_handshaked = false;
    gLink.is_trading_screen = false;
    gLink.last_state = 0;
    gLink.link_state = 0;
    gLink.block_req = 0;
    gLink.has_queue = false;
    gLink.block_index = 0;
    gLink.save_state = false;
    gLink.state = LINK_STATE_HANDSHAKE;
    gLink.in_data = NO_DATA;
    gLink.out_data = NO_DATA;
    gLink.part_sent_index = 0;
    gLink.slot_selected = -1;
    gLink.trade_state = 0;
    gLink.read_party_counter = 0;
    trd_start_timer();
}

void trd_end_link() {
    printf("closing link connection\n");
    sleep_ms(5000);
    while (queue_not_empty(&gLink.q))
        queue_dequeue(&gLink.q, NULL);
    trd_timeout_callback();
}

static void trd_initialize() {
    pio_initialize(1000, trd_timeout_callback);
    gLink.q = queue_initialize();
    link_player_block = shd_create_link_player(LINKTYPE_TRADE_SETUP);
    trainer_card = shd_create_trainer_card();
    shd_create_party(zigzagoon);
    shd_create_dummies();
    
    printf("initialization complete\n");
}
