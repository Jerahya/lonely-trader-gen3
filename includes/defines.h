#ifndef DEFINES_H
#define DEFINES_H

//Game
#define TRUE                    1
#define FALSE                   0

#define MALE                    0
#define FEMALE                  1

#define VERSION_SAPPHIRE        1
#define VERSION_RUBY            2
#define VERSION_EMERALD         3
#define VERSION_FIRE_RED        4
#define VERSION_LEAF_GREEN      5
#define VERSION_HEART_GOLD      7
#define VERSION_SOUL_SILVER     8
#define VERSION_DIAMOND         10
#define VERSION_PEARL           11
#define VERSION_PLATINUM        12
#define VERSION_GAMECUBE        15

#define LANGUAGE_JAPANESE       1
#define LANGUAGE_ENGLISH        2
#define LANGUAGE_FRENCH         3
#define LANGUAGE_ITALIAN        4
#define LANGUAGE_GERMAN         5
#define LANGUAGE_KOREAN         6
#define LANGUAGE_SPANISH        7
#define NUM_LANGUAGES           7

#define CMD_LENGTH                      8
#define PLAYER_NAME_LENGTH              7
#define POKEMON_NAME_LENGTH             10

#define TRAINER_CARD_PROFILE_LENGTH     4
#define TRAINER_CARD_STICKER_TYPES      3
#define PARTY_SIZE                      6


//LINK
#define KEY_EMPTY                       0x0011
#define KEY_DOWN                        0x0012
#define KEY_UP                          0x0013
#define KEY_LEFT                        0x0014
#define KEY_RIGHT                       0x0015
#define LINK_KEY_CODE_READY             0x0016
#define LINK_KEY_CODE_EXIT_ROOM         0x0017

#define MASTER_HANDSHAKE                0x8FFF
#define SLAVE_HANDSHAKE                 0xB9A0
#define NO_DATA                         0x0000
#define COPY_DATA                       0x9998

#define LINKTYPE_TRADE                  0x1111
#define LINKTYPE_TRADE_CONNECTING       0x1122
#define LINKTYPE_TRADE_SETUP            0x1133
#define LINKTYPE_TRADE_DISCONNECTED     0x1144

#define LINKCMD_SEND_LINK_TYPE          0x2222
#define LINKCMD_READY_EXIT_STANDBY      0x2FFE
#define LINKCMD_READY_CLOSE_LINK        0x5FFF
#define LINKCMD_CONT_BLOCK              0x8888
#define LINKCMD_SEND_ITEM               0xAAAB
#define LINKCMD_READY_TO_TRADE          0xAABB
#define LINKCMD_READY_FINISH_TRADE      0xABCD
#define LINKCMD_INIT_BLOCK              0xBBBB
#define LINKCMD_SEND_HELD_KEYS          0xCAFE
#define LINKCMD_SEND_BLOCK_REQ          0xCCCC
#define LINKCMD_START_TRADE             0xCCDD
#define LINKCMD_CONFIRM_FINISH_TRADE    0xDCBA
#define LINKCMD_SET_MONS_TO_TRADE       0xDDDD
#define LINKCMD_PLAYER_CANCEL_TRADE     0xDDEE
#define LINKCMD_REQUEST_CANCEL          0xEEAA
#define LINKCMD_BOTH_CANCEL_TRADE       0xEEBB
#define LINKCMD_PARTNER_CANCEL_TRADE    0xEECC
#define LINKCMD_NONE                    0xEFFF

#define LINK_STATE_HANDSHAKE            0X9998 
#define LINK_STATE_CONN_ESTABLISHED     0x9997


#endif