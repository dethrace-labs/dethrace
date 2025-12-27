#include "network.h"
#include "brender.h"
#include "car.h"
#include "controls.h"
#include "displays.h"
#include "errors.h"
#include "globvars.h"
#include "globvrbm.h"
#include "globvrpb.h"
#include "graphics.h"
#include "harness/hooks.h"
#include "harness/trace.h"
#include "loading.h"
#include "netgame.h"
#include "newgame.h"
#include "oil.h"
#include "opponent.h"
#include "pd/net.h"
#include "pd/sys.h"
#include "pedestrn.h"
#include "piping.h"
#include "powerup.h"
#include "pratcam.h"
#include "replay.h"
#include "spark.h"
#include "structur.h"
#include "utility.h"
#include "world.h"
#include <stdlib.h>

tU32 gMess_max_flags;
tU32 gMess_mid_flags;
tU32 gMess_min_flags;

// GLOBAL: CARM95 0x0050d224
tU32 gGuarantee_number;

// GLOBAL: CARM95 0x0050d228
int gNet_service_disable = 0;

// GLOBAL: CARM95 0x0050d22c
int gIn_net_service = 0;

// GLOBAL: CARM95 0x0050d230
int gPlayer_list_batch_number = 0;

// GLOBAL: CARM95 0x0050d234
int gOnly_receive_guarantee_replies = 0;

// GLOBAL: CARM95 0x0050d238
void* gMessage_to_free;

// GLOBAL: CARM95 0x0050d23c
tNet_message* gBroadcast_stack;

// GLOBAL: CARM95 0x0050d240
tNet_message* gTo_host_stack;

// GLOBAL: CARM95 0x0050d244
tU32 gLast_flush_message = 0;

// GLOBAL: CARM95 0x0050d248
int gRace_only_flags[33] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1,
    0
};

// GLOBAL: CARM95 0x0050d2cc
int gJoin_list_mode;

// GLOBAL: CARM95 0x00535dd0
tNet_game_player_info gNew_net_players[6];

// GLOBAL: CARM95 0x00534c90
tGuaranteed_message gGuarantee_list[100]; // DOS debug symbols has this as [150]

// GLOBAL: CARM95 0x00534c70
tMid_message* gMid_messages;

// GLOBAL: CARM95 0x00534c74
tU32 gLast_player_list_received;

// GLOBAL: CARM95 0x00534c7c
tMin_message* gMin_messages;

// GLOBAL: CARM95 0x00535dc0
void (*gAdd_proc)(tNet_game_details*);

// GLOBAL: CARM95 0x00534c64
int gReceiving_batch_number;

// GLOBAL: CARM95 0x00536250
int gReceiving_new_players;

// GLOBAL: CARM95 0x00534c80
tMax_message* gMax_messages;

// GLOBAL: CARM95 0x00534c58
int gNext_guarantee;

// GLOBAL: CARM95 0x00534c84
tU32 gAsk_time;

// GLOBAL: CARM95 0x00551420
int gNet_initialised;

// GLOBAL: CARM95 0x00534c60
int gDont_allow_joiners;

// GLOBAL: CARM95 0x00536254
tNet_game_details* gCurrent_join_poll_game;

// GLOBAL: CARM95 0x00534c8c
int gMessage_header_size;

// GLOBAL: CARM95 0x00534c5c
int gJoin_poll_index;

// GLOBAL: CARM95 0x00534c88
int gJoin_request_denied;

// GLOBAL: CARM95 0x00534c68
int gHost_died;

// GLOBAL: CARM95 0x00535dc4
int gCar_was_taken;

// GLOBAL: CARM95 0x00534c78
int gBastard_has_answered;

// GLOBAL: CARM95 0x00535dc8
int gTime_for_next_one;

// GLOBAL: CARM95 0x005514d4
int gReceived_game_scores;

#define MIN_MESSAGES_CAPACITY 20
#define MID_MESSAGES_CAPACITY 10
#define MAX_MESSAGES_CAPACITY 20

#define MAX_MESAGE_STACK_SIZE 512

// IDA: int __cdecl NetInitialise()
// FUNCTION: CARM95 0x004463c0
int NetInitialise(void) {
    int i;

    SwitchToRealResolution();
    InitAbuseomatic();
    gNet_service_disable = 0;
    gIn_net_service = 0;
    gMessage_header_size = PDNetGetHeaderSize();
    gOnly_receive_guarantee_replies = 0;
    gMin_messages = BrMemAllocate(MIN_MESSAGES_CAPACITY * (gMessage_header_size + sizeof(tMin_message)), kMem_net_min_messages);
    gMid_messages = BrMemAllocate(MID_MESSAGES_CAPACITY * (gMessage_header_size + sizeof(tMid_message)), kMem_net_mid_messages);
    gMax_messages = BrMemAllocate(MAX_MESSAGES_CAPACITY * (gMessage_header_size + sizeof(tMax_message)), kMem_net_max_messages);
    for (i = 0; i < MIN_MESSAGES_CAPACITY; i++) {
        ((tNet_message*)&gMin_messages[i])->contents.header.type = NETMSGID_NONE;
    }
    for (i = 0; i < MID_MESSAGES_CAPACITY; i++) {
        ((tNet_message*)&gMid_messages[i])->contents.header.type = NETMSGID_NONE;
    }
    for (i = 0; i < MAX_MESSAGES_CAPACITY; i++) {
        ((tNet_message*)&gMax_messages[i])->contents.header.type = NETMSGID_NONE;
    }
    gNet_initialised = PDNetInitialise() == 0;
    if (gNet_initialised) {
        InitNetHeadups();
    }
    GenerateItFoxShadeTable();
    gDont_allow_joiners = 0;
    SwitchToLoresMode();
    return !gNet_initialised;
}

// IDA: int __cdecl NetShutdown()
// FUNCTION: CARM95 0x0044657f
int NetShutdown(void) {
    int err;
    int i;

    err = PDNetShutdown();
    DisposeAbuseomatic();
    BrMemFree(gMin_messages);
    BrMemFree(gMid_messages);
    BrMemFree(gMax_messages);
    DisposeNetHeadups();
    return err;
}

// IDA: void __cdecl ShutdownNetIfRequired()
// FUNCTION: CARM95 0x004465d1
void ShutdownNetIfRequired(void) {

    if (gNet_initialised) {
        NetShutdown();
        gNet_initialised = 0;
    }
}

// IDA: void __cdecl DisableNetService()
// FUNCTION: CARM95 0x004465f8
void DisableNetService(void) {

    gNet_service_disable = 1;
}

// IDA: void __cdecl ReenableNetService()
// FUNCTION: CARM95 0x0044660d
void ReenableNetService(void) {

    gNet_service_disable = 0;
}

// IDA: int __cdecl PermitNetServiceReentrancy()
// FUNCTION: CARM95 0x00446622
int PermitNetServiceReentrancy(void) {
    int prev;

    prev = !!gIn_net_service;
    if (prev) {
        gIn_net_service = 0;
    }
    return prev;
}

// IDA: void __cdecl HaltNetServiceReentrancy()
// FUNCTION: CARM95 0x0044665a
void HaltNetServiceReentrancy(void) {

    gIn_net_service = 1;
}

// IDA: void __usercall NetSendHeadupToAllPlayers(char *pMessage@<EAX>)
// FUNCTION: CARM95 0x0044666f
void NetSendHeadupToAllPlayers(char* pMessage) {
    tNet_contents* the_contents;

    if (gNet_mode) {
        the_contents = NetGetBroadcastContents(NETMSGID_HEADUP, 0);
        strcpy(the_contents->data.headup.text, pMessage);
    }
}

// IDA: void __usercall NetSendHeadupToEverybody(char *pMessage@<EAX>)
// FUNCTION: CARM95 0x004466c1
void NetSendHeadupToEverybody(char* pMessage) {
    tNet_contents* the_contents;

    if (gNet_mode != eNet_mode_none) {
        if (gProgram_state.racing) {
            NewTextHeadupSlot(eHeadupSlot_misc, 0, 3000, -4, pMessage);
        }
        the_contents = NetGetBroadcastContents(NETMSGID_HEADUP, 0);
        strcpy(the_contents->data.headup.text, pMessage);
    }
}

// IDA: void __usercall NetSendHeadupToPlayer(char *pMessage@<EAX>, tPlayer_ID pPlayer@<EDX>)
// FUNCTION: CARM95 0x00446737
void NetSendHeadupToPlayer(char* pMessage, tPlayer_ID pPlayer) {
    tNet_message* message;

    if (gNet_mode != eNet_mode_none) {
        if (gLocal_net_ID == pPlayer) {
            if (gProgram_state.racing) {
                NewTextHeadupSlot(eHeadupSlot_misc, 0, 3000, -4, pMessage);
            }
        } else {
            message = NetBuildMessage(NETMSGID_HEADUP, 0);
            strcpy(message->contents.data.headup.text, pMessage);
            NetGuaranteedSendMessageToPlayer(gCurrent_net_game, message, pPlayer, 0);
        }
    }
}

// IDA: void __cdecl InitialisePlayerStati()
// FUNCTION: CARM95 0x0044754e
void InitialisePlayerStati(void) {
    int i;

    for (i = 0; i < COUNT_OF(gNet_players); i++) {
        gNet_players[i].last_heard_from_him = PDGetTotalTime();
        gNet_players[i].player_status = ePlayer_status_loading;
    }
}

// IDA: void __cdecl LeaveTempGame()
// FUNCTION: CARM95 0x00446847
void LeaveTempGame(void) {

    if (gCurrent_join_poll_game != NULL) {
        NetLeaveGameLowLevel(gCurrent_join_poll_game);
    }
    gTime_for_next_one = 1;
    gCurrent_join_poll_game = NULL;
}

// IDA: void __cdecl DisposeCurrentJoinPollGame()
// FUNCTION: CARM95 0x00446881
void DisposeCurrentJoinPollGame(void) {

    if (gCurrent_join_poll_game != NULL) {
        NetDisposeGameDetails(gCurrent_join_poll_game);
        gCurrent_join_poll_game = NULL;
    }
}

// IDA: void __cdecl DoNextJoinPoll()
// FUNCTION: CARM95 0x0044a179
void DoNextJoinPoll(void) {
    tNet_message* the_message;

    if (gTime_for_next_one) {
        gCurrent_join_poll_game = NetAllocatePIDGameDetails();
        if (gCurrent_join_poll_game != NULL) {
            if (PDNetGetNextJoinGame(gCurrent_join_poll_game, gJoin_poll_index)) {
                if (NetJoinGameLowLevel(gCurrent_join_poll_game, "!TEMP!")) {
                    DisposeCurrentJoinPollGame();
                } else {
                    gTime_for_next_one = 0;
                    the_message = NetBuildMessage(NETMSGID_SENDMEDETAILS, 0);
                    NetSendMessageToAddress(gCurrent_join_poll_game, the_message, gCurrent_join_poll_game);
                    gBastard_has_answered = 0;
                    gAsk_time = PDGetTotalTime();
                }
                gJoin_poll_index++;
            } else {
                gJoin_poll_index = 0;
                if (gCurrent_join_poll_game != NULL) {
                    DisposeCurrentJoinPollGame();
                }
            }
        }
    } else {
        if (gBastard_has_answered) {
            gAdd_proc(gCurrent_join_poll_game);
            LeaveTempGame();
        } else {
            if (PDGetTotalTime() - gAsk_time > 10000) {
                LeaveTempGame();
                DisposeCurrentJoinPollGame();
            }
        }
    }
}

// IDA: void __usercall NetStartProducingJoinList(void (*pAdd_proc)(tNet_game_details*)@<EAX>)
// FUNCTION: CARM95 0x004467d9
void NetStartProducingJoinList(void (*pAdd_proc)(tNet_game_details*)) {

    gAdd_proc = pAdd_proc;
    gJoin_list_mode = 1;
    gBastard_has_answered = 0;
    gTime_for_next_one = 1;
    gJoin_poll_index = 0;
    gCurrent_join_poll_game = NULL;
    PDNetStartProducingJoinList();
}

// IDA: void __cdecl NetEndJoinList()
// FUNCTION: CARM95 0x00446823
void NetEndJoinList(void) {

    gJoin_list_mode = 0;
    DisposeCurrentJoinPollGame();
    LeaveTempGame();
    PDNetEndJoinList();
}

// IDA: void __usercall NetDisposePIDGameInfo(tNet_game_details *pDetails@<EAX>)
// FUNCTION: CARM95 0x004468b1
void NetDisposePIDGameInfo(tNet_game_details* pDetails) {

    if (pDetails != NULL) {
        BrMemFree(pDetails);
    }
}

// IDA: void __usercall NetDisposeGameDetails(tNet_game_details *pDetails@<EAX>)
// FUNCTION: CARM95 0x004468d2
void NetDisposeGameDetails(tNet_game_details* pDetails) {

    // LOG_WARN("NetDisposeGameDetails(%p)", pDetails);
    if (pDetails != NULL) {
        NetDisposePIDGameInfo(pDetails);
    }
}

// IDA: tNet_game_details* __cdecl NetAllocatePIDGameDetails()
// FUNCTION: CARM95 0x0044759d
tNet_game_details* NetAllocatePIDGameDetails(void) {
    tNet_game_details* game;

    return BrMemAllocate(sizeof(tNet_game_details), kMem_net_pid_details);
}

// IDA: void __usercall NetLeaveGameLowLevel(tNet_game_details *pDetails@<EAX>)
// FUNCTION: CARM95 0x004468f3
void NetLeaveGameLowLevel(tNet_game_details* pDetails) {

    if (gNet_mode == eNet_mode_host) {
        PDNetHostFinishGame(gCurrent_net_game);
    } else {
        PDNetLeaveGame(pDetails);
    }
}

// IDA: void __usercall NetLeaveGame(tNet_game_details *pNet_game@<EAX>)
// FUNCTION: CARM95 0x0044692a
void NetLeaveGame(tNet_game_details* pNet_game) {
    tNet_message* the_message;
    char s[256];
    char* s2;
    int i;
    int must_revert_reentrancy;

    if (gNet_mode == eNet_mode_none) {
        return;
    }
    gOnly_receive_guarantee_replies = 1;
    if (gNet_mode == eNet_mode_host) {
        gDont_allow_joiners = 1;
        the_message = NetBuildMessage(NETMSGID_HOSTICIDE, 0);
        must_revert_reentrancy = PermitNetServiceReentrancy();
        NetGuaranteedSendMessageToAllPlayers(pNet_game, the_message, NULL);
        if (must_revert_reentrancy) {
            HaltNetServiceReentrancy();
        }
    } else if (!gHost_died) {
        the_message = NetBuildMessage(NETMSGID_LEAVE, 0);
        NetGuaranteedSendMessageToHost(pNet_game, the_message, NULL);
        strcpy(s, gProgram_state.player_name[0]);
        strcat(s, " ");
        strcat(s, GetMiscString(kMiscString_HasLeftTheGame));
        NetSendHeadupToAllPlayers(s);
    }
    for (i = 0; i < gNumber_of_net_players; i++) {
        DisposeCarN(i);
    }
    ClearOutStorageSpace(&gOur_car_storage_space);
    ClearOutStorageSpace(&gNet_cars_storage_space);
    must_revert_reentrancy = PermitNetServiceReentrancy();
    NetSendMessageStacks();
    NetWaitForGuaranteeReplies();
    NetLeaveGameLowLevel(gCurrent_net_game);
    if (must_revert_reentrancy) {
        HaltNetServiceReentrancy();
    }
    gCurrent_net_game = NULL;
    gNet_mode = eNet_mode_none;
    gNumber_of_net_players = 0;
    gProgram_state.prog_status = eProg_idling;
    gOnly_receive_guarantee_replies = 0;
}

// IDA: void __usercall NetSetPlayerSystemInfo(tNet_game_player_info *pPlayer@<EAX>, void *pSender_address@<EDX>)
// FUNCTION: CARM95 0x00446b3b
void NetSetPlayerSystemInfo(tNet_game_player_info* pPlayer, void* pSender_address) {

    PDNetSetPlayerSystemInfo(pPlayer, pSender_address);
}

// IDA: void __usercall NetDisposePlayer(tNet_game_player_info *pPlayer@<EAX>)
// FUNCTION: CARM95 0x00446b56
void NetDisposePlayer(tNet_game_player_info* pPlayer) {

    PDNetDisposePlayer(pPlayer);
}

// IDA: void __usercall FillInThisPlayer(tNet_game_details *pGame@<EAX>, tNet_game_player_info *pPlayer@<EDX>, int pCar_index@<EBX>, int pHost@<ECX>)
// FUNCTION: CARM95 0x004475c5
void FillInThisPlayer(tNet_game_details* pGame, tNet_game_player_info* pPlayer, int pCar_index, int pHost) {

    pPlayer->host = pHost;
    pPlayer->ID = NetExtractPlayerID(pGame);
    strcpy(pPlayer->player_name, gProgram_state.player_name[0]);
    pPlayer->this_players_time_stamp = PDGetTotalTime();
    pPlayer->car_index = pCar_index;
    pPlayer->reposition_time = 0;
    pPlayer->last_waste_message = 0;
    pPlayer->wasteage_attributed = 0;
    pPlayer->name_not_clipped = 1;
    pPlayer->played = 0;
    pPlayer->won = 0;
    pPlayer->games_score = 0;
    pPlayer->race_stuff_initialised = 0;
    if (pGame->options.random_car_choice && (pGame->options.car_choice == eNet_car_all || pGame->options.car_choice == eNet_car_both)) {
        pPlayer->next_car_index = -1;
    }
    InitialisePlayerScore(pPlayer);
}

// IDA: void __usercall LoadCarN(int pIndex@<EAX>, tNet_game_player_info *pPlayer@<EDX>)
// FUNCTION: CARM95 0x0044725a
void LoadCarN(int pIndex, tNet_game_player_info* pPlayer) {
    int switched_res;

    pPlayer->car = BrMemAllocate(sizeof(tCar_spec), kMem_net_car_spec);
    switched_res = SwitchToRealResolution();
    LoadCar(gOpponents[pPlayer->car_index].car_file_name,
        eDriver_net_human,
        pPlayer->car,
        pPlayer->car_index,
        pPlayer->player_name,
        &gNet_cars_storage_space);
    if (switched_res) {
        SwitchToLoresMode();
    }
    InitialiseCar(pPlayer->car);
    if (pPlayer->player_status < ePlayer_status_racing) {
        pPlayer->car->disabled = 1;
    }
    SetCarStorageTexturingLevel(&gNet_cars_storage_space, GetCarTexturingLevel(), eCTL_full);
}

// IDA: void __usercall DisposeCarN(int pIndex@<EAX>)
// FUNCTION: CARM95 0x00446b6d
void DisposeCarN(int pIndex) {
    int i;
    int j;

    for (i = 0; i < gCurrent_race.number_of_racers; i++) {
        if (gCurrent_race.opponent_list[i].car_spec == gNet_players[pIndex].car) {
            gCurrent_race.number_of_racers--;
            for (j = i; j < gCurrent_race.number_of_racers; j++) {
                gCurrent_race.opponent_list[j].index = gCurrent_race.opponent_list[j + 1].index;
                gCurrent_race.opponent_list[j].ranking = gCurrent_race.opponent_list[j + 1].ranking;
                gCurrent_race.opponent_list[j].net_player_index = gCurrent_race.opponent_list[j + 1].net_player_index;
                gCurrent_race.opponent_list[j].car_spec = gCurrent_race.opponent_list[j + 1].car_spec;
            }
        }
    }
    if (gProgram_state.racing) {
        DisposeKevStuffCar(gNet_players[pIndex].car);
    }
    DisposeCar(gNet_players[pIndex].car, gNet_players[pIndex].car_index);
    if (gThis_net_player_index != pIndex) {
        BrMemFree(gNet_players[pIndex].car);
    }
    if (gAction_replay_mode) {
        ToggleReplay();
    }
    ResetPiping();
}

// IDA: void __usercall PlayerHasLeft(int pIndex@<EAX>)
// FUNCTION: CARM95 0x0044731b
void PlayerHasLeft(int pIndex) {

    if (gCurrent_net_game->options.random_car_choice && (gCurrent_net_game->options.car_choice == eNet_car_all || gCurrent_net_game->options.car_choice == eNet_car_both)) {
        if (gNet_players[pIndex].car_index >= 0) {
            gCar_details[gNet_players[pIndex].car_index].ownership = eCar_owner_none;
        }
        if (gNet_players[pIndex].next_car_index >= 0) {
            gCar_details[gNet_players[pIndex].next_car_index].ownership = eCar_owner_none;
        }
    }
}

// IDA: void __usercall NetPlayersChanged(int pNew_count@<EAX>, tNet_game_player_info *pNew_players@<EDX>)
// FUNCTION: CARM95 0x00446ca1
void NetPlayersChanged(int pNew_count, tNet_game_player_info* pNew_players) {
    int i;
    int j;
    int k;
    int switched_res;
    int new_player;
    int player_still_there;
    tPlayer_ID old_fox_it;

    if (gCurrent_net_game->type == eNet_game_type_tag || gCurrent_net_game->type == eNet_game_type_foxy) {
#ifdef DETHRACE_FIX_BUGS
        old_fox_it = -1;
        if (gIt_or_fox >= 0) {
#endif
            old_fox_it = gNet_players[gIt_or_fox].ID;
#ifdef DETHRACE_FIX_BUGS
        }
#endif
    }
    for (i = 0; i < pNew_count; i++) {
        new_player = 1;
        for (j = 0; j < gNumber_of_net_players; j++) {
            if (pNew_players[i].ID == gNet_players[j].ID) {
                new_player = 0;
                pNew_players[i].last_waste_message = gNet_players[j].last_waste_message;
                pNew_players[i].player_status = gNet_players[j].player_status;
                pNew_players[i].car_index = gNet_players[j].car_index;
                pNew_players[i].score = gNet_players[j].score;
                pNew_players[i].credits = gNet_players[j].credits;
                pNew_players[i].wasted = gNet_players[j].wasted;
                pNew_players[i].wasteage_attributed = gNet_players[j].wasteage_attributed;
                pNew_players[i].next_car_index = gNet_players[j].next_car_index;
                pNew_players[i].car = gNet_players[j].car;
                break;
            }
        }
        if (new_player) {
            if (pNew_players[i].ID == gLocal_net_ID) {
                switched_res = SwitchToRealResolution();
                LoadCar(gOpponents[pNew_players[i].car_index].car_file_name,
                    eDriver_local_human,
                    &gProgram_state.current_car,
                    pNew_players[i].car_index,
                    pNew_players[i].player_name,
                    &gNet_cars_storage_space);
                if (switched_res) {
                    SwitchToLoresMode();
                }
                pNew_players[i].car = &gProgram_state.current_car;
                if (pNew_players[i].player_status < ePlayer_status_racing) {
                    pNew_players[i].car->disabled = 1;
                }
                SetCarStorageTexturingLevel(&gNet_cars_storage_space, GetCarTexturingLevel(), eCTL_full);
            } else {
                LoadCarN(i, &pNew_players[i]);
            }
            gCurrent_race.opponent_list[i].index = -1;
            gCurrent_race.opponent_list[i].ranking = IRandomBetween(0, 99);
            gCurrent_race.opponent_list[i].car_spec = pNew_players[i].car;
            gCurrent_race.opponent_list[i].net_player_index = i;
            pNew_players[i].opponent_list_index = i;
            gCurrent_race.number_of_racers = pNew_count;
        }
    }
    for (i = 0; i < gNumber_of_net_players; i++) {
        player_still_there = 0;
        for (j = 0; j < pNew_count; j++) {
            if (pNew_players[j].ID == gNet_players[i].ID) {
                player_still_there = 1;
                break;
            }
        }
        if (!player_still_there) {
            for (j = 0; j < gNumber_of_net_players; j++) {
                if (gCurrent_race.opponent_list[j].net_player_index == i) {
                    memmove(&gCurrent_race.opponent_list[j], &gCurrent_race.opponent_list[j + 1], (gNumber_of_net_players - j - 1) * sizeof(tOpp_spec));
                    for (k = 0; k < pNew_count; k++) {
                        if (j < pNew_players[k].opponent_list_index) {
                            pNew_players[k].opponent_list_index--;
                        }
                    }
                    break;
                }
            }
            PlayerHasLeft(i);
            DisposeCarN(i);
            if (gInitialised_grid) {
                for (i = 0; i < pNew_count; i++) {
                    gCurrent_race.opponent_list[pNew_players[i].opponent_list_index].net_player_index = i;
                }
            }
        }
    }
    gNumber_of_net_players = pNew_count;
    memcpy(gNet_players, pNew_players, pNew_count * sizeof(tNet_game_player_info));
    for (i = 0; i < gNumber_of_net_players; i++) {
        gNet_players[i].last_heard_from_him = PDGetTotalTime();
    }
    ForceRebuildActiveCarList();
    if (gCurrent_net_game->type == eNet_game_type_tag || gCurrent_net_game->type == eNet_game_type_foxy) {
        gIt_or_fox = -1;
        for (i = 0; i < gNumber_of_net_players; i++) {
            if (gNet_players[i].ID == old_fox_it) {
                gIt_or_fox = i;
                break;
            }
        }
    }
}

// IDA: tNet_game_details* __usercall NetHostGame@<EAX>(tNet_game_type pGame_type@<EAX>, tNet_game_options *pOptions@<EDX>, int pStart_rank@<EBX>, char *pHost_name@<ECX>, int pCar_index)
// FUNCTION: CARM95 0x004473b1
tNet_game_details* NetHostGame(tNet_game_type pGame_type, tNet_game_options* pOptions, int pStart_rank, char* pHost_name, int pCar_index) {
    tNet_game_details* game;
    void* host_address;
    tNet_game_player_info me;

    game = NetAllocatePIDGameDetails();
    if (pHost_name[0] == '\0') {
        sprintf(pHost_name, "%s", "HOST");
    }
    DisableNetService();
    if (PDNetHostGame(game, pHost_name, &host_address) == 0) {
        NetDisposeGameDetails(game);
        return NULL;
    }
    gCurrent_net_game = game;
    gNeed_to_send_start_race = 0;
    strcpy(game->host_name, pHost_name);
    game->host_ID = NetExtractPlayerID(game);
    game->num_players = 1;
    memcpy(&game->options, pOptions, sizeof(tNet_game_options));
    game->status.stage = eNet_game_starting;
    game->type = pGame_type;
    game->start_race = pStart_rank;
    game->no_races_yet = 1;
    gReceiving_new_players = 0;
    gHost_died = 0;
    gNumber_of_net_players = 0;
    gThis_net_player_index = 0;
    gLocal_net_ID = game->host_ID;
    FillInThisPlayer(game, &me, pCar_index, 1);
    gNet_players[0].race_stuff_initialised = 1;
    NetSetPlayerSystemInfo(&me, host_address);
    NetPlayersChanged(1, &me);
    InitialisePlayerStati();
    gNet_mode = eNet_mode_host;
    gDont_allow_joiners = 0;
    return game;
}

// IDA: int __usercall NetInitClient@<EAX>(tNet_game_details *pDetails@<EAX>)
int NetInitClient(tNet_game_details* pDetails) {

    return PDNetInitClient(pDetails);
}

// IDA: int __usercall NetJoinGameLowLevel@<EAX>(tNet_game_details *pDetails@<EAX>, char *pPlayer_name@<EDX>)
// FUNCTION: CARM95 0x00447883
int NetJoinGameLowLevel(tNet_game_details* pDetails, char* pPlayer_name) {

    return PDNetJoinGame(pDetails, pPlayer_name);
}

DR_STATIC_ASSERT(offsetof(tNet_message_join, player_info) == 4);
// DR_STATIC_ASSERT(offsetof(tNet_game_player_info, this_players_time_stamp) == 0x10);
// DR_STATIC_ASSERT(offsetof(tNet_game_player_info, wasted) == 0x68);
// DR_STATIC_ASSERT(offsetof(tNet_game_player_info, initial_position) == 0x8c);
// DR_STATIC_ASSERT(offsetof(tNet_game_player_info, car) == 0xbc);

// IDA: int __usercall NetJoinGame@<EAX>(tNet_game_details *pDetails@<EAX>, char *pPlayer_name@<EDX>, int pCar_index@<EBX>)
// FUNCTION: CARM95 0x004476d9
int NetJoinGame(tNet_game_details* pDetails, char* pPlayer_name, int pCar_index) {
    int result;
    tNet_message* the_message;
    tU32 start_time;

    result = NetJoinGameLowLevel(pDetails, pPlayer_name);
    if (result != 0) {
        return result;
    }
    DisableNetService();
    gReceiving_new_players = 0;
    gNet_mode = eNet_mode_client;
    gCurrent_net_game = pDetails;
    gLocal_net_ID = NetExtractPlayerID(pDetails);
    gNumber_of_net_players = 0;
    gLast_player_list_received = 0;
    gJoin_request_denied = 0;
    gCar_was_taken = 0;
    gHost_died = 0;
    the_message = NetBuildMessage(NETMSGID_JOIN, 0);
    FillInThisPlayer(pDetails, &the_message->contents.data.join.player_info, pCar_index, 0);
    ReenableNetService();
    NetGuaranteedSendMessageToAddress(pDetails, the_message, pDetails, NULL);
    start_time = PDGetTotalTime();
    while (1) {
        NetService(0);
        if (gNumber_of_net_players != 0) {
            break;
        }
        if (PDGetTotalTime() - start_time >= 30000 || gJoin_request_denied || gHost_died) {
            break;
        }
    }
    DisableNetService();
    InitialisePlayerStati();
    if (gNumber_of_net_players == 0) {
        ReenableNetService();
        if (gJoin_request_denied && gCar_was_taken) {
            result = -4;
        } else {
            gNet_mode = eNet_mode_none;
#if !defined(DETHRACE_FIX_BUGS)
            // Avoid double free
            NetDisposeGameDetails(gCurrent_net_game);
#endif
            gCurrent_net_game = NULL;
            if (gJoin_request_denied) {
                result = -2;
            } else {
                result = -1;
            }
        }
    }
    return result;
}

// IDA: void __usercall NetObtainSystemUserName(char *pName@<EAX>, int pMax_length@<EDX>)
// FUNCTION: CARM95 0x004478a3
void NetObtainSystemUserName(char* pName, int pMax_length) {

    PDNetObtainSystemUserName(pName, pMax_length);
    pName[9] = '\0';
}

// IDA: tU32 __usercall NetExtractGameID@<EAX>(tNet_game_details *pDetails@<EAX>)
// FUNCTION: CARM95 0x004478c5
tU32 NetExtractGameID(tNet_game_details* pDetails) {

    return PDNetExtractGameID(pDetails);
}

// IDA: tPlayer_ID __usercall NetExtractPlayerID@<EAX>(tNet_game_details *pDetails@<EAX>)
// FUNCTION: CARM95 0x004478e1
tPlayer_ID NetExtractPlayerID(tNet_game_details* pDetails) {

    return PDNetExtractPlayerID(pDetails);
}

// IDA: int __usercall NetSendMessageToAddress@<EAX>(tNet_game_details *pDetails@<EAX>, tNet_message *pMessage@<EDX>, void *pAddress@<EBX>)
// FUNCTION: CARM95 0x004478fd
int NetSendMessageToAddress(tNet_game_details* pDetails, tNet_message* pMessage, void* pAddress) {

    if (gNet_mode == eNet_mode_none && !gJoin_list_mode) {
        return -3;
    }
    pMessage->sender = gLocal_net_ID;
    pMessage->senders_time_stamp = PDGetTotalTime();
    GetCheckSum(pMessage);
    return PDNetSendMessageToAddress(pDetails, pMessage, pAddress);
}

// IDA: int __usercall NetSendMessageToPlayer@<EAX>(tNet_game_details *pDetails@<EAX>, tNet_message *pMessage@<EDX>, tPlayer_ID pPlayer@<EBX>)
int NetSendMessageToPlayer(tNet_game_details* pDetails, tNet_message* pMessage, tPlayer_ID pPlayer) {
    int i;

    if (gNet_mode == eNet_mode_none) {
        return -3;
    }
    pMessage->sender = gLocal_net_ID;
    pMessage->senders_time_stamp = PDGetTotalTime();
    for (i = 0; i < gNumber_of_net_players; i++) {
        if (gNet_players[i].ID == pPlayer) {
            GetCheckSum(pMessage);
            return PDNetSendMessageToAddress(pDetails, pMessage, &gNet_players[i]);
        }
    }
    return -3;
}

// IDA: int __usercall NetSendMessageToHost@<EAX>(tNet_game_details *pDetails@<EAX>, tNet_message *pMessage@<EDX>)
// FUNCTION: CARM95 0x00447a10
int NetSendMessageToHost(tNet_game_details* pDetails, tNet_message* pMessage) {

    if (gNet_mode == eNet_mode_none) {
        return -3;
    }
    pMessage->sender = gLocal_net_ID;
    pMessage->senders_time_stamp = PDGetTotalTime();
    DoCheckSum(pMessage);
    return PDNetSendMessageToAddress(pDetails, pMessage, &pDetails->pd_net_info);
}

// IDA: int __usercall NetReplyToMessage@<EAX>(tNet_game_details *pDetails@<EAX>, tNet_message *pIncoming_message@<EDX>, tNet_message *pReply_message@<EBX>)
int NetReplyToMessage(tNet_game_details* pDetails, tNet_message* pIncoming_message, tNet_message* pReply_message) {

    return NetSendMessageToPlayer(pDetails, pReply_message, pIncoming_message->sender);
}

// IDA: int __usercall NetSendMessageToAllPlayers@<EAX>(tNet_game_details *pDetails@<EAX>, tNet_message *pMessage@<EDX>)
// FUNCTION: CARM95 0x00447a99
int NetSendMessageToAllPlayers(tNet_game_details* pDetails, tNet_message* pMessage) {

    pMessage->sender = gLocal_net_ID;
    pMessage->senders_time_stamp = PDGetTotalTime();
    GetCheckSum(pMessage);
    return PDNetSendMessageToAllPlayers(pDetails, pMessage);
}

// IDA: tU32 __usercall NetGetContentsSize@<EAX>(tNet_message_type pType@<EAX>, tS32 pSize_decider@<EDX>)
// FUNCTION: CARM95 0x00447adb
tU32 NetGetContentsSize(tNet_message_type pType, tS32 pSize_decider) {
    tU32 the_size;

    switch (pType) {
    case NETMSGID_SENDMEDETAILS:
        return sizeof(tNet_message_send_me_details);
    case NETMSGID_DETAILS:
        return sizeof(tNet_message_my_details);
    case NETMSGID_JOIN:
        return sizeof(tNet_message_join);
    case NETMSGID_NEWPLAYERLIST:
        return sizeof(tNet_message_new_player_list);
    case NETMSGID_GUARANTEEREPLY:
        return sizeof(tNet_message_guarantee_reply);
    case NETMSGID_CARDETAILSREQ:
        return sizeof(tNet_message_car_details_req);
    case NETMSGID_CARDETAILS:
        return sizeof(tNet_message_car_details);
    case NETMSGID_LEAVE:
        return sizeof(tNet_message_leave);
    case NETMSGID_HOSTICIDE:
        return sizeof(tNet_message_host_pissing_off);
    case NETMSGID_RACEOVER:
        return sizeof(tNet_message_race_over);
    case NETMSGID_STATUSREPORT:
        return sizeof(tNet_message_status_report);
    case NETMSGID_STARTRACE:
        return sizeof(tNet_message_start_race);
    case NETMSGID_HEADUP:
        return sizeof(tNet_message_headup);
    case NETMSGID_HOSTQUERY:
        return sizeof(tNet_message_host_query);
    case NETMSGID_HOSTREPLY:
        return sizeof(tNet_message_host_reply);
    case NETMSGID_MECHANICS:
        if (pSize_decider == 0) {
            return offsetof(tNet_message_mechanics_info, wheel_dam_offset);
        } else {
            return sizeof(tNet_message_mechanics_info);
        }
    case NETMSGID_NONCAR_INFO:
        return sizeof(tNet_message_non_car_info);
    case NETMSGID_TIMESYNC:
        return sizeof(tNet_message_time_sync);
    case NETMSGID_CONFIRM:
        return sizeof(tNet_message_players_confirm);
    case NETMSGID_DISABLECAR:
        return sizeof(tNet_message_disable_car);
    case NETMSGID_ENABLECAR:
        return sizeof(tNet_message_enable_car);
    case NETMSGID_POWERUP:
        return sizeof(tNet_message_powerup);
    case NETMSGID_RECOVER:
        return sizeof(tNet_message_recover);
    case NETMSGID_SCORES:
        return sizeof(tNet_message_scores);
    case NETMSGID_WASTED:
        return sizeof(tNet_message_wasted);
    case NETMSGID_PEDESTRIAN:
        switch (pSize_decider) {
        case 0:
            return offsetof(tNet_message_pedestrian, to_pos);
        case 1:
            return offsetof(tNet_message_pedestrian, offset);
        case 2:
            return sizeof(tNet_message_pedestrian);
        default:
            TELL_ME_IF_WE_PASS_THIS_WAY();
        }
    case NETMSGID_GAMEPLAY:
        return sizeof(tNet_message_gameplay);
    case NETMSGID_NONCARPOSITION:
        return sizeof(tNet_message_non_car_position);
    case NETMSGID_COPINFO:
        return sizeof(tNet_message_cop_info);
    case NETMSGID_GAMESCORES:
        return sizeof(tNet_message_game_scores);
    case NETMSGID_OILSPILL:
        return sizeof(tNet_message_oil_spill);
    case NETMSGID_CRUSHPOINT:
        return sizeof(tNet_message_crush_point);
    default:
        TELL_ME_IF_WE_PASS_THIS_WAY();
        return 4;
    }
}

// IDA: tU32 __usercall NetGetMessageSize@<EAX>(tNet_message_type pType@<EAX>, tS32 pSize_decider@<EDX>)
// FUNCTION: CARM95 0x00447dd9
tU32 NetGetMessageSize(tNet_message_type pType, tS32 pSize_decider) {

    return NetGetContentsSize(pType, pSize_decider) + sizeof(tNet_message) - sizeof(tNet_contents);
}

// IDA: tS32 __usercall NetCalcSizeDecider@<EAX>(tNet_contents *pContents@<EAX>)
tS32 NetCalcSizeDecider(tNet_contents* pContents) {
    tS32 the_decider;

    return 0;
}

// IDA: tNet_message* __usercall NetBuildMessage@<EAX>(tNet_message_type pType@<EAX>, tS32 pSize_decider@<EDX>)
// FUNCTION: CARM95 0x00447e7f
tNet_message* NetBuildMessage(tNet_message_type pType, tS32 pSize_decider) {
    tNet_message* the_message;
    tU32 the_size;

    the_size = NetGetMessageSize(pType, pSize_decider);
    the_message = NetAllocateMessage(the_size);
    if (the_message != NULL) {
        the_message->num_contents = 1;
        the_message->overall_size = the_size;
        the_message->contents.header.type = pType;
    }
    return the_message;
}

// IDA: tNet_contents* __usercall NetGetToHostContents@<EAX>(tNet_message_type pType@<EAX>, tS32 pSize_decider@<EDX>)
// FUNCTION: CARM95 0x00447ee4
tNet_contents* NetGetToHostContents(tNet_message_type pType, tS32 pSize_decider) {
    tU32 the_size;
    tNet_contents* contents;

    the_size = NetGetContentsSize(pType, pSize_decider);
    if (gTo_host_stack && the_size + gTo_host_stack->overall_size > MAX_MESAGE_STACK_SIZE) {
        NetSendMessageToHost(gCurrent_net_game, gTo_host_stack);
        gTo_host_stack = 0;
    }
    if (!gTo_host_stack) {
        gTo_host_stack = NetAllocateMessage(MAX_MESAGE_STACK_SIZE);
        gTo_host_stack->overall_size = offsetof(tNet_message, contents);
        gTo_host_stack->num_contents = 0;
    }
    contents = (tNet_contents*)((char*)gTo_host_stack + gTo_host_stack->overall_size);
    gTo_host_stack->overall_size += the_size;
    contents->header.type = pType;
    contents->header.contents_size = the_size;
    gTo_host_stack->num_contents++;
    return contents;
}

// IDA: tNet_contents* __usercall NetGetBroadcastContents@<EAX>(tNet_message_type pType@<EAX>, tS32 pSize_decider@<EDX>)
// FUNCTION: CARM95 0x00447fcc
tNet_contents* NetGetBroadcastContents(tNet_message_type pType, tS32 pSize_decider) {
    tU32 the_size;
    tNet_contents* contents;

    the_size = NetGetContentsSize(pType, pSize_decider);
    if (gBroadcast_stack && the_size + gBroadcast_stack->overall_size > MAX_MESAGE_STACK_SIZE) {
        NetSendMessageToAllPlayers(gCurrent_net_game, gBroadcast_stack);
        gBroadcast_stack = NULL;
    }
    if (gBroadcast_stack == NULL) {
        gBroadcast_stack = NetAllocateMessage(MAX_MESAGE_STACK_SIZE);
        gBroadcast_stack->overall_size = offsetof(tNet_message, contents);
        gBroadcast_stack->num_contents = 0;
    }
    contents = (tNet_contents*)((char*)gBroadcast_stack + gBroadcast_stack->overall_size);
    gBroadcast_stack->overall_size += the_size;
    contents->header.type = pType;
    contents->header.contents_size = the_size;
    gBroadcast_stack->num_contents++;
    return contents;
}

// IDA: void __cdecl NetSendMessageStacks()
// FUNCTION: CARM95 0x004480b4
void NetSendMessageStacks(void) {

    gLast_flush_message = PDGetTotalTime();
    if (gBroadcast_stack != NULL) {
        NetSendMessageToAllPlayers(gCurrent_net_game, gBroadcast_stack);
        gBroadcast_stack = NULL;
    }
    if (gTo_host_stack != NULL) {
        NetSendMessageToHost(gCurrent_net_game, gTo_host_stack);
        gTo_host_stack = NULL;
    }
}

// IDA: tNet_message* __usercall NetAllocateMessage@<EAX>(int pSize@<EAX>)
// FUNCTION: CARM95 0x0044811f
tNet_message* NetAllocateMessage(int pSize) {
    void* pointer;
    void* last_message;
    char* test;
    // GLOBAL: CARM95 0x50d2d0
    static int rr_min;
    // GLOBAL: CARM95 0x50d2d4
    static int rr_mid;
    // GLOBAL: CARM95 0x50d2d8
    static int rr_max;
    tNet_message* message;
    int i;

    pointer = NULL;
    if (pSize <= sizeof(tMin_message) - sizeof(void*)) {
        for (i = 0; i < MIN_MESSAGES_CAPACITY; i++) {
            if (((tNet_message*)&gMin_messages[rr_min])->contents.header.type == NETMSGID_NONE) {
                pointer = &gMin_messages[rr_min];
                break;
            }
            rr_min++;
            if (rr_min >= MIN_MESSAGES_CAPACITY) {
                rr_min = 0;
            }
        }
    }
    if (pointer == NULL && pSize <= sizeof(tMid_message) - sizeof(void*)) {
        for (i = 0; i < MID_MESSAGES_CAPACITY; i++) {
            if (((tNet_message*)&gMid_messages[rr_mid])->contents.header.type == NETMSGID_NONE) {
                pointer = &gMid_messages[rr_mid];
                break;
            }
            rr_mid++;
            if (rr_mid >= MID_MESSAGES_CAPACITY) {
                rr_mid = 0;
            }
        }
    }
    if (pointer == NULL && pSize <= sizeof(tMax_message) - sizeof(void*)) {
        for (i = 0; i < MAX_MESSAGES_CAPACITY; i++) {
            if (((tNet_message*)&gMax_messages[rr_max])->contents.header.type == NETMSGID_NONE) {
                pointer = &gMax_messages[rr_max];
                break;
            }
            rr_max++;
            if (rr_max >= MAX_MESSAGES_CAPACITY) {
                rr_max = 0;
            }
        }
    }
    if (pointer == NULL) {
        pointer = BrMemAllocate(gMessage_header_size + pSize + sizeof(void*), kMem_dynamic_message);
        if (pointer != NULL) {
            *(void**)pointer = NULL;
            if (gMessage_to_free != NULL) {
                for (last_message = gMessage_to_free; *(void**)last_message != NULL; last_message = *(void**)last_message) {
                }
                *(void**)last_message = pointer;
            } else {
                gMessage_to_free = pointer;
            }
            pointer = (char*)pointer + sizeof(void*);
        }
    }
    if (pointer == NULL) {
        LOG_PANIC("null pointer!");
        message = NULL;
    } else {
        message = (tNet_message*)((tU8*)pointer + gMessage_header_size);
        message->guarantee_number = 0;
        message->version = 1;
        message->magic_number = 0x763a5058;
    }
    return message;
}

// IDA: void __cdecl NetFreeExcessMemory()
// FUNCTION: CARM95 0x004483eb
void NetFreeExcessMemory(void) {
    void* temp;

    while (gMessage_to_free != NULL && ((tNet_message*)((char*)gMessage_to_free + sizeof(void*)))->contents.header.type == NETMSGID_NONE) {
        temp = *(void**)gMessage_to_free;
        BrMemFree(gMessage_to_free);
        gMessage_to_free = temp;
    }
}

// IDA: int __usercall NetDisposeMessage@<EAX>(tNet_game_details *pDetails@<EAX>, tNet_message *pMessage@<EDX>)
// FUNCTION: CARM95 0x00448445
int NetDisposeMessage(tNet_game_details* pDetails, tNet_message* pMessage) {

    if (pMessage->guarantee_number != 0) {
        return -1;
    }
    pMessage->contents.header.type = NETMSGID_NONE;
    return 0;
}

// IDA: tNet_message* __usercall NetGetNextMessage@<EAX>(tNet_game_details *pDetails@<EAX>, void **pSender_address@<EDX>)
// FUNCTION: CARM95 0x00448475
tNet_message* NetGetNextMessage(tNet_game_details* pDetails, void** pSender_address) {

    return PDNetGetNextMessage(pDetails, pSender_address);
}

// IDA: void __usercall ReceivedSendMeDetails(tNet_contents *pContents@<EAX>, void *pSender_address@<EDX>)
// FUNCTION: CARM95 0x0044929e
void ReceivedSendMeDetails(tNet_contents* pContents, void* pSender_address) {
    tNet_message* message;

    if (gDont_allow_joiners) {
        return;
    }
    message = NetBuildMessage(NETMSGID_DETAILS, 0);
    memcpy(&message->contents.data.details.details.host_name, gCurrent_net_game->host_name, sizeof(*gCurrent_net_game) - offsetof(tNet_game_details, host_name));
    NetSendMessageToAddress(gCurrent_net_game, message, pSender_address);
}

// IDA: void __usercall ReceivedDetails(tNet_contents *pContents@<EAX>)
// FUNCTION: CARM95 0x004492f6
void ReceivedDetails(tNet_contents* pContents) {

    if (gCurrent_join_poll_game == NULL) {
        return;
    }
    gBastard_has_answered = 1;
    memcpy(gCurrent_join_poll_game->host_name, &pContents->data.details.details.host_name, sizeof(*gCurrent_join_poll_game) - offsetof(tNet_game_details, host_name));
}

// IDA: void __cdecl SendOutPlayerList()
// FUNCTION: CARM95 0x004495e0
void SendOutPlayerList(void) {
    tNet_message* message;
    int i;

    gCurrent_net_game->num_players = gNumber_of_net_players;
    for (i = 0; i < gNumber_of_net_players; i++) {
        message = NetBuildMessage(NETMSGID_NEWPLAYERLIST, 1);
        message->contents.data.player_list.number_of_players = gNumber_of_net_players;
        message->contents.data.player_list.this_index = i;
        message->contents.data.player_list.batch_number = gPlayer_list_batch_number;
        memcpy(&message->contents.data.player_list.player, &gNet_players[i], sizeof(gNet_players[i]));
        NetGuaranteedSendMessageToAllPlayers(gCurrent_net_game, message, 0);
    }
    gPlayer_list_batch_number++;
    if (gInitialised_grid) {
        SetUpNetCarPositions();
        if (gStart_race_sent) {
            gNeed_to_send_start_race = 1;
            for (i = 1; i < gNumber_of_net_players; i++) {
                gNet_players[i].awaiting_confirmation = 1;
            }
        }
    }
}

// IDA: void __usercall ReceivedJoin(tNet_contents *pContents@<EAX>, void *pSender_address@<EDX>)
// FUNCTION: CARM95 0x00449330
void ReceivedJoin(tNet_contents* pContents, void* pSender_address) {
    int i;
    int new_player_count;
    int slot_index;
    tNet_message* message;
    tNet_game_player_info* new_players;

    new_player_count = gNumber_of_net_players;
    new_players = BrMemAllocate((new_player_count + 1) * sizeof(tNet_game_player_info), kMem_player_list_join);
    memcpy(new_players, gNet_players, gNumber_of_net_players * sizeof(tNet_game_player_info));

    if ((!gCurrent_net_game->options.open_game && gProgram_state.racing) || gCurrent_net_game->num_players > 5 || gDont_allow_joiners) {
        message = NetBuildMessage(NETMSGID_NEWPLAYERLIST, 0);
        // Send player count = 0 when race has already begun or is full
        message->contents.data.player_list.number_of_players = 0;
        NetSendMessageToAddress(gCurrent_net_game, message, pSender_address);
    } else {
        for (i = 0; i < new_player_count; i++) {
            if (new_players[i].ID == pContents->data.join.player_info.ID) {
                return;
            }
        }
        slot_index = new_player_count;
        new_player_count++;
        if (pContents->data.join.player_info.car_index < 0) {
            pContents->data.join.player_info.car_index = PickARandomCar();
        } else {
            for (i = 0; i < gNumber_of_net_players; i++) {
                if (gNet_players[i].car_index == pContents->data.join.player_info.car_index) {
                    message = NetBuildMessage(NETMSGID_NEWPLAYERLIST, 0);
                    // Send player count = -1 when selected car is unavailable
                    message->contents.data.player_list.number_of_players = -1;
                    NetSendMessageToAddress(gCurrent_net_game, message, pSender_address);
                    return;
                }
            }
        }
        if (pContents->data.join.player_info.car_index >= 0) {
            gCar_details[pContents->data.join.player_info.car_index].ownership = eCar_owner_someone;
        }
        memcpy(&new_players[slot_index], &pContents->data.join.player_info, sizeof(tNet_game_player_info));
        new_players[slot_index].player_status = ePlayer_status_loading;
        new_players[slot_index].last_heard_from_him = PDGetTotalTime();
        new_players[slot_index].grid_position_set = 0;
        if (new_players[slot_index].player_name[0] == '\0') {
            sprintf(new_players[slot_index].player_name, "%s %d", "PLAYER", slot_index);
        }
        NetSetPlayerSystemInfo(&new_players[slot_index], pSender_address);
        NetPlayersChanged(new_player_count, new_players);
        BrMemFree(new_players);
        SendOutPlayerList();
    }
}
// IDA: void __usercall KickPlayerOut(tPlayer_ID pID@<EAX>)
// FUNCTION: CARM95 0x004496f8
void KickPlayerOut(tPlayer_ID pID) {
    int i;
    int j;
    int new_player_count;
    tNet_game_player_info* new_players;

    new_player_count = gNumber_of_net_players;
    new_players = (tNet_game_player_info*)BrMemAllocate(sizeof(tNet_game_player_info) * gNumber_of_net_players, kMem_player_list_leave);
    memcpy(new_players, gNet_players, sizeof(tNet_game_player_info) * new_player_count);
    for (i = 0; i < new_player_count; i++) {
        if (new_players[i].ID == pID) {
            if (new_players[i].car_index >= 0) {
                gCar_details[new_players[i].car_index].ownership = eCar_owner_none;
            }
            if (new_players[i].next_car_index >= 0 && gCurrent_net_game->options.random_car_choice && (gCurrent_net_game->options.car_choice == eNet_car_all || gCurrent_net_game->options.car_choice == eNet_car_both)) {
                gCar_details[new_players[i].next_car_index].ownership = eCar_owner_none;
            }
#ifdef DETHRACE_FIX_BUGS
            for (j = i; j < new_player_count - 1; j++) {
#else
            for (j = i; j < new_player_count; j++) {
#endif
                memcpy(&new_players[j], &new_players[j + 1], sizeof(tNet_game_player_info));
            }
            new_player_count--;
            break;
        }
    }
    NetPlayersChanged(new_player_count, new_players);
    BrMemFree(new_players);
    SendOutPlayerList();
}

// IDA: void __usercall ReceivedLeave(tNet_contents *pContents@<EAX>, tNet_message *pMessage@<EDX>)
// FUNCTION: CARM95 0x004496de
void ReceivedLeave(tNet_contents* pContents, tNet_message* pMessage) {

    KickPlayerOut(pMessage->sender);
}

// IDA: void __usercall NetFullScreenMessage(int pStr_index@<EAX>, int pLeave_it_up_there@<EDX>)
// FUNCTION: CARM95 0x00448495
void NetFullScreenMessage(int pStr_index, int pLeave_it_up_there) {
    tU32 start_time;
    char* s;
    // Jeff: added underscore suffix to avoid collisions with samed-named globals
    int gPixel_buffer_size_;
    char* gPixels_copy_;
    char* gPalette_copy_;
    int restore_screen;

    if (pLeave_it_up_there || (gProgram_state.racing && !gInterface_within_race_mode)) {
        restore_screen = 0;
    } else {
        gPixel_buffer_size_ = gBack_screen->height * gBack_screen->row_bytes;
        gPixels_copy_ = BrMemAllocate(gPixel_buffer_size_, 0xB0u);
        gPalette_copy_ = BrMemAllocate(0x400u, 0xB1u);
        memcpy(gPixels_copy_, gBack_screen->pixels, gPixel_buffer_size_);
        memcpy(gPalette_copy_, gCurrent_palette_pixels, 0x400u);
        restore_screen = 1;
    }
    FadePaletteDown();
    LoadFont(FONT_MEDIUMHD);
    ClearEntireScreen();
    if (pStr_index <= 0) {
        s = "FIXED THAT YOU TWISTED BASTARDS";
    } else {
        s = GetMiscString(pStr_index);
    }
    OoerrIveGotTextInMeBoxMissus(
        FONT_MEDIUMHD,
        s,
        gBack_screen,
        0,
        gGraf_specs[gGraf_spec_index].total_height / 2 - gFonts[kFont_MEDIUMHD].height,
        gGraf_specs[gGraf_spec_index].total_width,
        gGraf_specs[gGraf_spec_index].total_height,
        1);
    PDScreenBufferSwap(0);
    EnsureRenderPalette();
    EnsurePaletteUp();
    if (!pLeave_it_up_there) {
        start_time = PDGetTotalTime();
        while (PDGetTotalTime() - start_time < 3000) {
            ;
        }
        FadePaletteDown();
        if (restore_screen) {
            memcpy(gBack_screen->pixels, gPixels_copy_, gPixel_buffer_size_);
            memcpy(gCurrent_palette_pixels, gPalette_copy_, 0x400u);
#ifdef DETHRACE_3DFX_PATCH
            g16bit_palette_valid = 0;
#endif
            BrMemFree(gPixels_copy_);
            BrMemFree(gPalette_copy_);
            PDScreenBufferSwap(0);
            FadePaletteUp();
        } else {
            ClearEntireScreen();
        }
    }
}

// IDA: void __usercall HostHasBittenTheDust(int pMessage_index@<EAX>)
// FUNCTION: CARM95 0x0044989e
void HostHasBittenTheDust(int pMessage_index) {

    if (!gHost_died) {
        gHost_died = 1;
        NetFullScreenMessage(pMessage_index, 0);
        gProgram_state.prog_status = eProg_idling;
    }
}

// IDA: void __usercall ReceivedHosticide(tNet_contents *pContents@<EAX>)
// FUNCTION: CARM95 0x00449889
void ReceivedHosticide(tNet_contents* pContents) {

    HostHasBittenTheDust(kMiscString_GAME_TERMINATED_BY_HOST);
}

// IDA: void __cdecl ConfirmReceipt()
void ConfirmReceipt(void) {
    tNet_message* the_message;
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ReceivedNewPlayerList(tNet_contents *pContents@<EAX>, tNet_message *pM@<EDX>)
// FUNCTION: CARM95 0x004498d8
void ReceivedNewPlayerList(tNet_contents* pContents, tNet_message* pM) {
    int i;

    if (pContents->data.player_list.number_of_players <= 0) {
        gJoin_request_denied = 1;
        if (pContents->data.player_list.number_of_players < 0) {
            gCar_was_taken = 1;
        }
        return;
    }
    if (pContents->data.player_list.batch_number >= gReceiving_batch_number) {
        if (!gReceiving_new_players) {
            gLast_player_list_received = pM->senders_time_stamp;
            for (i = 0; i < COUNT_OF(gNew_net_players); i++) {
                gNew_net_players[i].car_index = -1;
            }
            gReceiving_new_players = 1;
            gReceiving_batch_number = pContents->data.player_list.batch_number;
        }
        if (pContents->data.player_list.batch_number <= gReceiving_batch_number) {
            memcpy(&gNew_net_players[pContents->data.player_list.this_index], &pContents->data.player_list.player, sizeof(tNet_game_player_info));
            for (i = 0; i < pContents->data.player_list.number_of_players; i++) {
                if (gNew_net_players[i].car_index < 0) {
                    return;
                }
            }
            gReceiving_new_players = 0;
            NetPlayersChanged(pContents->data.player_list.number_of_players, gNew_net_players);
            gThis_net_player_index = -1;
            for (i = 0; i < gNumber_of_net_players; i++) {
                if (gNet_players[i].ID == gLocal_net_ID) {
                    gThis_net_player_index = i;
                    break;
                }
            }
            if (gThis_net_player_index < 0) {
                FatalError(kFatalError_NotInReceivedPlayerList);
            }
            gNet_players[0].last_heard_from_him = PDGetTotalTime();
            gCurrent_race.number_of_racers = gNumber_of_net_players;
            if (gSynch_race_start) {
                for (i = 0; i < gNumber_of_net_players; i++) {
                    gCurrent_race.opponent_list[gNet_players[i].opponent_list_index].net_player_index = i;
                }
            }
        } else {
            gReceiving_new_players = 0;
            ReceivedNewPlayerList(pContents, pM);
        }
    }
}

// IDA: void __usercall ReceivedRaceOver(tNet_contents *pContents@<EAX>)
// FUNCTION: CARM95 0x00449afb
void ReceivedRaceOver(tNet_contents* pContents) {

    gRace_finished = 0;
    if (gProgram_state.racing && (gNet_mode == eNet_mode_client || pContents->data.race_over.reason == eRace_over_network_victory || pContents->data.race_over.reason == eRace_over_network_loss)) {
        RaceCompleted(pContents->data.race_over.reason);
    }
}

// IDA: void __usercall ReceivedStatusReport(tNet_contents *pContents@<EAX>, tNet_message *pMessage@<EDX>)
// FUNCTION: CARM95 0x00449b46
void ReceivedStatusReport(tNet_contents* pContents, tNet_message* pMessage) {
    int i;

    for (i = 0; i < gNumber_of_net_players; i++) {
        if (gNet_players[i].ID == pMessage->sender) {
            gNet_players[i].player_status = pContents->data.report.status;
            gNet_players[i].last_heard_from_him = PDGetTotalTime();
            if (gNet_players[i].player_status < ePlayer_status_racing || gNet_players[i].player_status == ePlayer_status_recovering) {
                if (gNet_players[i].player_status < ePlayer_status_racing) {
                    DisableCar(gNet_players[i].car);
                }
            } else {
                if (gNet_players[i].car->disabled) {
                    SendCurrentPowerups();
                }
                EnableCar(gNet_players[i].car);
            }
            return;
        }
    }
}

// IDA: void __usercall ReceivedStartRace(tNet_contents *pContents@<EAX>)
// FUNCTION: CARM95 0x00449c42
void ReceivedStartRace(tNet_contents* pContents) {
    int i;
    int index;

    if (pContents->data.start_race.car_count == -1) {
        if (gProgram_state.racing) {
            index = pContents->data.start_race.car_list[0].index;
            BrMatrix34Copy(&gNet_players[index].car->car_master_actor->t.t.mat, &pContents->data.start_race.car_list[0].mat);
            ReinitialiseCar(gNet_players[index].car);
            if (gThis_net_player_index == index) {
                if (!gInterface_within_race_mode) {
                    FadePaletteDown();
                }
                CancelPendingCunningStunt();
                gProgram_state.credits_earned = gInitial_net_credits[gCurrent_net_game->options.starting_money_index];
                gProgram_state.credits_lost = 0;
                InitialiseExternalCamera();
            }
            gNet_players[index].last_waste_message = 0;
            gNet_players[index].wasteage_attributed = 0;
        }
    } else if (gSynch_race_start) {
        for (i = 0; i < pContents->data.start_race.car_count; i++) {
            gNet_players[pContents->data.start_race.car_list[i].index].next_car_index = pContents->data.start_race.car_list[i].next_car_index;
        }
    } else {
        for (i = 0; i < pContents->data.start_race.car_count; i++) {
            gCurrent_race.number_of_racers = i + 1;
            gCurrent_race.opponent_list[i].index = -1;
            gCurrent_race.opponent_list[i].ranking = -1;
            gCurrent_race.opponent_list[i].car_spec = gNet_players[pContents->data.start_race.car_list[i].index].car;
            gCurrent_race.opponent_list[i].net_player_index = pContents->data.start_race.car_list[i].index;
            gNet_players[gCurrent_race.opponent_list[i].net_player_index].last_waste_message = 0;
            gNet_players[gCurrent_race.opponent_list[i].net_player_index].wasteage_attributed = 0;
            if (!gProgram_state.racing || gCurrent_race.opponent_list[i].car_spec->driver != eDriver_local_human) {
                BrMatrix34Copy(&gCurrent_race.opponent_list[i].car_spec->car_master_actor->t.t.mat, &pContents->data.start_race.car_list[i].mat);
                InitialiseCar(gCurrent_race.opponent_list[i].car_spec);
            }
            gNet_players[pContents->data.start_race.car_list[i].index].next_car_index = pContents->data.start_race.car_list[i].next_car_index;
        }
        gPending_race = pContents->data.start_race.next_race;
        gCurrent_race.number_of_racers = pContents->data.start_race.car_count;
        gSynch_race_start = 1;
        if (!pContents->data.start_race.racing || gProgram_state.racing) {
            gWait_for_it = 0;
        }
    }
}

// IDA: void __usercall ReceivedGuaranteeReply(tNet_contents *pContents@<EAX>)
// FUNCTION: CARM95 0x00448695
void ReceivedGuaranteeReply(tNet_contents* pContents) {
    int i;

    for (i = 0; i < gNext_guarantee; i++) {
        if (gGuarantee_list[i].guarantee_number == pContents->data.reply.guarantee_number) {
            gGuarantee_list[i].recieved = 1;
        }
    }
}

// IDA: void __usercall ReceivedHeadup(tNet_contents *pContents@<EAX>)
// FUNCTION: CARM95 0x004486fb
void ReceivedHeadup(tNet_contents* pContents) {

    if (gProgram_state.racing) {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 3000, -4, pContents->data.headup.text);
    }
}

// IDA: void __usercall ReceivedHostQuery(tNet_contents *pContents@<EAX>, tNet_message *pMessage@<EDX>)
// FUNCTION: CARM95 0x0044872d
void ReceivedHostQuery(tNet_contents* pContents, tNet_message* pMessage) {
    tNet_message* message;

    message = NetBuildMessage(NETMSGID_HOSTREPLY, 0);
    message->contents.data.heres_where_we_at.race_has_started = gProgram_state.racing;
    message->contents.data.heres_where_we_at.race_index = gProgram_state.current_race_index;
    message->contents.data.heres_where_we_at.pending_race = gPending_race;
    NetGuaranteedSendMessageToPlayer(gCurrent_net_game, message, pMessage->sender, NULL);
    if (gProgram_state.racing) {
        SignalToStartRace();
        UpdateEnvironments();
    }
}

// IDA: void __usercall ReceivedHostReply(tNet_contents *pContents@<EAX>)
// FUNCTION: CARM95 0x0044879d
void ReceivedHostReply(tNet_contents* pContents) {
    tNet_message* message;

    if (pContents->data.heres_where_we_at.race_index != gProgram_state.current_race_index) {
        NetLeaveGame(gCurrent_net_game);
        NetFullScreenMessage(kMiscString_RACE_CHANGED_DURING_LOADING, 0);
        gProgram_state.prog_status = eProg_idling;
    }
    if (pContents->data.heres_where_we_at.race_has_started) {
        if (gCurrent_net_game->options.open_game) {
            gPending_race = pContents->data.heres_where_we_at.pending_race;
        } else {
            NetFullScreenMessage(kMiscString_SORRY_YOU_RE_TOO_LATE, 0);
            gProgram_state.prog_status = eProg_idling;
        }
    }
}

// IDA: void __usercall SendGuaranteeReply(tNet_message *pMessage@<EAX>, void *pSender_address@<EDX>)
// FUNCTION: CARM95 0x0044882b
void SendGuaranteeReply(tNet_message* pMessage, void* pSender_address) {
    tNet_message* message;

    message = NetBuildMessage(NETMSGID_GUARANTEEREPLY, 0);
    message->contents.data.reply.guarantee_number = pMessage->guarantee_number;
    pMessage->guarantee_number = 0;
    NetSendMessageToAddress(gCurrent_net_game, message, pSender_address);
}

// IDA: int __usercall PlayerIsInList@<EAX>(tPlayer_ID pID@<EAX>)
// FUNCTION: CARM95 0x00449f18
int PlayerIsInList(tPlayer_ID pID) {
    int i;

    for (i = 0; i < gNumber_of_net_players; i++) {
        if (gNet_players[i].ID == pID) {
            gNet_players[i].last_heard_from_him = PDGetTotalTime();
            return 1;
        }
    }
    return 0;
}

// IDA: void __usercall ReceivedTimeSync(tNet_contents *pContents@<EAX>, tNet_message *pMessage@<EDX>, tU32 pReceive_time@<EBX>)
// FUNCTION: CARM95 0x00448874
void ReceivedTimeSync(tNet_contents* pContents, tNet_message* pMessage, tU32 pReceive_time) {

    if (pMessage->senders_time_stamp - pContents->data.time_sync.race_start_time > pReceive_time + 10) {
        gRace_start -= pMessage->senders_time_stamp - pContents->data.time_sync.race_start_time - pReceive_time;
    }
}

// IDA: void __usercall ReceivedConfirm(tNet_contents *pContents@<EAX>)
// FUNCTION: CARM95 0x004488b4
void ReceivedConfirm(tNet_contents* pContents) {
    int i;

    for (i = 0; i < gNumber_of_net_players; i++) {
        if (gNet_players[i].ID == pContents->data.confirm.player) {
            gNet_players[i].awaiting_confirmation = 0;
            return;
        }
    }
}

// IDA: void __usercall ReceivedDisableCar(tNet_contents *pContents@<EAX>)
// FUNCTION: CARM95 0x00448914
void ReceivedDisableCar(tNet_contents* pContents) {
}

// IDA: void __usercall ReceivedEnableCar(tNet_contents *pContents@<EAX>)
// FUNCTION: CARM95 0x0044891f
void ReceivedEnableCar(tNet_contents* pContents) {
}

// IDA: void __usercall ReceivedScores(tNet_contents *pContents@<EAX>)
// FUNCTION: CARM95 0x0044892a
void ReceivedScores(tNet_contents* pContents) {
    int i;

    UseGeneralScore(pContents->data.scores.general_score);
    for (i = 0; i < gNumber_of_net_players; i++) {
        gNet_players[i].score = pContents->data.scores.scores[i];
    }
}

// IDA: void __usercall ReceivedWasted(tNet_contents *pContents@<EAX>)
// FUNCTION: CARM95 0x00448981
void ReceivedWasted(tNet_contents* pContents) {
    tNet_game_player_info* victim;
    tNet_game_player_info* culprit;
    char s[256];
    tCar_spec* car;
    // GLOBAL: CARM95 0x50d2dc
    static tS32 last_got_wasted_time;
    // GLOBAL: CARM95 0x50d2e0
    static tS32 last_wasted_em_time;
    // GLOBAL: CARM95 0x50d2e4
    static tS32 last_wasty_message_time;
    // GLOBAL: CARM95 0x50d2e8
    static tNet_game_player_info* last_culprit;
    // GLOBAL: CARM95 0x50d2ec
    static tNet_game_player_info* last_victim;

    victim = NetPlayerFromID(pContents->data.wasted.victim);
    if (victim == NULL) {
        return;
    }
    victim->car->knackered = 1;
    if (pContents->data.wasted.victim == gLocal_net_ID) {
        if (gCurrent_net_game->type == eNet_game_type_fight_to_death) {
            DoFancyHeadup(kFancyHeadupYouLost);
            gRace_finished = 1;
        } else {
            last_got_wasted_time = PDGetTotalTime();
            if (last_got_wasted_time - last_wasted_em_time > 1000) {
                DoFancyHeadup(kFancyHeadupYouAreWasted);
            } else {
                DoFancyHeadup(kFancyHeadupYouAreBothWasted);
            }
        }
    }
    if (pContents->data.wasted.culprit == -1) {
        if (victim->last_waste_message == 0) {
            victim->last_waste_message = GetTotalTime();
        }
    } else if (!victim->wasteage_attributed) {
        if (pContents->data.wasted.culprit == -2) {
            culprit = NULL;
        } else {
            culprit = NetPlayerFromID(pContents->data.wasted.culprit);
        }
        if (culprit != NULL && gNet_mode == eNet_mode_host && gCurrent_net_game->type == eNet_game_type_car_crusher) {
            culprit->score++;
            if (culprit->score >= gCurrent_net_game->options.race_end_target) {
                DeclareWinner(culprit - gNet_players);
            }
        }
        victim->last_waste_message = GetTotalTime();
        victim->wasteage_attributed = 1;
        if (victim == last_culprit && culprit == last_victim && victim != NULL && culprit != NULL && PDGetTotalTime() - last_wasty_message_time < 1000) {
            sprintf(s, "%s %s %s %s", victim->player_name, GetMiscString(kMiscString_AND), culprit->player_name, GetMiscString(kMiscString_WASTED_EACH_OTHER));
        } else {
            sprintf(s, "%s %s %s", victim->player_name, GetMiscString(kMiscString_WastedBy), culprit ? culprit->player_name : GetMiscString(kMiscString_COP));
        }
        NewTextHeadupSlot2(eHeadupSlot_misc, 0, 3000, -4, s, 0);
        last_wasty_message_time = PDGetTotalTime();
        last_culprit = culprit;
        last_victim = victim;
        if (pContents->data.wasted.culprit == gLocal_net_ID) {
            PratcamEvent(kPratcam_opponent_wasted);
            last_wasted_em_time = PDGetTotalTime();
            if (last_wasted_em_time - last_got_wasted_time > 1000) {
                DoFancyHeadup(kFancyHeadupYouWastedEm);
            } else {
                DoFancyHeadup(kFancyHeadupYouAreBothWasted);
            }
        }
    }
}

// IDA: void __usercall ReceivedCarDetailsReq(tNet_contents *pContents@<EAX>, void *pSender_address@<EDX>)
// FUNCTION: CARM95 0x00448ca9
void ReceivedCarDetailsReq(tNet_contents* pContents, void* pSender_address) {
    tNet_message* message;
    int i;

    message = NetBuildMessage(NETMSGID_CARDETAILS, 0);
    message->contents.data.car_details.count = gNumber_of_net_players;
    for (i = 0; i < gNumber_of_net_players; i++) {
        message->contents.data.car_details.details[i].car_index = gNet_players[i].car_index;
        // truncates from 32 to 16 characters
        memcpy(message->contents.data.car_details.details[i].owner, gNet_players[i].player_name, sizeof(message->contents.data.car_details.details[i].owner));
        message->contents.data.car_details.details[i].owner[sizeof(message->contents.data.car_details.details[i].owner) - 1] = '\0';
    }
    NetGuaranteedSendMessageToAddress(gCurrent_net_game, message, pSender_address, NULL);
}

// IDA: void __usercall ReceivedCarDetails(tNet_contents *pContents@<EAX>)
// FUNCTION: CARM95 0x00448d70
void ReceivedCarDetails(tNet_contents* pContents) {
    int i;
    int j;

    SetNetAvailability(gNet_options);
    for (i = 0; i < gNumber_of_racers; i++) {
        for (j = 0; j < pContents->data.car_details.count; j++) {
            if (i == pContents->data.car_details.details[j].car_index) {
                gCar_details[i].ownership = eCar_owner_someone;
                strcpy(gCar_details[i].name, pContents->data.car_details.details[j].owner);
            }
        }
    }
    gReceived_car_details = 1;
}

// IDA: void __usercall ReceivedGameScores(tNet_contents *pContents@<EAX>)
// FUNCTION: CARM95 0x00448e41
void ReceivedGameScores(tNet_contents* pContents) {
    int i;

    gReceived_game_scores = 1;
    for (i = 0; i < gNumber_of_net_players; i++) {
        gNet_players[i].played = pContents->data.game_scores.scores[i].played;
        gNet_players[i].won = pContents->data.game_scores.scores[i].won;
        gNet_players[i].games_score = pContents->data.game_scores.scores[i].score;
    }
}

// IDA: void __usercall ReceivedMessage(tNet_message *pMessage@<EAX>, void *pSender_address@<EDX>, tU32 pReceive_time@<EBX>)
// FUNCTION: CARM95 0x00448eca
void ReceivedMessage(tNet_message* pMessage, void* pSender_address, tU32 pReceive_time) {
    tNet_contents* contents;
    int i;

    contents = &pMessage->contents;
    if (pMessage->guarantee_number != 0) {
        SendGuaranteeReply(pMessage, pSender_address);
    }
    if (!gProgram_state.racing && gRace_only_flags[pMessage->contents.header.type]) {
        return;
    }
    if (gOnly_receive_guarantee_replies && pMessage->contents.header.type != NETMSGID_GUARANTEEREPLY) {
        return;
    }

    for (i = 0; i < pMessage->num_contents; i++) {
        if (contents->header.type <= NETMSGID_CARDETAILS || PlayerIsInList(pMessage->sender)) {
            switch (contents->header.type) {
            case NETMSGID_SENDMEDETAILS: // 0x00,
                ReceivedSendMeDetails(contents, pSender_address);
                break;
            case NETMSGID_DETAILS: // 0x01,
                ReceivedDetails(contents);
                break;
            case NETMSGID_JOIN: // 0x02,
                ReceivedJoin(contents, pSender_address);
                break;
            case NETMSGID_NEWPLAYERLIST: // 0x03,
                ReceivedNewPlayerList(contents, pMessage);
                break;
            case NETMSGID_GUARANTEEREPLY: // 0x04,
                ReceivedGuaranteeReply(contents);
                break;
            case NETMSGID_CARDETAILSREQ: // 0x05,
                ReceivedCarDetailsReq(contents, pSender_address);
                break;
            case NETMSGID_CARDETAILS: // 0x06,
                ReceivedCarDetails(contents);
                break;
            case NETMSGID_LEAVE: // 0x07,
                ReceivedLeave(contents, pMessage);
                break;
            case NETMSGID_HOSTICIDE: // 0x08,
                ReceivedHosticide(contents);
                break;
            case NETMSGID_RACEOVER: // 0x09,
                ReceivedRaceOver(contents);
                break;
            case NETMSGID_STATUSREPORT: // 0x0a,
                ReceivedStatusReport(contents, pMessage);
                break;
            case NETMSGID_STARTRACE: // 0x0b,
                ReceivedStartRace(contents);
                break;
            case NETMSGID_HEADUP: // 0x0c,
                ReceivedHeadup(contents);
                break;
            case NETMSGID_HOSTQUERY: // 0x0d,
                ReceivedHostQuery(contents, pMessage);
                break;
            case NETMSGID_HOSTREPLY: // 0x0e,
                ReceivedHostReply(contents);
                break;
            case NETMSGID_MECHANICS: // 0x0f,
                ReceivedMechanics(contents);
                break;
            case NETMSGID_NONCAR_INFO: // 0x10,
                ReceivedNonCar(contents);
                break;
            case NETMSGID_TIMESYNC: // 0x11,
                ReceivedTimeSync(contents, pMessage, pReceive_time);
                break;
            case NETMSGID_CONFIRM: // 0x12,
                ReceivedConfirm(contents);
                break;
            case NETMSGID_DISABLECAR: // 0x13,
                ReceivedDisableCar(contents);
                break;
            case NETMSGID_ENABLECAR: // 0x14,
                ReceivedEnableCar(contents);
                break;
            case NETMSGID_POWERUP: // 0x15,
                ReceivedPowerup(contents);
                break;
            case NETMSGID_RECOVER: // 0x16,
                ReceivedRecover(contents);
                break;
            case NETMSGID_SCORES: // 0x17,
                ReceivedScores(contents);
                break;
            case NETMSGID_WASTED: // 0x18,
                ReceivedWasted(contents);
                break;
            case NETMSGID_PEDESTRIAN: // 0x19,
                ReceivedPedestrian(contents, pMessage, pReceive_time);
                break;
            case NETMSGID_GAMEPLAY: // 0x1a,
                ReceivedGameplay(contents, pMessage, pReceive_time);
                break;
            case NETMSGID_NONCARPOSITION: // 0x1b,
                ReceivedNonCarPosition(contents);
                break;
            case NETMSGID_COPINFO: // 0x1c,
                ReceivedCopInfo(contents);
                break;
            case NETMSGID_GAMESCORES: // 0x1d,
                ReceivedGameScores(contents);
                break;
            case NETMSGID_OILSPILL: // 0x1e,
                ReceivedOilSpill(contents);
                break;
            case NETMSGID_CRUSHPOINT: // 0x1f,
                RecievedCrushPoint(contents);
                break;
            }
        }
        contents = (tNet_contents*)((tU8*)contents + contents->header.contents_size);
    }
}

// IDA: void __cdecl NetReceiveAndProcessMessages()
// FUNCTION: CARM95 0x00449f82
void NetReceiveAndProcessMessages(void) {
    tNet_message* message;
    void* sender_address;
    tU32 receive_time;
    int old_net_service;

    old_net_service = gIn_net_service;
    if (gNet_mode != eNet_mode_none || gJoin_list_mode) {
        gIn_net_service = 1;
        while ((message = NetGetNextMessage(gCurrent_net_game, &sender_address)) != NULL) {
            receive_time = GetRaceTime();
            if (message->magic_number == 0x763a5058) {
                CheckCheckSum(message);
                ReceivedMessage(message, sender_address, receive_time);
            } else {
                message->guarantee_number = 0;
            }
            NetDisposeMessage(gCurrent_net_game, message);
        }
    }
    gIn_net_service = old_net_service;
}

// IDA: void __cdecl BroadcastStatus()
// FUNCTION: CARM95 0x0044a046
void BroadcastStatus(void) {
    tNet_message* message;

    message = NetBuildMessage(NETMSGID_STATUSREPORT, 0);
    message->contents.data.report.status = gNet_players[gThis_net_player_index].player_status;
    NetSendMessageToAllPlayers(gCurrent_net_game, message);
}

// IDA: void __cdecl CheckForDisappearees()
// FUNCTION: CARM95 0x0044a2a3
void CheckForDisappearees(void) {
    int i;
    int j;
    tU32 the_time;
    char s[256];
    char* s2;

    the_time = PDGetTotalTime();
    if (gNet_mode == eNet_mode_host) {
        for (i = 0; i < gNumber_of_net_players; i++) {
            if (!gNet_players[i].host && gNet_players[i].last_heard_from_him != 0 && the_time - gNet_players[i].last_heard_from_him >= 20000) {
                strcpy(s, gNet_players[i].player_name);
                strcat(s, " ");
                strcat(s, GetMiscString(kMiscString_IS_NO_LONGER_RESPONDING));
                NetSendHeadupToAllPlayers(s);
                KickPlayerOut(gNet_players[i].ID);
                if (gProgram_state.racing) {
                    NewTextHeadupSlot(eHeadupSlot_misc, 0, 3000, -4, s);
                }
            }
        }
    } else if (!gHost_died && gNumber_of_net_players != 0 && gNet_players[0].last_heard_from_him != 0 && the_time - gNet_players[0].last_heard_from_him >= 20000) {
        HostHasBittenTheDust(kMiscString_PANIC_HOST_HAS_DISAPPEARED);
    }
}

// IDA: void __cdecl CheckForPendingStartRace()
// FUNCTION: CARM95 0x0044a478
void CheckForPendingStartRace(void) {
    int i;

    if (gNet_mode == eNet_mode_host && gNeed_to_send_start_race) {
        for (i = 1; i < gNumber_of_net_players; i++) {
            if (gNet_players[i].awaiting_confirmation) {
                return;
            }
        }
        SignalToStartRace();
    }
}

// IDA: void __usercall NetService(int pIn_race@<EAX>)
// FUNCTION: CARM95 0x0044a08a
void NetService(int pIn_race) {
    tU32 time;
    // GLOBAL: CARM95 0x50d2f0
    static tU32 last_status_broadcast;

    if (gIn_net_service || gNet_service_disable) {
        return;
    }
    time = PDGetTotalTime();
    gIn_net_service = 1;
    if (gJoin_list_mode) {
        NetFreeExcessMemory();
        DoNextJoinPoll();
        NetReceiveAndProcessMessages();
    } else {
        if (gNet_mode != eNet_mode_none) {
            NetFreeExcessMemory();
            if (!pIn_race) {
                NetReceiveAndProcessMessages();
            }
            if (time - last_status_broadcast > 1000) {
                last_status_broadcast = PDGetTotalTime();
                BroadcastStatus();
            }
            CheckForDisappearees();
            CheckForPendingStartRace();
        }
    }
    if (gJoin_list_mode || gNet_mode != eNet_mode_none) {
        ResendGuaranteedMessages();
        if (time > gLast_flush_message + 200) {
            NetSendMessageStacks();
        }
    }
    gIn_net_service = 0;
}

// IDA: void __usercall NetFinishRace(tNet_game_details *pDetails@<EAX>, tRace_over_reason pReason@<EDX>)
// FUNCTION: CARM95 0x0044a4e1
void NetFinishRace(tNet_game_details* pDetails, tRace_over_reason pReason) {
    tNet_message* the_message;

    gNeed_to_send_start_race = 0;
    the_message = NetBuildMessage(NETMSGID_RACEOVER, 0);
    the_message->contents.data.race_over.reason = pReason;
    NetGuaranteedSendMessageToAllPlayers(gCurrent_net_game, the_message, NULL);
}

// IDA: void __usercall NetPlayerStatusChanged(tPlayer_status pNew_status@<EAX>)
// FUNCTION: CARM95 0x0044a525
void NetPlayerStatusChanged(tPlayer_status pNew_status) {
    if (gNet_mode != eNet_mode_none && pNew_status != gNet_players[gThis_net_player_index].player_status) {
        gNet_players[gThis_net_player_index].player_status = pNew_status;
        BroadcastStatus();
        if (gProgram_state.current_car.disabled && pNew_status >= ePlayer_status_racing && pNew_status != ePlayer_status_recovering) {
            EnableCar(&gProgram_state.current_car);
        } else if (!gProgram_state.current_car.disabled && pNew_status < ePlayer_status_racing) {
            DisableCar(&gProgram_state.current_car);
        }
    }
}

// IDA: tPlayer_status __cdecl NetGetPlayerStatus()
// FUNCTION: CARM95 0x0044a5d0
tPlayer_status NetGetPlayerStatus(void) {

    return gNet_players[gThis_net_player_index].player_status;
}

// IDA: int __usercall NetGuaranteedSendMessageToAllPlayers@<EAX>(tNet_game_details *pDetails@<EAX>, tNet_message *pMessage@<EDX>, int (*pNotifyFail)(tU32, tNet_message*)@<EBX>)
// FUNCTION: CARM95 0x0044a5ef
int NetGuaranteedSendMessageToAllPlayers(tNet_game_details* pDetails, tNet_message* pMessage, int (*pNotifyFail)(tU32, tNet_message*)) {
    int i;
    int err;

    err = 0;
    if (gNumber_of_net_players == 1) {
        NetDisposeMessage(pDetails, pMessage);
        err = 0;
    } else {
        for (i = 0; i < gNumber_of_net_players; i++) {
            if (gThis_net_player_index != i) {
                err |= NetGuaranteedSendMessageToAddress(pDetails, pMessage, &gNet_players[i], pNotifyFail);
            }
        }
    }
    return err;
}

// IDA: int __usercall NetGuaranteedSendMessageToEverybody@<EAX>(tNet_game_details *pDetails@<EAX>, tNet_message *pMessage@<EDX>, int (*pNotifyFail)(tU32, tNet_message*)@<EBX>)
// FUNCTION: CARM95 0x0044a689
int NetGuaranteedSendMessageToEverybody(tNet_game_details* pDetails, tNet_message* pMessage, int (*pNotifyFail)(tU32, tNet_message*)) {

    pMessage->sender = gLocal_net_ID;
    pMessage->senders_time_stamp = PDGetTotalTime();
    pMessage->num_contents = 1;
    pMessage->guarantee_number = 0;
    ReceivedMessage(pMessage, &gNet_players[gThis_net_player_index], GetRaceTime());
    return NetGuaranteedSendMessageToAllPlayers(pDetails, pMessage, pNotifyFail);
}

// IDA: int __usercall NetGuaranteedSendMessageToHost@<EAX>(tNet_game_details *pDetails@<EAX>, tNet_message *pMessage@<EDX>, int (*pNotifyFail)(tU32, tNet_message*)@<EBX>)
// FUNCTION: CARM95 0x0044a6f9
int NetGuaranteedSendMessageToHost(tNet_game_details* pDetails, tNet_message* pMessage, int (*pNotifyFail)(tU32, tNet_message*)) {

    return NetGuaranteedSendMessageToAddress(pDetails, pMessage, &pDetails->pd_net_info, pNotifyFail);
}

// IDA: int __usercall NetGuaranteedSendMessageToPlayer@<EAX>(tNet_game_details *pDetails@<EAX>, tNet_message *pMessage@<EDX>, tPlayer_ID pPlayer@<EBX>, int (*pNotifyFail)(tU32, tNet_message*)@<ECX>)
// FUNCTION: CARM95 0x0044a721
int NetGuaranteedSendMessageToPlayer(tNet_game_details* pDetails, tNet_message* pMessage, tPlayer_ID pPlayer, int (*pNotifyFail)(tU32, tNet_message*)) {
    int i;

    for (i = 0; i <= gNumber_of_net_players; i++) {
        if (pPlayer == gNet_players[i].ID) {
            break;
        }
    }
    if (i == gNumber_of_net_players) {
        return -1;
    }
    if (gLocal_net_ID != pPlayer) {
        return NetGuaranteedSendMessageToAddress(pDetails, pMessage, &gNet_players[i].pd_net_info, pNotifyFail);
    }
    pMessage->sender = gLocal_net_ID;
    pMessage->senders_time_stamp = PDGetTotalTime();
    pMessage->num_contents = 1;
    pMessage->guarantee_number = 0;
    ReceivedMessage(pMessage, &gNet_players[i], GetRaceTime());
    NetDisposeMessage(pDetails, pMessage);
    return 0;
}

// IDA: int __usercall NetGuaranteedSendMessageToAddress@<EAX>(tNet_game_details *pDetails@<EAX>, tNet_message *pMessage@<EDX>, void *pAddress@<EBX>, int (*pNotifyFail)(tU32, tNet_message*)@<ECX>)
// FUNCTION: CARM95 0x0044a80f
int NetGuaranteedSendMessageToAddress(tNet_game_details* pDetails, tNet_message* pMessage, void* pAddress, int (*pNotifyFail)(tU32, tNet_message*)) {
    char buffer[256]; // Added by Dethrace

    if (gNet_mode == eNet_mode_none && !gJoin_list_mode) {
        return -3;
    }
    pMessage->sender = gLocal_net_ID;
    pMessage->senders_time_stamp = PDGetTotalTime();
    if (gNext_guarantee >= COUNT_OF(gGuarantee_list)) {
        sprintf(buffer, "Guarantee list full %d", pMessage->contents.header.type);
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 500, -1, buffer);
        pMessage->guarantee_number = 0;
        return 0;
    }
    pMessage->guarantee_number = gGuarantee_number;
    gGuarantee_list[gNext_guarantee].guarantee_number = gGuarantee_number;
    gGuarantee_number++;
    gGuarantee_list[gNext_guarantee].message = pMessage;
    gGuarantee_list[gNext_guarantee].send_time = PDGetTotalTime();
    gGuarantee_list[gNext_guarantee].next_resend_time = gGuarantee_list[gNext_guarantee].send_time + 100;
    gGuarantee_list[gNext_guarantee].resend_period = 100;
    memcpy(&gGuarantee_list[gNext_guarantee].pd_address, pAddress, sizeof(tPD_net_player_info));
    gGuarantee_list[gNext_guarantee].NotifyFail = pNotifyFail;
    gGuarantee_list[gNext_guarantee].recieved = 0;
    gNext_guarantee++;
    DoCheckSum(pMessage);
    return PDNetSendMessageToAddress(pDetails, pMessage, pAddress);
}

// IDA: void __cdecl ResendGuaranteedMessages()
// FUNCTION: CARM95 0x0044a9f1
void ResendGuaranteedMessages(void) {
    int i;
    int j;
    tU32 time;

    i = 0;
    time = PDGetTotalTime();
    for (j = 0; j < gNext_guarantee; j++) {
        if (i != j) {
            memcpy(&gGuarantee_list[i], &gGuarantee_list[j], sizeof(tGuaranteed_message));
        }
        if (!gGuarantee_list[i].recieved) {
            if (gGuarantee_list[i].NotifyFail != NULL) {
                gGuarantee_list[i].recieved |= gGuarantee_list[i].NotifyFail(time - gGuarantee_list[i].send_time, gGuarantee_list[i].message);
            } else {
                if (time - gGuarantee_list[i].send_time > 10000) {
                    gGuarantee_list[i].recieved = 1;
                }
            }
        }
        if (!gGuarantee_list[i].recieved) {
            if (time > gGuarantee_list[i].next_resend_time) {
                gGuarantee_list[i].message->guarantee_number = gGuarantee_list[i].guarantee_number;
                GetCheckSum(gGuarantee_list[i].message);
                PDNetSendMessageToAddress(gCurrent_net_game, gGuarantee_list[i].message, &gGuarantee_list[i].pd_address);
                gGuarantee_list[i].resend_period = (tU32)(gGuarantee_list[i].resend_period * 1.2f);
                gGuarantee_list[i].next_resend_time += gGuarantee_list[i].resend_period;
            }
            i++;
        } else if ((i <= 0 || gGuarantee_list[i - 1].message != gGuarantee_list[i].message)
            && (gNext_guarantee <= j + 1 || gGuarantee_list[j + 1].message != gGuarantee_list[i].message)) {
            gGuarantee_list[i].message->guarantee_number = 0;
            NetDisposeMessage(gCurrent_net_game, gGuarantee_list[i].message);
        }
    }
    gNext_guarantee = i;
}

// IDA: int __usercall SampleFailNotifier@<EAX>(tU32 pAge@<EAX>, tNet_message *pMessage@<EDX>)
// FUNCTION: CARM95 0x0044ad06
int SampleFailNotifier(tU32 pAge, tNet_message* pMessage) {

    return pAge > 9999;
}

// IDA: void __cdecl NetWaitForGuaranteeReplies()
// FUNCTION: CARM95 0x0044ad2f
void NetWaitForGuaranteeReplies(void) {
    tU32 start_time;

    start_time = PDGetTotalTime();
    while (gNext_guarantee != 0) {
        if (PDGetTotalTime() - start_time >= 5000) {
            break;
        }
        NetService(0);
    }
}

// IDA: tNet_game_player_info* __usercall NetPlayerFromID@<EAX>(tPlayer_ID pPlayer@<EAX>)
// FUNCTION: CARM95 0x0044ad74
tNet_game_player_info* NetPlayerFromID(tPlayer_ID pPlayer) {
    int i;

    for (i = 0; i < gNumber_of_net_players; i++) {
        if (gNet_players[i].ID == pPlayer) {
            return &gNet_players[i];
        }
    }
    return 0;
}

// IDA: tCar_spec* __usercall NetCarFromPlayerID@<EAX>(tPlayer_ID pPlayer@<EAX>)
// FUNCTION: CARM95 0x0044add4
tCar_spec* NetCarFromPlayerID(tPlayer_ID pPlayer) {
    int i;
    tNet_game_player_info* player;

    player = NetPlayerFromID(pPlayer);
    if (player) {
        return player->car;
    }
    return NULL;
}

// IDA: tNet_game_player_info* __usercall NetPlayerFromCar@<EAX>(tCar_spec *pCar@<EAX>)
// FUNCTION: CARM95 0x0044ae15
tNet_game_player_info* NetPlayerFromCar(tCar_spec* pCar) {
    int i;

    for (i = 0; i < gNumber_of_net_players; i++) {
        if (gNet_players[i].car == pCar) {
            return &gNet_players[i];
        }
    }
    return 0;
}

// IDA: tU32 __usercall DoCheckSum@<EAX>(tNet_message *pMessage@<EAX>)
// FUNCTION: CARM95 0x0044ae74
tU32 DoCheckSum(tNet_message* pMessage) {
    int i;
    int j;
    tU32 the_sum;
    tU32* p;
    tU8* q;

    // empty function
    return 0;
}

// IDA: void __usercall GetCheckSum(tNet_message *pMessage@<EAX>)
void GetCheckSum(tNet_message* pMessage) {
}

// IDA: void __usercall CheckCheckSum(tNet_message *pMessage@<EAX>)
// FUNCTION: CARM95 0x0044ae7f
void CheckCheckSum(tNet_message* pMessage) {
}
