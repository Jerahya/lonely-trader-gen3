#ifndef CUSTOMTYPES_H
#define CUSTOMTYPES_H

#include "pico/stdlib.h"
#include "defines.h"

typedef uint8_t bool8;
typedef uint16_t bool16;

//Block request type address
enum {
    PARTY_SET       = 0x01,
    TRAINER_CARD    = 0x02,
    MAILS           = 0x03,
    RIBBONS         = 0x04, //Not really sure
};

struct LinkPlayer
{
    uint16_t version;
    uint16_t lp_field_2;
    uint32_t trainerId;
    uint8_t name[PLAYER_NAME_LENGTH + 1];
    uint8_t progressFlags;
    uint8_t neverRead;
    uint8_t progressFlagsCopy;
    uint8_t gender;
    uint32_t linkType;
    uint16_t id;
    uint16_t language;
};

struct LinkPlayerBlock
{
    char magic1[16];
    struct LinkPlayer linkPlayer;
    char magic2[16];
};

struct TrainerCard
{
    /*0x00*/ uint8_t gender;
    uint8_t stars;
    bool8 hasPokedex;
    bool8 caughtAllHoenn;
    bool8 hasAllPaintings;
    uint16_t hofDebutHours;
    uint16_t hofDebutMinutes;
    uint16_t hofDebutSeconds;
    uint16_t caughtMonsCount;
    uint16_t trainerId;
    uint16_t playTimeHours;
    uint16_t playTimeMinutes;
    uint16_t linkBattleWins;
    uint16_t linkBattleLosses;
    uint16_t battleTowerWins;
    uint16_t battleTowerStraightWins;
    uint16_t contestsWithFriends;
    uint16_t pokeblocksWithFriends;
    uint16_t pokemonTrades;
    uint32_t money;
    uint16_t easyChatProfile[TRAINER_CARD_PROFILE_LENGTH];
    uint8_t playerName[PLAYER_NAME_LENGTH + 1];
    uint8_t version;
    bool16 linkHasAllFrontierSymbols;
    union {
                uint32_t berryCrush;
                uint32_t frontier;
             } linkPoints;
    uint32_t unionRoomNum;
    uint8_t filler[8];
    bool8 shouldDrawStickers;
    uint8_t unused;
    uint8_t monIconTint;
    uint8_t unionRoomClass;
    uint8_t stickers[TRAINER_CARD_STICKER_TYPES];
    bool16 monSpecies[PARTY_SIZE];
    bool16 hasAllFrontierSymbols;
    bool16 frontierBP;
};


struct LinkBlockSize {
    uint16_t size;
    uint16_t status;
};

struct PokemonParty {
    uint8_t slot0[100];
    uint8_t slot1[100];
    uint8_t slot2[100];
    uint8_t slot3[100];
    uint8_t slot4[100];
    uint8_t slot5[100];
};


struct MailDummy {
    uint16_t v1[13];//0xFFFF
    uint16_t v2[2];//0x00
    uint16_t v3;//0x01
    uint16_t v4[2];//0x00
};

struct DummyMessage {
    struct MailDummy v0;
    struct MailDummy v1;
    struct MailDummy v2;
    struct MailDummy v3;
    struct MailDummy v4;
    struct MailDummy v5;
    uint16_t filler[2];//0xFFFFFFFF
};

struct TradeActionBlock {
    uint16_t cmd_trade;
    uint16_t slot;
    uint16_t f1[8];//0x00
    uint16_t f2[3];//0xFF
    uint16_t f3[3];//0x00
};


#endif