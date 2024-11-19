#include "defines.h"
#include "pico/multicore.h"
#include "trade.h"
#include "utilities.h"

static void wrk_notify_link_type() {
    printf("Link type: ");
    switch (gLink.link_state)
    {
        case LINKTYPE_TRADE:
            printf("Trade\n");
            break;
        case LINKTYPE_TRADE_CONNECTING:
            printf("Connecting\n");
            break;
        case LINKTYPE_TRADE_SETUP:
            printf("Setup\n");
            break;
        case LINKTYPE_TRADE_DISCONNECTED:
            printf("Disconnected\n");
            break;
        default:
            printf("Unkown\n");
            break;
    }
}

static void wrk_notify_block_request() {
    if (gLink.is_trading_screen) return;
    printf("Block request: ");
    switch (gLink.block_req)
    {
        case TRAINER_CARD:
            printf("Trainer Card\n");
            break;

        case PARTY_SET:
            printf("Party\n");
            break;

        case MAILS:
            printf("Mails\n");
            break;

        case RIBBONS:
            printf("Ribbon\n");
            printf("In trading screen.\n");
            break;
        
        default:
            printf("Unkown\n");
            break;
    }
}

void wrk_notify_key_state() {
    switch (gLink.received_block[1])
    {
        case LINK_KEY_CODE_READY:
            printf("Player ready to trade.\n");
            break;

        case LINK_KEY_CODE_EXIT_ROOM:
            printf("Player ready to exit room.\n");
            trd_end_link();
            break;

        default: break;
    }
}

void wrk_notify_trade_state() {
    switch (gLink.trade_state)
    {
        case LINKCMD_START_TRADE:
            printf("Trading start\n");
            break;

        case LINKCMD_SET_MONS_TO_TRADE:
            printf("Player selected slot: %d\n", gLink.slot_selected);
            break;

        case LINKCMD_CONFIRM_FINISH_TRADE:
            printf("Trading confirm finish\n");
            break;

        case LINKCMD_READY_FINISH_TRADE:
            printf("Trading ready to finish\n");
            break;

        case LINKCMD_PLAYER_CANCEL_TRADE:
            printf("Player cancel trade\n");
            break;

        case LINKCMD_REQUEST_CANCEL:
            printf("Player request trade cancel\n");
            break;

        case LINKCMD_BOTH_CANCEL_TRADE:
            printf("Traiding cancelled!\n");
            break;
        
        default: break;
    }
}

void wrk_copy_player_party() {
    copy_data((uint16_t *)&playerParty + (((gLink.part_sent_index - 1) * 100) + gLink.read_party_counter), &gLink.received_block[1], (gLink.read_party_counter >= 98 ? 2 : 7) * 2);
    gLink.read_party_counter = gLink.read_party_counter + 7;
}

void wrk_main() {
    
    while (1) {
        uint16_t gState = multicore_fifo_pop_blocking();
        
        switch (gState)
        {
            case LINK_STATE_CONN_ESTABLISHED:
                printf("Connection established!\n");
                break;
            case LINKCMD_SEND_LINK_TYPE:
                gLink.read_party_counter = 0;
                wrk_notify_link_type();
                break;

            case LINKCMD_SEND_BLOCK_REQ:
                wrk_notify_block_request();
                break;

            case LINKCMD_READY_EXIT_STANDBY:
                printf("Ready exit standby\n");
                break;

            case LINKCMD_SEND_HELD_KEYS:
                wrk_notify_key_state();
                break;
            
            case LINKCMD_READY_CLOSE_LINK:
                printf("link close ready\n");
                break;

            case COPY_DATA:
                wrk_copy_player_party();
                break;
        
            default: break;
        }

        if (gLink.is_trading_screen) wrk_notify_trade_state();
        
        tight_loop_contents();
    }
}
