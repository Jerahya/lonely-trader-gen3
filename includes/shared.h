#ifndef SHARED_H
#define SHARED_H
#pragma once
#include "custom_types.h"

static const char sASCIIGameFreakInc[] = "GameFreak inc.";
static const uint16_t use_version = VERSION_EMERALD;

extern uint16_t no_data[];
extern uint16_t key_up[];
extern uint16_t key_down[];
extern uint16_t key_left[];
extern uint16_t key_right[];
extern uint16_t key_empty[];
extern uint16_t key_ready[];
extern uint16_t key_exit[];

extern uint8_t zigzagoon[100];
extern struct LinkBlockSize link_type_response;

extern struct TradeActionBlock select_slot;
extern struct LinkPlayer link_player;

//mail and ribbons. just filler for now
extern uint16_t dummy_filler[2];
extern uint16_t dummy_filler2[21];

extern struct LinkPlayerBlock link_player_block;
extern struct TrainerCard trainer_card;
extern struct PokemonParty tradingParty;
extern struct MailDummy dummyMail;
extern struct DummyMessage dummyMailMsg;

void shd_create_dummies();
void shd_create_party(uint8_t first[100]);
struct LinkPlayerBlock shd_create_link_player(uint32_t linkType);
struct TrainerCard shd_create_trainer_card();


#endif