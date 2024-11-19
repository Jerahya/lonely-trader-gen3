#include "shared.h"
#include "utilities.h"
#include "flash_storage.h"

uint16_t no_data[]      = { 0x0000 };
uint16_t key_up[]       = { KEY_UP };
uint16_t key_down[]     = { KEY_DOWN };
uint16_t key_left[]     = { KEY_LEFT };
uint16_t key_right[]    = { KEY_RIGHT };
uint16_t key_empty[]    = { KEY_EMPTY };
uint16_t key_ready[]    = { LINK_KEY_CODE_READY };
uint16_t key_exit[]     = { LINK_KEY_CODE_EXIT_ROOM };

uint8_t zigzagoon[] = { 
    0xEE, 0x7F, 0x1B, 0xA0, 0x23, 0x1C, 0xCF, 0xA3, 0xD4, 0xC3, 
    0xC1, 0xD4, 0xBB, 0xC1, 0xC9, 0xC9, 0xC8, 0xFF, 0x02, 0x02, 
    0xC4, 0xBB, 0xD3, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x65, 0x34, 
    0x00, 0x00, 0xEC, 0x63, 0xF9, 0x03, 0xCD, 0x63, 0xD4, 0x03, 
    0xEE, 0x4B, 0xD4, 0x03, 0xED, 0x62, 0xD4, 0x03, 0xC5, 0x63, 
    0xD4, 0x03, 0xCD, 0x25, 0xD4, 0x03, 0xCD, 0x63, 0xD4, 0x03, 
    0xCD, 0x63, 0xD4, 0x03, 0xCD, 0x63, 0xD4, 0x03, 0xCD, 0x73, 
    0x56, 0x22, 0x9B, 0x34, 0x20, 0x38, 0xCD, 0x63, 0xD4, 0x03, 
    0x00, 0x00, 0x00, 0x00, 0x02, 0xFF, 0x0D, 0x00, 0x0D, 0x00, 
    0x06, 0x00, 0x07, 0x00, 0x06, 0x00, 0x06, 0x00, 0x07, 0x00 };

struct LinkPlayer link_player =  {
    .version = use_version + 0x4000,
    .language = LANGUAGE_ENGLISH,
    .lp_field_2 = 0x8000,
    .trainerId = 12345,
    .name = { 0xC4, 0xBB, 0xD3, 0xFF, 0x0, 0x0, 0x0, 0x0 },
    .progressFlags = 0x01,
    .progressFlagsCopy = 0x01,
    .neverRead = 0x00,
    .gender = FEMALE,
    .id = 0x0
};

struct LinkBlockSize link_type_response = {
    .size = 60,
    .status = 0x81
};

struct TradeActionBlock select_slot = {
    .cmd_trade  = LINKCMD_READY_TO_TRADE,
    .slot       = 0,
    .f1         = { 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },
    .f2         = { 0xFFFF, 0xFFFF, 0xFFFF },
    .f3         = { 0x0000, 0x0000, 0x0000 }
};


uint16_t dummy_filler[] = { 0xFFFF, 0xFFFF };
uint16_t dummy_filler2[] = { 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFF00, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0x0001, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF };

struct LinkPlayerBlock link_player_block = {};
struct TrainerCard trainer_card = {};
struct PokemonParty tradingParty = {};
struct MailDummy dummyMail = {};
struct DummyMessage dummyMailMsg = {};

void shd_create_dummies() {
    memset(dummyMail.v1, 0xFFFF, sizeof(dummyMail.v1));
    memset(dummyMail.v2, 0x0000, sizeof(dummyMail.v2));
    memset(dummyMail.v4, 0x0000, sizeof(dummyMail.v4));
    dummyMail.v3 = 0x0001;
    dummyMailMsg.v0 = dummyMail;
    dummyMailMsg.v1 = dummyMail;
    dummyMailMsg.v2 = dummyMail;
    dummyMailMsg.v3 = dummyMail;
    dummyMailMsg.v4 = dummyMail;
    dummyMailMsg.v5 = dummyMail;
    memset(dummyMailMsg.filler, 0xFF, sizeof(dummyMailMsg.filler));//dummyMailMsg size is 42 but we only use 40, otherwise we'll get a checksum error
}

void shd_create_party(uint8_t first[100]) {
    memcpy(tradingParty.slot0, first, 100);
    if (flash_read_data((uint16_t *)tradingParty.slot0, 100)) {
        printf("Loaded saved pokemon\n");
    } else {
        printf("Loaded default pokemon\n");
    }

    memset(tradingParty.slot1, 0x0, sizeof(tradingParty.slot1));
    memset(tradingParty.slot2, 0x0, sizeof(tradingParty.slot2));
    memset(tradingParty.slot3, 0x0, sizeof(tradingParty.slot3));
    memset(tradingParty.slot4, 0x0, sizeof(tradingParty.slot4));
    memset(tradingParty.slot5, 0x0, sizeof(tradingParty.slot5));
    
    tradingParty.slot1[85] = 0xFF;
    tradingParty.slot2[85] = 0xFF;
    tradingParty.slot3[85] = 0xFF;
    tradingParty.slot4[85] = 0xFF;
    tradingParty.slot5[85] = 0xFF;
}

struct LinkPlayerBlock shd_create_link_player(uint32_t linkType) {
    struct LinkPlayerBlock block;
    link_player.linkType = linkType;
    block.linkPlayer = link_player;
    memcpy(block.magic1, sASCIIGameFreakInc, sizeof(block.magic1) - 1);
    memcpy(block.magic2, sASCIIGameFreakInc, sizeof(block.magic2) - 1);
    return block;
}

struct TrainerCard shd_create_trainer_card()
{
    uint16_t profile[4] = { 0x0a29, 0x1020, 0x020e, 0x020b };
    uint8_t player_name[PLAYER_NAME_LENGTH + 1] = { 0xC4, 0xBB, 0xD3, 0xFF, 0x0, 0x0, 0x0, 0x0}; //JAY

    struct TrainerCard tc;
    tc.gender                   = FEMALE;
    tc.stars                    = 0;
    tc.hasPokedex               = true;
    tc.caughtAllHoenn           = false;
    tc.hasAllPaintings          = false;
    tc.hofDebutHours            = 0;
    tc.hofDebutMinutes          = 0;
    tc.hofDebutSeconds          = 0;
    tc.caughtMonsCount          = 2;
    tc.trainerId                = 404;
    tc.playTimeHours            = 1;
    tc.playTimeMinutes          = 30;
    tc.linkBattleWins           = 0;
    tc.linkBattleLosses         = 0;
    tc.battleTowerWins          = 0;
    tc.battleTowerStraightWins  = 0;
    tc.contestsWithFriends      = 0;
    tc.pokeblocksWithFriends    = 0;
    tc.pokemonTrades            = 0;
    tc.money                    = 1;
    tc.version                  = use_version;
    tc.linkHasAllFrontierSymbols = false;
    tc.linkPoints.berryCrush    = 0;
    tc.linkPoints.frontier      = 0;
    tc.unionRoomNum             = 0;
    tc.shouldDrawStickers       = false;
    tc.unused                   = 0;
    tc.monIconTint              = 0;
    tc.unionRoomClass           = 0;
    tc.hasAllFrontierSymbols    = false;
    tc.frontierBP               = false;

    memcpy(tc.easyChatProfile, profile, sizeof(profile));
    memcpy(tc.playerName, player_name, sizeof(profile));
    memset(tc.filler, 0x0, sizeof(tc.filler));
    memset(tc.stickers, 0x0, sizeof(tc.stickers));
    memset(tc.monSpecies, 0x0, sizeof(tc.monSpecies));

    return tc;
};
