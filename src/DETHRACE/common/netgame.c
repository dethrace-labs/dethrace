#include "netgame.h"
#include "brender/brender.h"
#include "car.h"
#include "displays.h"
#include "errors.h"
#include "globvars.h"
#include "globvrpb.h"
#include "graphics.h"
#include "harness/trace.h"
#include "loading.h"
#include "network.h"
#include "newgame.h"
#include "opponent.h"
#include "pd/sys.h"
#include "pedestrn.h"
#include "powerup.h"
#include "racestrt.h"
#include "structur.h"
#include "utility.h"
#include <stdlib.h>
#include <string.h>

int gPowerup_cost[4] = { 1500, 2500, 4000, 6000 };
int gGame_scores[6];
int gPed_target;
int gNot_shown_race_type_headup;
tU32 gLast_it_change;
tU32 gTime_for_punishment;
tNet_game_player_info* gLast_lepper;
int gInitialised_grid;
int gIt_or_fox;

// IDA: void __usercall SendCarData(tU32 pNext_frame_time@<EAX>)
void SendCarData(tU32 pNext_frame_time) {
    tNet_contents* contents;
    tCar_spec* car;
    tCollision_info* ncar;
    int i;
    int j;
    static tU32 last_time;
    tU32 time;
    int damaged_wheels;
    LOG_TRACE("(%d)", pNext_frame_time);

    if (gNet_mode) {
        TELL_ME_IF_WE_PASS_THIS_WAY();
    }
}

// IDA: void __usercall ReceivedRecover(tNet_contents *pContents@<EAX>)
void ReceivedRecover(tNet_contents* pContents) {
    int i;
    LOG_TRACE("(%p)", pContents);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall CopyMechanics(tCar_spec *pCar@<EAX>, tNet_contents *pContents@<EDX>)
void CopyMechanics(tCar_spec* pCar, tNet_contents* pContents) {
    int j;
    LOG_TRACE("(%p, %p)", pCar, pContents);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ReceivedMechanics(tNet_contents *pContents@<EAX>)
void ReceivedMechanics(tNet_contents* pContents) {
    int i;
    tCar_spec* car;
    LOG_TRACE("(%p)", pContents);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ReceivedCopInfo(tNet_contents *pContents@<EAX>)
void ReceivedCopInfo(tNet_contents* pContents) {
    tCar_spec* c;
    int i;
    LOG_TRACE("(%p)", pContents);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl SendAllNonCarPositions()
void SendAllNonCarPositions(void) {
    int i;
    br_actor** list;
    tNon_car_spec* non_car;
    tNet_contents* contents;
    LOG_TRACE("()");

    list = gProgram_state.track_spec.non_car_list;
    for (i = 0; gProgram_state.track_spec.ampersand_digits > i; ++i) {
        if (list[i]->type_data) {
            non_car = (tNon_car_spec*)list[i]->type_data;
            if (non_car->collision_info.driver == eDriver_non_car_unused_slot || non_car->collision_info.car_ID != i) {
                contents = NetGetBroadcastContents(NETMSGID_NONCARPOSITION, 0);
                BrMatrix34Copy(&contents->data.non_car_position.mat, &list[i]->t.t.mat);
                contents->data.non_car_position.ID = i;
                contents->data.non_car_position.flags = list[i]->identifier[3] == '!';
            }
        }
    }
    NetSendMessageStacks();
}

// IDA: void __usercall ReceivedNonCarPosition(tNet_contents *pContents@<EAX>)
void ReceivedNonCarPosition(tNet_contents* pContents) {
    br_actor* actor;
    LOG_TRACE("(%p)", pContents);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ReceivedNonCar(tNet_contents *pContents@<EAX>)
void ReceivedNonCar(tNet_contents* pContents) {
    br_actor* actor;
    br_vector3 tv;
    tU8 cx;
    tU8 cz;
    tTrack_spec* track_spec;
    tNon_car_spec* ncar;
    tCollision_info* c;
    LOG_TRACE("(%p)", pContents);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SignalToStartRace2(int pIndex@<EAX>)
void SignalToStartRace2(int pIndex) {
    tNet_message* the_message;
    int i;
    int j;
    LOG_TRACE("(%d)", pIndex);

    if (gCurrent_race.number_of_racers > 6) {
        FadePaletteUp();
        dr_dprintf("AAAARRRGGGHHH!!!! More than 6 racers!!!!");
        PDFatalError("AAAARRRGGGHHH!!!! More than 6 racers!!!!");
    }
    gNeed_to_send_start_race = 0;
    gStart_race_sent = 1;
    the_message = NetBuildMessage(NETMSGID_STARTRACE, 0);
    the_message->contents.data.start_race.racing = gProgram_state.racing;
    if (pIndex >= 0) {
        gNet_players[pIndex].last_waste_message = 0;
        gNet_players[pIndex].wasteage_attributed = 0;
        the_message->contents.data.start_race.car_count = -1;
        the_message->contents.data.start_race.car_list[0].index = pIndex;
        BrMatrix34Copy(&the_message->contents.data.start_race.car_list[0].mat,
            &gCurrent_race.opponent_list[gNet_players[pIndex].opponent_list_index].car_spec->car_master_actor->t.t.mat);
    } else {
        the_message->contents.data.start_race.car_count = gCurrent_race.number_of_racers;
        for (i = 0; i < gCurrent_race.number_of_racers; i++) {
            BrMatrix34Copy(&the_message->contents.data.start_race.car_list[i].mat,
                &gCurrent_race.opponent_list[i].car_spec->car_master_actor->t.t.mat);
            for (j = 0; j < gNumber_of_net_players; j++) {
                if (gCurrent_race.opponent_list[i].car_spec == gNet_players[j].car) {
                    the_message->contents.data.start_race.car_list[i].index = j;
                    break;
                }
            }
            if (gCurrent_net_game->options.random_car_choice && (gCurrent_net_game->options.car_choice == eNet_car_all || gCurrent_net_game->options.car_choice == eNet_car_both)) {
                if (gNet_players[the_message->contents.data.start_race.car_list[i].index].next_car_index < 0) {
                    gNet_players[the_message->contents.data.start_race.car_list[i].index].next_car_index = PickARandomCar();
                    gCar_details[gNet_players[the_message->contents.data.start_race.car_list[i].index].next_car_index].ownership = eCar_owner_someone;
                }
                the_message->contents.data.start_race.car_list[i].next_car_index = gNet_players[the_message->contents.data.start_race.car_list[i].index].next_car_index;
            }
        }
        if (gPending_race < 0) {
            gPending_race = PickNetRace(gProgram_state.current_race_index,
                gCurrent_net_game->options.race_sequence_type);
        }
        the_message->contents.data.start_race.next_race = gPending_race;
    }
    NetGuaranteedSendMessageToAllPlayers(gCurrent_net_game, the_message, NULL);
    if (gProgram_state.racing) {
        SendCurrentPowerups();
    }
}

// IDA: void __cdecl SignalToStartRace()
void SignalToStartRace(void) {
    LOG_TRACE("()");

    gCurrent_net_game->no_races_yet = 0;
    SignalToStartRace2(-1);
}

// IDA: void __cdecl SetUpNetCarPositions()
void SetUpNetCarPositions(void) {
    int i;
    int j;
    int k;
    int grid_index;
    int racer_count;
    LOG_TRACE("()");

    DisableNetService();
    if (!gInitialised_grid) {
        for (i = 0; i < gNumber_of_net_players; i++) {
            gNet_players[i].grid_position_set = 0;
            gNet_players[i].last_waste_message = 0;
            gNet_players[i].wasteage_attributed = 0;
        }
    }
    for (i = 0; i < gNumber_of_net_players; i++) {
        gCurrent_race.opponent_list[i].index = -1;
        gCurrent_race.opponent_list[i].ranking = IRandomBetween(0, 99);
        gCurrent_race.opponent_list[i].car_spec = gNet_players[i].car;
        gCurrent_race.opponent_list[i].net_player_index = i;
        gNet_players[i].opponent_list_index = i;
    }
    if (!gInitialised_grid && gCurrent_net_game->options.grid_start) {
        qsort(gCurrent_race.opponent_list, gNumber_of_net_players, sizeof(tOpp_spec), SortGridFunction);
    }
    gCurrent_race.number_of_racers = 0;
    for (i = 0; i < gNumber_of_net_players; i++) {
        gNet_players[gCurrent_race.opponent_list[i].net_player_index].opponent_list_index = i;
    }
    for (i = 0; i < gNumber_of_net_players; i++) {
        if ((gCurrent_race.opponent_list[i].car_spec->driver == eDriver_oppo && !gInitialised_grid)
            || (gCurrent_race.opponent_list[i].car_spec->driver >= eDriver_net_human && !gNet_players[gCurrent_race.opponent_list[i].net_player_index].grid_position_set)) {
            grid_index = -1;
            racer_count = 0;
            while (racer_count < 6 && grid_index < 0) {
                grid_index = racer_count;
                for (k = 0; k < gNumber_of_net_players; k++) {
                    if (k != i
                        && gNet_players[gCurrent_race.opponent_list[k].net_player_index].grid_position_set
                        && gNet_players[gCurrent_race.opponent_list[k].net_player_index].grid_index == racer_count) {
                        grid_index = -1;
                        break;
                    }
                }
                racer_count++;
            }
            if (grid_index < 0) {
                FatalError(kFatalError_NetworkCodeSelfCheck);
            }
            SetInitialPosition(&gCurrent_race, i, grid_index);
            gNet_players[gCurrent_race.opponent_list[i].net_player_index].grid_index = grid_index;
            if (gInitialised_grid) {
                InitialiseCar2(gCurrent_race.opponent_list[i].car_spec, 0);
            } else {
                gCurrent_race.number_of_racers = i + 1;
            }
            gNet_players[gCurrent_race.opponent_list[i].net_player_index].grid_position_set = 1;
        }
    }
    gCurrent_race.number_of_racers = gNumber_of_net_players;
    gInitialised_grid = 1;
    ReenableNetService();
}

// IDA: void __usercall ReinitialiseCar(tCar_spec *pCar@<EAX>)
void ReinitialiseCar(tCar_spec* pCar) {
    int i;
    LOG_TRACE("(%p)", pCar);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall RepositionPlayer(int pIndex@<EAX>)
void RepositionPlayer(int pIndex) {
    tNet_message* the_message;
    tCar_spec* car;
    LOG_TRACE("(%d)", pIndex);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DisableCar(tCar_spec *pCar@<EAX>)
void DisableCar(tCar_spec* pCar) {
    LOG_TRACE("(%p)", pCar);

    if (pCar->driver_name[0] != '\0') {
        if (!pCar->disabled) {
            pCar->disabled = 1;
            ForceRebuildActiveCarList();
        }
        if (pCar->car_master_actor->t.t.mat.m[3][0] < 500.0f) {
            pCar->car_master_actor->t.t.mat.m[3][0] = pCar->car_master_actor->t.t.mat.m[3][0] + 1000.0f;
            pCar->car_master_actor->t.t.mat.m[3][1] = pCar->car_master_actor->t.t.mat.m[3][1] + 1000.0f;
            pCar->car_master_actor->t.t.mat.m[3][2] = pCar->car_master_actor->t.t.mat.m[3][2] + 1000.0f;
            pCar->old_frame_mat.m[3][0] = pCar->car_master_actor->t.t.mat.m[3][0];
            pCar->old_frame_mat.m[3][1] = pCar->car_master_actor->t.t.mat.m[3][1];
            pCar->old_frame_mat.m[3][2] = pCar->car_master_actor->t.t.mat.m[3][2];
        }
    }
}

// IDA: void __usercall EnableCar(tCar_spec *pCar@<EAX>)
void EnableCar(tCar_spec* pCar) {
    LOG_TRACE("(%p)", pCar);

    if (pCar->driver_name[0] != '\0') {
        if (pCar->disabled) {
            pCar->disabled = 0;
            ForceRebuildActiveCarList();
        }
        if (pCar->car_master_actor->t.t.mat.m[3][0] > 500.0f) {
            pCar->car_master_actor->t.t.mat.m[3][0] = pCar->car_master_actor->t.t.mat.m[3][0] - 1000.0f;
            pCar->car_master_actor->t.t.mat.m[3][1] = pCar->car_master_actor->t.t.mat.m[3][1] - 1000.0f;
            pCar->car_master_actor->t.t.mat.m[3][2] = pCar->car_master_actor->t.t.mat.m[3][2] - 1000.0f;
            pCar->old_frame_mat.m[3][0] = pCar->car_master_actor->t.t.mat.m[3][0];
            pCar->old_frame_mat.m[3][1] = pCar->car_master_actor->t.t.mat.m[3][1];
            pCar->old_frame_mat.m[3][2] = pCar->car_master_actor->t.t.mat.m[3][2];
        }
    }
}

// IDA: void __usercall DoNetworkHeadups(int pCredits@<EAX>)
void DoNetworkHeadups(int pCredits) {
    char s[256];
    char s2[256];
    static tU32 last_flash;
    static int flash_state;
    LOG_TRACE("(%d)", pCredits);

    if (gNot_shown_race_type_headup) {
        gNot_shown_race_type_headup = 0;
        NewTextHeadupSlot(4, 0, 2000, -4, GetMiscString(59 + gCurrent_net_game->type));
    }
    if (gTime_for_punishment && gTime_for_punishment <= PDGetTotalTime()) {
        gTime_for_punishment = 0;
        switch (gCurrent_net_game->type) {
        case eNet_game_type_carnage:
            NewTextHeadupSlot(4, 0, 2000, -4, GetMiscString(219));
            break;
        case eNet_game_type_checkpoint:
            NewTextHeadupSlot(4, 0, 2000, -4, GetMiscString(220));
            break;
        case eNet_game_type_sudden_death:
            NewTextHeadupSlot(4, 0, 2000, -4, GetMiscString(221));
            break;
        case eNet_game_type_foxy:
            NewTextHeadupSlot(4, 0, 2000, -4, GetMiscString(222));
            break;
        default:
            break;
        }
    }
    if (gNet_mode == eNet_mode_none || gNet_recovery_cost[gCurrent_net_game->type] <= gProgram_state.credits_earned - gProgram_state.credits_lost || Flash(200, &last_flash, &flash_state)) {
        sprintf(s, "\xf8%d\xfa %s", pCredits, GetMiscString(94));
        ChangeHeadupText(gNet_cash_headup, s);
    } else {
        ChangeHeadupText(gNet_cash_headup, "");
    }
    switch (gCurrent_net_game->type) {
    case eNet_game_type_carnage:
        sprintf(s, "%s \xf8%d\xfa", GetMiscString(180), gPed_target);
        break;
    case eNet_game_type_car_crusher:
        sprintf(s, "%s \xf8%d\xfa", GetMiscString(181), gCurrent_net_game->options.race_end_target);
        break;
    case eNet_game_type_foxy:
        TimerString(gCurrent_net_game->options.race_end_target, s2, 1, 1);
        sprintf(s, "%s \xf8%s\xfa", GetMiscString(182), s2);
        break;
    case eNet_game_type_tag:
        TimerString(gCurrent_net_game->options.race_end_target, s2, 1, 1);
        sprintf(s, "%s \xf8%s\xfa", GetMiscString(183), s2);
        break;
    default:
        s[0] = '\0';
        break;
    }
    ChangeHeadupText(gNet_ped_headup, s);
}

// IDA: int __usercall SortNetHeadAscending@<EAX>(void *pFirst_one@<EAX>, void *pSecond_one@<EDX>)
int SortNetHeadAscending(void* pFirst_one, void* pSecond_one) {
    LOG_TRACE("(%p, %p)", pFirst_one, pSecond_one);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall SortNetHeadDescending@<EAX>(void *pFirst_one@<EAX>, void *pSecond_one@<EDX>)
int SortNetHeadDescending(void* pFirst_one, void* pSecond_one) {
    LOG_TRACE("(%p, %p)", pFirst_one, pSecond_one);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall ClipName(char *pName@<EAX>, tDR_font *pFont@<EDX>, int pMax_width@<EBX>)
void ClipName(char* pName, tDR_font* pFont, int pMax_width) {
    LOG_TRACE("(\"%s\", %p, %d)", pName, pFont, pMax_width);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DoNetScores2(int pOnly_sort_scores@<EAX>)
void DoNetScores2(int pOnly_sort_scores) {
    int i;
    int j;
    int score;
    int flags;
    int index;
    int right_edge;
    int x;
    int len;
    int ascending_order;
    char s[256];
    static tU32 last_flash;
    static int flash_state;
    tHeadup_pair headup_pairs[6];
    LOG_TRACE("(%d)", pOnly_sort_scores);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl DoNetScores()
void DoNetScores(void) {
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl InitNetHeadups()
void InitNetHeadups(void) {
    LOG_TRACE("()");

    gIcons_pix = LoadPixelmap("CARICONS.PIX");
    if (gIcons_pix != NULL) {
        BrMapAdd(gIcons_pix);
    }
    gDigits_pix = LoadPixelmap("HDIGITS.PIX");
    if (gDigits_pix != NULL) {
        BrMapAdd(gDigits_pix);
    }
}

// IDA: void __cdecl DisposeNetHeadups()
void DisposeNetHeadups(void) {
    LOG_TRACE("()");

    if (gIcons_pix != NULL) {
        BrMapRemove(gIcons_pix);
        BrPixelmapFree(gIcons_pix);
    }
    if (gDigits_pix != NULL) {
        BrMapRemove(gDigits_pix);
        BrPixelmapFree(gDigits_pix);
    }
}

// IDA: void __cdecl EverybodysLost()
void EverybodysLost(void) {
    tNet_message* the_message;
    int i;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __usercall DeclareWinner(int pWinner_index@<EAX>)
void DeclareWinner(int pWinner_index) {
    tNet_message* the_message;
    int i;
    int j;
    int best_score_index;
    char s[256];
    LOG_TRACE("(%d)", pWinner_index);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall PlayerIsIt(tNet_game_player_info *pPlayer@<EAX>)
void PlayerIsIt(tNet_game_player_info* pPlayer) {
    int i;
    char s[256];
    LOG_TRACE("(%p)", pPlayer);
    NOT_IMPLEMENTED();
}

// IDA: int __usercall FarEnoughAway@<EAX>(tNet_game_player_info *pPlayer_1@<EAX>, tNet_game_player_info *pPlayer_2@<EDX>)
int FarEnoughAway(tNet_game_player_info* pPlayer_1, tNet_game_player_info* pPlayer_2) {
    br_vector3 difference;
    LOG_TRACE("(%p, %p)", pPlayer_1, pPlayer_2);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall CarInContactWithItOrFox(tNet_game_player_info *pPlayer@<EAX>)
void CarInContactWithItOrFox(tNet_game_player_info* pPlayer) {
    LOG_TRACE("(%p)", pPlayer);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SelectRandomItOrFox(int pNot_this_one@<EAX>)
void SelectRandomItOrFox(int pNot_this_one) {
    int i;
    int new_choice;
    LOG_TRACE("(%d)", pNot_this_one);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl CalcPlayerScores()
void CalcPlayerScores(void) {
    int i;
    int j;
    int knock_out_bit;
    int e_dam;
    int t_dam;
    int d_dam;
    int w_dam;
    int cars_left;
    int car_left;
    int flags;
    int score;
    int highest;
    int next_highest;
    int lowest_score;
    int player_left;
    int new_choice;
    tCar_spec* car;
    tNet_message* message;
    tS32 time;
    char s[256];
    tNet_game_player_info* lowest_score_player;
    LOG_TRACE("()");
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl SendPlayerScores()
void SendPlayerScores(void) {
    tNet_contents* the_contents;
    int i;
    LOG_TRACE("()");

    the_contents = NetGetBroadcastContents(NETMSGID_SCORES, 0);
    if (gCurrent_net_game->type == eNet_game_type_carnage) {
        the_contents->data.scores.general_score = gPed_target;
    } else if (gCurrent_net_game->type == eNet_game_type_tag || gCurrent_net_game->type == eNet_game_type_foxy) {
        the_contents->data.scores.general_score = gNet_players[gIt_or_fox].ID;
    }
    for (i = 0; i < gNumber_of_net_players; i++) {
        the_contents->data.scores.scores[i] = gNet_players[i].score;
    }
}

// IDA: void __cdecl DoNetGameManagement()
void DoNetGameManagement(void) {
    LOG_TRACE("()");

    if (gNet_mode == eNet_mode_host) {
        CalcPlayerScores();
        SendPlayerScores();
    }
}

// IDA: void __usercall InitialisePlayerScore(tNet_game_player_info *pPlayer@<EAX>)
void InitialisePlayerScore(tNet_game_player_info* pPlayer) {
    LOG_TRACE("(%p)", pPlayer);

    PossibleService();
    switch (gCurrent_net_game->type) {
    case eNet_game_type_fight_to_death:
        pPlayer->score = 100;
        break;
    case eNet_game_type_car_crusher:
    case eNet_game_type_carnage:
        pPlayer->score = 0;
        break;
    case eNet_game_type_checkpoint:
        pPlayer->score = 0xffff;
        break;
    case eNet_game_type_sudden_death:
        pPlayer->score = 0;
        break;
    case eNet_game_type_tag:
        pPlayer->score = 0;
        break;
    case eNet_game_type_foxy:
        pPlayer->score = 0;
        break;
    default:
        TELL_ME_IF_WE_PASS_THIS_WAY();
    }
    pPlayer->credits = gInitial_net_credits[gCurrent_net_game->options.starting_money_index];
    pPlayer->wasted = 0;
    pPlayer->reposition_time = 0;
}

// IDA: void __cdecl InitPlayers()
void InitPlayers(void) {
    int i;
    LOG_TRACE("()");

    for (i = 0; i < gNumber_of_net_players; i++) {
        InitialisePlayerScore(&gNet_players[i]);
    }
    if (gNet_mode == eNet_mode_host) {
        gLast_it_change = 0;
        gLast_lepper = NULL;
    }
    gTime_for_punishment = 0;
    gNot_shown_race_type_headup = 1;
    gIt_or_fox = -1;
}

// IDA: void __usercall BuyPSPowerup(int pIndex@<EAX>)
void BuyPSPowerup(int pIndex) {
    char s[256];
    char s2[256];
    LOG_TRACE("(%d)", pIndex);

    if (gNet_mode == eNet_mode_none) {
        NewTextHeadupSlot(4, 0, 3000, -4, GetMiscString(178));
    } else if (gProgram_state.current_car.power_up_levels[pIndex] < 4) {
        if (gNet_mode == eNet_mode_none || gPowerup_cost[gProgram_state.current_car.power_up_levels[pIndex]] <= (gProgram_state.credits_earned - gProgram_state.credits_lost)) {
            SpendCredits(gPowerup_cost[gProgram_state.current_car.power_up_levels[pIndex]]);
            ImprovePSPowerup(&gProgram_state.current_car, pIndex);
        } else {
            strcpy(s, GetMiscString(179));
            sprintf(s2, "%d", gPowerup_cost[gProgram_state.current_car.power_up_levels[pIndex]]);
            SubsStringJob(s, s2);
            NewTextHeadupSlot(4, 0, 3008, -4, s);
        }
    } else {
        NewTextHeadupSlot(4, 0, 3000, -4, GetMiscString(174));
    }
}

// IDA: void __cdecl BuyArmour()
void BuyArmour(void) {
    LOG_TRACE("()");

    BuyPSPowerup(0);
}

// IDA: void __cdecl BuyPower()
void BuyPower(void) {
    LOG_TRACE("()");

    if (gNet_mode != eNet_mode_none && gCurrent_net_game->type == eNet_game_type_foxy && gThis_net_player_index == gIt_or_fox) {
        NewTextHeadupSlot(4, 0, 1000, -4, GetMiscString(214));
    } else if (gNet_mode != eNet_mode_none && gCurrent_net_game->type == eNet_game_type_tag && gThis_net_player_index != gIt_or_fox) {
        NewTextHeadupSlot(4, 0, 1000, -4, GetMiscString(215));
    } else {
        BuyPSPowerup(1);
    }
}

// IDA: void __cdecl BuyOffense()
void BuyOffense(void) {
    LOG_TRACE("()");

    BuyPSPowerup(2);
}

// IDA: void __usercall UseGeneralScore(int pScore@<EAX>)
void UseGeneralScore(int pScore) {
    int i;
    LOG_TRACE("(%d)", pScore);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall NetSendEnvironmentChanges(tNet_game_player_info *pPlayer@<EAX>)
void NetSendEnvironmentChanges(tNet_game_player_info* pPlayer) {
    LOG_TRACE("(%p)", pPlayer);

    SendAllPedestrianPositions(pPlayer->ID);
    SendAllNonCarPositions();
}

// IDA: void __cdecl UpdateEnvironments()
void UpdateEnvironments(void) {
    int i;
    LOG_TRACE("()");

    for (i = 1; i < gNumber_of_net_players; i++) {
        if (gNet_players[i].race_stuff_initialised == 0) {
            NetSendEnvironmentChanges(&gNet_players[i]);
            gNet_players[i].race_stuff_initialised = 1;
        }
        NetSendMessageStacks();
        SendGameplay(gNet_players[i].ID, eNet_gameplay_go_for_it, 0, 0, 0, 0);
    }
}

// IDA: void __usercall ReceivedGameplay(tNet_contents *pContents@<EAX>, tNet_message *pMessage@<EDX>, tU32 pReceive_time@<EBX>)
void ReceivedGameplay(tNet_contents* pContents, tNet_message* pMessage, tU32 pReceive_time) {
    int must_revert_reentrancy;
    int gPixel_buffer_size;
    char* gPixels_copy;
    char* gPalette_copy;
    static int pause_semaphore;
    LOG_TRACE("(%p, %p, %d)", pContents, pMessage, pReceive_time);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall SendGameplay(tPlayer_ID pPlayer@<EAX>, tNet_gameplay_mess pMess@<EDX>, int pParam_1@<EBX>, int pParam_2@<ECX>, int pParam_3, int pParam_4)
void SendGameplay(tPlayer_ID pPlayer, tNet_gameplay_mess pMess, int pParam_1, int pParam_2, int pParam_3, int pParam_4) {
    tNet_message* the_message;
    LOG_TRACE("(%d, %d, %d, %d, %d, %d)", pPlayer, pMess, pParam_1, pParam_2, pParam_3, pParam_4);

    the_message = NetBuildMessage(NETMSGID_GAMEPLAY, 0);
    the_message->contents.data.gameplay.mess = pMess;
    the_message->contents.data.gameplay.param_1 = pParam_1;
    the_message->contents.data.gameplay.param_2 = pParam_2;
    the_message->contents.data.gameplay.param_3 = pParam_3;
    the_message->contents.data.gameplay.param_4 = pParam_4;
    NetGuaranteedSendMessageToPlayer(gCurrent_net_game, the_message, pPlayer, 0);
}

// IDA: void __usercall SendGameplayToAllPlayers(tNet_gameplay_mess pMess@<EAX>, int pParam_1@<EDX>, int pParam_2@<EBX>, int pParam_3@<ECX>, int pParam_4)
void SendGameplayToAllPlayers(tNet_gameplay_mess pMess, int pParam_1, int pParam_2, int pParam_3, int pParam_4) {
    tNet_message* the_message;
    LOG_TRACE("(%d, %d, %d, %d, %d)", pMess, pParam_1, pParam_2, pParam_3, pParam_4);

    the_message = NetBuildMessage(NETMSGID_GAMEPLAY, 0);
    the_message->contents.data.gameplay.mess = pMess;
    the_message->contents.data.gameplay.param_1 = pParam_1;
    the_message->contents.data.gameplay.param_2 = pParam_2;
    the_message->contents.data.gameplay.param_3 = pParam_3;
    the_message->contents.data.gameplay.param_4 = pParam_4;
    NetGuaranteedSendMessageToAllPlayers(gCurrent_net_game, the_message, NULL);
}

// IDA: void __usercall SendGameplayToHost(tNet_gameplay_mess pMess@<EAX>, int pParam_1@<EDX>, int pParam_2@<EBX>, int pParam_3@<ECX>, int pParam_4)
void SendGameplayToHost(tNet_gameplay_mess pMess, int pParam_1, int pParam_2, int pParam_3, int pParam_4) {
    tNet_message* the_message;
    LOG_TRACE("(%d, %d, %d, %d, %d)", pMess, pParam_1, pParam_2, pParam_3, pParam_4);
    NOT_IMPLEMENTED();
}

// IDA: void __cdecl InitNetGameplayStuff()
void InitNetGameplayStuff(void) {
    LOG_TRACE("()");
}

// IDA: void __cdecl DefaultNetName()
void DefaultNetName(void) {
    NetObtainSystemUserName(gNet_player_name, 32);
}

// IDA: void __usercall NetSendPointCrush(tCar_spec *pCar@<EAX>, tU16 pCrush_point_index@<EDX>, br_vector3 *pEnergy_vector@<EBX>)
void NetSendPointCrush(tCar_spec* pCar, tU16 pCrush_point_index, br_vector3* pEnergy_vector) {
    tNet_contents* contents;
    LOG_TRACE("(%p, %d, %p)", pCar, pCrush_point_index, pEnergy_vector);

    STUB_ONCE();
}

// IDA: void __usercall RecievedCrushPoint(tNet_contents *pContents@<EAX>)
void RecievedCrushPoint(tNet_contents* pContents) {
    tCar_spec* car;
    LOG_TRACE("(%p)", pContents);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall GetReducedMatrix(tReduced_matrix *m1@<EAX>, br_matrix34 *m2@<EDX>)
void GetReducedMatrix(tReduced_matrix* m1, br_matrix34* m2) {
    LOG_TRACE("(%p, %p)", m1, m2);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall GetExpandedMatrix(br_matrix34 *m1@<EAX>, tReduced_matrix *m2@<EDX>)
void GetExpandedMatrix(br_matrix34* m1, tReduced_matrix* m2) {
    LOG_TRACE("(%p, %p)", m1, m2);
    NOT_IMPLEMENTED();
}

// IDA: void __usercall NetEarnCredits(tNet_game_player_info *pPlayer@<EAX>, tS32 pCredits@<EDX>)
void NetEarnCredits(tNet_game_player_info* pPlayer, tS32 pCredits) {
    LOG_TRACE("(%p, %d)", pPlayer, pCredits);
    NOT_IMPLEMENTED();
}
