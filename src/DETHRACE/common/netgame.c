#include "netgame.h"
#include "brender.h"
#include "brucetrk.h"
#include "car.h"
#include "controls.h"
#include "crush.h"
#include "displays.h"
#include "errors.h"
#include "globvars.h"
#include "globvrbm.h"
#include "globvrpb.h"
#include "grafdata.h"
#include "graphics.h"
#include "harness/trace.h"
#include "loading.h"
#include "network.h"
#include "newgame.h"
#include "opponent.h"
#include "pd/sys.h"
#include "pedestrn.h"
#include "powerup.h"
#include "pratcam.h"
#include "racestrt.h"
#include "spark.h"
#include "structur.h"
#include "utility.h"
#include <limits.h>
#include <stdlib.h>
#include <string.h>

// GLOBAL: CARM95 0x0050c5e0
int gPowerup_cost[4] = { 1500, 2500, 4000, 6000 };

// GLOBAL: CARM95 0x0050c5f0
int gGame_scores[6] = { 1, 2, 3, 4, 6, 10 };

// GLOBAL: CARM95 0x0053220c
int gPed_target;

// GLOBAL: CARM95 0x00532210
int gNot_shown_race_type_headup;

// GLOBAL: CARM95 0x00532208
tU32 gLast_it_change;

// GLOBAL: CARM95 0x00532204
tU32 gTime_for_punishment;

// GLOBAL: CARM95 0x00532200
tNet_game_player_info* gLast_lepper;

// GLOBAL: CARM95 0x00551d7c
int gInitialised_grid;

// GLOBAL: CARM95 0x00551d80
int gIt_or_fox;

#define PACK_POWERUPS(car) (car->power_up_levels[0] & 0xff) + ((car->power_up_levels[2] & 0xff) << 6) + ((car->power_up_levels[1] & 0xff) << 3);

// IDA: void __usercall SendCarData(tU32 pNext_frame_time@<EAX>)
// FUNCTION: CARM95 0x0042f2d0
void SendCarData(tU32 pNext_frame_time) {
    tNet_contents* contents;
    tCar_spec* car;
    tCollision_info* ncar;
    int i;
    int j;
    // GLOBAL: CARM95 0x50c608
    static tU32 last_time;
    tU32 time;
    int damaged_wheels;

    time = GetRaceTime();
    if (gNet_mode == eNet_mode_none || (time > last_time && last_time + 80 > time)) {
        return;
    }
    last_time = time;
    contents = NetGetBroadcastContents(NETMSGID_TIMESYNC, 0);
    contents->data.time_sync.race_start_time = gRace_start;

    if (gNet_mode == eNet_mode_host) {
        for (i = 0; i < gNumber_of_net_players; i++) {
            car = gNet_players[i].car;
            if (car->disabled) {
                continue;
            }
            damaged_wheels = car->damage_units[eDamage_lf_wheel].damage_level > 30 || car->damage_units[eDamage_rf_wheel].damage_level > 30 || car->damage_units[eDamage_lr_wheel].damage_level > 30 || car->damage_units[eDamage_rr_wheel].damage_level > 30;
            contents = NetGetBroadcastContents(NETMSGID_MECHANICS, damaged_wheels);
            GetReducedMatrix(&contents->data.mech.mat, &car->car_master_actor->t.t.mat);
            contents->data.mech.ID = gNet_players[i].ID;
            contents->data.mech.time = pNext_frame_time;
            BrVector3Copy(&contents->data.mech.omega, &car->omega);
            BrVector3Copy(&contents->data.mech.v, &car->v);
            contents->data.mech.curvature = (car->curvature / car->maxcurve * 32767.0f);
            contents->data.mech.keys = car->keys;
            contents->data.mech.keys.joystick_acc = (tU8)(car->joystick.acc >> 9);
            contents->data.mech.keys.joystick_dec = (tU8)(car->joystick.dec >> 9);
            contents->data.mech.revs = car->revs;
            for (j = 0; j < COUNT_OF(contents->data.mech.d); j++) {
                contents->data.mech.d[j] = (int)(car->oldd[j] / car->susp_height[j >> 1] * 255.0f);
            }
            for (j = 0; j < COUNT_OF(contents->data.mech.damage); j++) {
                contents->data.mech.damage[j] = car->damage_units[j].damage_level;
            }
            contents->data.mech.front = car->bounds[1].min.v[2];
            contents->data.mech.back = car->bounds[1].max.v[2];
            contents->data.mech.powerups = PACK_POWERUPS(car);
            contents->data.mech.repair_time = car->repair_time;
            contents->data.mech.cc_coll_time = car->last_car_car_collision;
            if (damaged_wheels) {
                for (j = 0; j < COUNT_OF(contents->data.mech.wheel_dam_offset); j++) {
                    contents->data.mech.wheel_dam_offset[j] = car->wheel_dam_offset[j];
                }
            }
            if (car->time_to_recover != 0) {
                if (car->time_to_recover - 500 < pNext_frame_time) {
                    contents = NetGetBroadcastContents(NETMSGID_RECOVER, 0);
                    contents->data.recover.ID = gNet_players[i].ID;
                    contents->data.recover.time_to_recover = car->time_to_recover;
                }
            }
        }
        for (i = 0; i < gNum_active_non_cars; i++) {
            contents = NetGetBroadcastContents(NETMSGID_NONCAR_INFO, 0);
            ncar = (tCollision_info*)gActive_non_car_list[i];
            GetReducedMatrix(&contents->data.mech.mat, &ncar->car_master_actor->t.t.mat);
            contents->data.non_car.ID = ncar->car_ID;
            contents->data.non_car.time = pNext_frame_time;
            BrVector3Copy(&contents->data.non_car.omega, &ncar->omega);
            BrVector3Copy(&contents->data.non_car.v, &ncar->v);
            contents->data.non_car.flags = ncar->car_master_actor->identifier[3] == 2 * ncar->doing_nothing_flag + '!';
        }
        for (i = 0; i < gProgram_state.AI_vehicles.number_of_cops; i++) {
            if (!gProgram_state.AI_vehicles.cops[i].finished_for_this_race) {
                contents = NetGetBroadcastContents(NETMSGID_COPINFO, 0);
                car = gProgram_state.AI_vehicles.cops[i].car_spec;
                GetReducedMatrix(&contents->data.mech.mat, &car->car_master_actor->t.t.mat);
                contents->data.cop_info.ID = car->car_ID;
                contents->data.cop_info.time = pNext_frame_time;
                BrVector3Copy(&contents->data.cop_info.omega, &car->omega);
                BrVector3Copy(&contents->data.cop_info.v, &car->v);
                for (j = 0; j < COUNT_OF(contents->data.cop_info.damage); ++j) {
                    contents->data.cop_info.damage[j] = car->damage_units[j].damage_level;
                }
                for (j = 0; j < COUNT_OF(contents->data.cop_info.d); j++) {
                    contents->data.cop_info.d[j] = car->oldd[j];
                }
            }
        }
    } else if (gNet_mode == eNet_mode_client) {
        car = &gProgram_state.current_car;
        if (car->disabled) {
            return;
        }
        damaged_wheels = car->damage_units[eDamage_lf_wheel].damage_level > 30 || car->damage_units[eDamage_rf_wheel].damage_level > 30 || car->damage_units[eDamage_lr_wheel].damage_level > 30 || car->damage_units[eDamage_rr_wheel].damage_level > 30;
        contents = NetGetToHostContents(NETMSGID_MECHANICS, damaged_wheels);
        GetReducedMatrix(&contents->data.mech.mat, &gProgram_state.current_car.car_master_actor->t.t.mat);
        contents->data.mech.ID = gNet_players[gThis_net_player_index].ID;
        contents->data.mech.time = pNext_frame_time;
        BrVector3Copy(&contents->data.mech.omega, &car->omega);
        BrVector3Copy(&contents->data.mech.v, &car->v);

        contents->data.mech.curvature = (car->curvature / car->maxcurve * 32767.0f);
        contents->data.mech.keys = car->keys;
        contents->data.mech.keys.joystick_acc = (tU8)(car->joystick.acc >> 9);
        contents->data.mech.keys.joystick_dec = (tU8)(car->joystick.dec >> 9);
        contents->data.mech.revs = car->revs;
        contents->data.mech.cc_coll_time = car->last_car_car_collision;
        for (j = 0; j < COUNT_OF(contents->data.mech.d); j++) {
            contents->data.mech.d[j] = (int)(car->oldd[j] / car->susp_height[j >> 1] * 255.f);
        }
        for (j = 0; j < COUNT_OF(contents->data.mech.damage); j++) {
            contents->data.mech.damage[j] = car->damage_units[j].damage_level;
        }
        contents->data.mech.front = car->bounds[1].min.v[2];
        contents->data.mech.back = car->bounds[1].max.v[2];
        contents->data.mech.powerups = PACK_POWERUPS(car);
        contents->data.mech.repair_time = car->repair_time;
        if (damaged_wheels) {
            for (j = 0; j < COUNT_OF(contents->data.mech.wheel_dam_offset); j++) {
                contents->data.mech.wheel_dam_offset[j] = car->wheel_dam_offset[j];
            }
        }
        if (car->time_to_recover > 0 && car->time_to_recover - 500 < pNext_frame_time) {
            contents = NetGetToHostContents(NETMSGID_RECOVER, 0);
            contents->data.recover.ID = gNet_players[gThis_net_player_index].ID;
            contents->data.recover.time_to_recover = gProgram_state.current_car.time_to_recover;
        }
    }
}

// IDA: void __usercall ReceivedRecover(tNet_contents *pContents@<EAX>)
// FUNCTION: CARM95 0x0042fc35
void ReceivedRecover(tNet_contents* pContents) {
    int i;

    if (gNet_players[gThis_net_player_index].ID != pContents->data.recover.ID) {
        for (i = 0; i < gNumber_of_net_players; i++) {
            if (gNet_players[i].ID == pContents->data.recover.ID) {
                gNet_players[i].car->time_to_recover = pContents->data.recover.time_to_recover;
            }
        }
    }
}

// IDA: void __usercall CopyMechanics(tCar_spec *pCar@<EAX>, tNet_contents *pContents@<EDX>)
// FUNCTION: CARM95 0x0042fcb8
void CopyMechanics(tCar_spec* pCar, tNet_contents* pContents) {
    int j;

    memcpy(&pCar->message, pContents, pContents->header.contents_size);
    // if it is not a full mechanics message...
    if (pContents->header.contents_size != sizeof(tNet_message_mechanics_info)) {
        for (j = 0; j < COUNT_OF(pCar->message.wheel_dam_offset); j++) {
            pCar->message.wheel_dam_offset[j] = 0.0f;
        }
    }
}

// IDA: void __usercall ReceivedMechanics(tNet_contents *pContents@<EAX>)
// FUNCTION: CARM95 0x0042fd29
void ReceivedMechanics(tNet_contents* pContents) {
    int i;
    tCar_spec* car;

    car = NULL;
    for (i = 0; i < gNumber_of_net_players; i++) {
        if (gNet_players[i].ID == pContents->data.mech.ID) {
            car = gNet_players[i].car;
            break;
        }
    }
    if (car == NULL || car->message.time >= pContents->data.mech.time) {
        return;
    }
    if (car->disabled) {
        EnableCar(car);
        GetExpandedMatrix(&car->car_master_actor->t.t.mat, &pContents->data.mech.mat);
        BrMatrix34Copy(&car->oldmat, &car->car_master_actor->t.t.mat);
        BrVector3InvScale(&car->car_master_actor->t.t.translate.t, &car->car_master_actor->t.t.translate.t, WORLD_SCALE);
        // car->car_master_actor->t.t.mat.m[3][0] = car->car_master_actor->t.t.mat.m[3][0] * 0.14492753;
        // car->car_master_actor->t.t.mat.m[3][1] = car->car_master_actor->t.t.mat.m[3][1] * 0.14492753;
        // car->car_master_actor->t.t.mat.m[3][2] = car->car_master_actor->t.t.mat.m[3][2] * 0.14492753;
        car->box_face_ref = gFace_num__car - 2;
        car->message.time = pContents->data.mech.time;
        car->message.type = NETMSGID_NONE;
        BrVector3Copy(&car->v, &pContents->data.mech.v);
        BrVector3Copy(&car->omega, &pContents->data.mech.omega);

        for (i = 0; i < COUNT_OF(car->message.d); i++) {
            car->message.d[i] = pContents->data.mech.d[i];
        }
        for (i = 0; i < COUNT_OF(car->message.damage); i++) {
            car->message.damage[i] = pContents->data.mech.damage[i];
        }
        DisableCar(car);
        return;
    }
    if (gNet_mode == eNet_mode_host) {
        if (gThis_net_player_index == i) {
            return;
        }
        if (car->last_car_car_collision <= pContents->data.mech.cc_coll_time) {
            CopyMechanics(car, pContents);
        }
        car->power_up_levels[0] = pContents->data.mech.powerups & 7;
        car->power_up_levels[1] = (pContents->data.mech.powerups >> 3) & 7;
        car->power_up_levels[2] = (pContents->data.mech.powerups >> 6) & 7;
        car->keys = car->message.keys;
        if (car->message.keys.joystick_acc < 0) {
            car->joystick.acc = -1;
        } else {
            car->joystick.acc = car->message.keys.joystick_acc << 9;
        }
        if (car->message.keys.joystick_dec < 0) {
            car->joystick.dec = -1;
        } else {
            car->joystick.dec = car->message.keys.joystick_dec << 9;
        }
    } else if (gNet_mode == eNet_mode_client) {
        if (gThis_net_player_index == i) {
            if (car->last_car_car_collision < pContents->data.mech.cc_coll_time) {
                CopyMechanics(car, pContents);
            }
        } else {
            CopyMechanics(car, pContents);
            car->power_up_levels[0] = pContents->data.mech.powerups & 7;
            car->power_up_levels[1] = (pContents->data.mech.powerups >> 3) & 7;
            car->power_up_levels[2] = (pContents->data.mech.powerups >> 6) & 7;
            car->keys = car->message.keys;
            if (car->message.keys.joystick_acc < 0) {
                car->joystick.acc = -1;
            } else {
                car->joystick.acc = car->message.keys.joystick_acc << 9;
            }
            if (car->message.keys.joystick_dec < 0) {
                car->joystick.dec = -1;
            } else {
                car->joystick.dec = car->message.keys.joystick_dec << 9;
            }
            if (!car->active) {
                GetExpandedMatrix(&car->car_master_actor->t.t.mat, &pContents->data.mech.mat);
                BrMatrix34Copy(&car->oldmat, &car->car_master_actor->t.t.mat);
                BrMatrix34ApplyP(&car->pos, &car->cmpos, &car->car_master_actor->t.t.mat);
                BrVector3InvScale(&car->car_master_actor->t.t.translate.t, &car->car_master_actor->t.t.translate.t, WORLD_SCALE);
                BrVector3InvScale(&car->pos, &car->pos, WORLD_SCALE);
                car->box_face_ref = gFace_num__car - 2;
            }
        }
    }
}

// IDA: void __usercall ReceivedCopInfo(tNet_contents *pContents@<EAX>)
// FUNCTION: CARM95 0x004302c5
void ReceivedCopInfo(tNet_contents* pContents) {
    tCar_spec* c;
    int i;

    if (gNet_mode != eNet_mode_client) {
        return;
    }
    if (pContents->data.cop_info.ID & 0xffffff00) {
        c = GetCarSpec(pContents->data.cop_info.ID >> 8, pContents->data.cop_info.ID & 0xff);
    } else {
        c = &gProgram_state.current_car;
    }
    if (c == NULL || c->message.time > pContents->data.cop_info.time) {
        return;
    }
    c->message.time = pContents->data.cop_info.time;
    if (c->active) {
        c->message.type = NETMSGID_MECHANICS;
        c->message.mat = pContents->data.cop_info.mat;
        BrVector3Copy(&c->message.v, &pContents->data.cop_info.v);
        BrVector3Copy(&c->message.omega, &pContents->data.cop_info.omega);
        c->message.curvature = (tS16)pContents->data.cop_info.curvature;
        for (i = 0; i < COUNT_OF(c->message.d); i++) {
            c->message.d[i] = pContents->data.cop_info.d[i];
        }
        for (i = 0; i < COUNT_OF(c->message.damage); i++) {
            c->message.damage[i] = pContents->data.cop_info.damage[i];
        }
    } else {
        GetExpandedMatrix(&c->car_master_actor->t.t.mat, &pContents->data.cop_info.mat);
        BrVector3InvScale(&c->car_master_actor->t.t.translate.t, &c->car_master_actor->t.t.translate.t, WORLD_SCALE);
        for (i = 0; i < COUNT_OF(c->damage_units); i++) {
            c->damage_units[i].damage_level = pContents->data.cop_info.damage[i];
        }
    }
}

// IDA: void __cdecl SendAllNonCarPositions()
// FUNCTION: CARM95 0x00433d2b
void SendAllNonCarPositions(void) {
    int i;
    br_actor** list;
    tNon_car_spec* non_car;
    tNet_contents* contents;

    list = gProgram_state.track_spec.non_car_list;
    for (i = 0; i < gProgram_state.track_spec.ampersand_digits; ++i) {
        if (list[i]->type_data != NULL) {
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
// FUNCTION: CARM95 0x00430520
void ReceivedNonCarPosition(tNet_contents* pContents) {
    br_actor* actor;

    actor = gProgram_state.track_spec.non_car_list[pContents->data.non_car_position.ID];
    if (actor != NULL && gNet_mode != eNet_mode_none) {
        BrMatrix34Copy(&actor->t.t.mat, &pContents->data.non_car_position.mat);
        if (pContents->data.non_car_position.flags) {
            actor->identifier[3] = '!';
        }
    }
}

// IDA: void __usercall ReceivedNonCar(tNet_contents *pContents@<EAX>)
// FUNCTION: CARM95 0x0043058d
void ReceivedNonCar(tNet_contents* pContents) {
    br_actor* actor;
    br_vector3 tv;
    tU8 cx;
    tU8 cz;
    tTrack_spec* track_spec;
    tNon_car_spec* ncar;
    tCollision_info* c;

    track_spec = &gProgram_state.track_spec;
    if (pContents->data.non_car.ID >= track_spec->ampersand_digits) {
        return;
    }
    actor = track_spec->non_car_list[pContents->data.non_car.ID];
    if (actor == NULL) {
        return;
    }
    ncar = (tNon_car_spec*)actor->type_data;
    if (ncar != NULL && (ncar->collision_info.driver != eDriver_non_car || ncar->collision_info.car_ID != pContents->data.non_car.ID)) {
        ncar = NULL;
    }
    if ((pContents->data.non_car.flags & 1) != 0) {
        actor->identifier[3] = '!';
    } else {
        actor->identifier[3] = 'x';
    }
    if (ncar == NULL && actor->identifier[1] >= '0' && actor->identifier[1] <= '9') {
        BrVector3Sub(&tv, &gProgram_state.current_car.car_master_actor->t.t.translate.t, &actor->t.t.translate.t);
        if (BrVector3LengthSquared(&tv) < 900.0f) {
            DoPullActorFromWorld(actor);
            ncar = (tNon_car_spec*)actor->type_data;
        }
    }
    if (ncar != NULL) {
        c = &ncar->collision_info;
        if ((pContents->data.non_car.flags & 2) != 0) {
            GetExpandedMatrix(&c->car_master_actor->t.t.mat, &pContents->data.non_car.mat);
            BrVector3Copy(&c->v, &pContents->data.non_car.v);
            BrVector3Copy(&c->omega, &pContents->data.non_car.omega);
            c->doing_nothing_flag = 1;
        } else {
            BrVector3Copy(&c->message.v, &pContents->data.non_car.v);
            BrVector3Copy(&c->message.omega, &pContents->data.non_car.omega);
            memcpy(&c->message.mat, &pContents->data.non_car.mat, sizeof(c->message.mat));
            c->message.time = pContents->data.non_car.time;
            c->message.type = NETMSGID_NONCAR_INFO;
            c->doing_nothing_flag = 0;
        }
    } else {
        GetExpandedMatrix(&actor->t.t.mat, &pContents->data.non_car.mat);
        BrVector3InvScale(&actor->t.t.translate.t, &actor->t.t.translate.t, WORLD_SCALE);
        XZToColumnXZ(&cx, &cz, actor->t.t.translate.t.v[0], actor->t.t.translate.t.v[2], track_spec);
        if (track_spec->columns[cz][cx] != actor->parent) {
            if (track_spec->columns[cz][cx] != NULL) {
                BrActorRemove(actor);
                BrActorAdd(track_spec->columns[cz][cx], actor);
            }
        }
    }
}

// IDA: void __usercall SignalToStartRace2(int pIndex@<EAX>)
// FUNCTION: CARM95 0x004308fe
void SignalToStartRace2(int pIndex) {
    tNet_message* the_message;
    int i;
    int j;

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
// FUNCTION: CARM95 0x00430bac
void SignalToStartRace(void) {

    gCurrent_net_game->no_races_yet = 0;
    SignalToStartRace2(-1);
}

// IDA: void __cdecl SetUpNetCarPositions()
// FUNCTION: CARM95 0x00430bcd
void SetUpNetCarPositions(void) {
    int i;
    int j;
    int k;
    int grid_index;
    int racer_count;

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
// FUNCTION: CARM95 0x00430f14
void ReinitialiseCar(tCar_spec* pCar) {
    int i;

    StopCarSmokingInstantly(pCar);
    LoseAllLocalPowerups(pCar);
    InitialiseCar(pCar);
    TotallyRepairACar(pCar);
    if (pCar->driver == eDriver_local_human) {
        gLast_it_change = PDGetTotalTime() + 2000;
    }
}

// IDA: void __usercall RepositionPlayer(int pIndex@<EAX>)
// FUNCTION: CARM95 0x00430f6e
void RepositionPlayer(int pIndex) {
    tNet_message* the_message;
    tCar_spec* car;

    car = gNet_players[pIndex].car;
    gNet_players[pIndex].wasted = 0;
    gNet_players[pIndex].reposition_time = 0;
    if (gCurrent_net_game->type == eNet_game_type_carnage
        || gCurrent_net_game->type == eNet_game_type_checkpoint
        || gCurrent_net_game->type == eNet_game_type_sudden_death) {
        BrMatrix34Copy(&car->car_master_actor->t.t.mat, &gNet_players[pIndex].initial_position);
    } else {
        SetInitialPosition(&gCurrent_race, gNet_players[pIndex].opponent_list_index, gNet_players[pIndex].grid_index);
    }
    ReinitialiseCar(car);
    SetFlipUpCar(car);
    car->last_car_car_collision = GetRaceTime() + 100;
    SignalToStartRace2(pIndex);
    if (car->driver == eDriver_local_human) {
        CancelPendingCunningStunt();
        gProgram_state.credits_earned = gInitial_net_credits[gCurrent_net_game->options.starting_money_index];
        gProgram_state.credits_lost = 0;
    }
}

// IDA: void __usercall DisableCar(tCar_spec *pCar@<EAX>)
// FUNCTION: CARM95 0x00431094
void DisableCar(tCar_spec* pCar) {

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
// FUNCTION: CARM95 0x00431165
void EnableCar(tCar_spec* pCar) {

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
// FUNCTION: CARM95 0x00431236
void DoNetworkHeadups(int pCredits) {
    char s[256];
    char s2[256];
    // GLOBAL: CARM95 0x50c60c
    static tU32 last_flash;
    // GLOBAL: CARM95 0x50c610
    static int flash_state;

    if (gNot_shown_race_type_headup) {
        gNot_shown_race_type_headup = 0;
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -4, GetMiscString(kMiscString_NetworkGameTypeNames_START + gCurrent_net_game->type));
    }
    if (gTime_for_punishment && gTime_for_punishment <= PDGetTotalTime()) {
        gTime_for_punishment = 0;
        switch (gCurrent_net_game->type) {
        case eNet_game_type_carnage:
            NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -4, GetMiscString(kMiscString_THAT_HALVED_YOUR_KILL_COUNT));
            break;
        case eNet_game_type_checkpoint:
            NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -4, GetMiscString(kMiscString_THAT_LOST_YOU_A_CHECKPOINT));
            break;
        case eNet_game_type_sudden_death:
            NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -4, GetMiscString(kMiscString_BACK_TO_THE_START));
            break;
        case eNet_game_type_foxy:
            NewTextHeadupSlot(eHeadupSlot_misc, 0, 2000, -4, GetMiscString(kMiscString_THAT_HALVED_YOUR_TIME));
            break;
        default:
            break;
        }
    }
    if (gNet_mode == eNet_mode_none || gNet_recovery_cost[gCurrent_net_game->type] <= gProgram_state.credits_earned - gProgram_state.credits_lost || Flash(200, &last_flash, &flash_state)) {
        sprintf(s, "\xf8%d\xfa %s", pCredits, GetMiscString(kMiscString_CREDITS));
        ChangeHeadupText(gNet_cash_headup, s);
    } else {
        ChangeHeadupText(gNet_cash_headup, "");
    }
    switch (gCurrent_net_game->type) {
    case eNet_game_type_carnage:
        sprintf(s, "%s \xf8%d\xfa", GetMiscString(kMiscString_TARGET_180), gPed_target);
        break;
    case eNet_game_type_car_crusher:
        sprintf(s, "%s \xf8%d\xfa", GetMiscString(kMiscString_TARGET_181), gCurrent_net_game->options.race_end_target);
        break;
    case eNet_game_type_foxy:
        TimerString(gCurrent_net_game->options.race_end_target, s2, 1, 1);
        sprintf(s, "%s \xf8%s\xfa", GetMiscString(kMiscString_TARGET_182), s2);
        break;
    case eNet_game_type_tag:
        TimerString(gCurrent_net_game->options.race_end_target, s2, 1, 1);
        sprintf(s, "%s \xf8%s\xfa", GetMiscString(kMiscString_LIMIT), s2);
        break;
    default:
        s[0] = '\0';
        break;
    }
    ChangeHeadupText(gNet_ped_headup, s);
}

#define HEADUP1 ((tHeadup_pair*)pFirst_one)
#define HEADUP2 ((tHeadup_pair*)pSecond_one)

// IDA: int __usercall SortNetHeadAscending@<EAX>(void *pFirst_one@<EAX>, void *pSecond_one@<EDX>)
// FUNCTION: CARM95 0x00431ff8
int SortNetHeadAscending(void* pFirst_one, void* pSecond_one) {

    if (HEADUP1->out_of_game) {
        if (HEADUP2->out_of_game) {
            return HEADUP1->out_of_game - HEADUP2->out_of_game;
        } else {
            return INT_MAX;
        }
    } else if (HEADUP2->out_of_game) {
        return -INT_MAX;
    } else if (HEADUP2->score == HEADUP1->score) {
        return gNet_players[HEADUP1->player_index].last_score_index
            - gNet_players[HEADUP2->player_index].last_score_index;
    } else {
        return HEADUP1->score - HEADUP2->score;
    }
}

// IDA: int __usercall SortNetHeadDescending@<EAX>(void *pFirst_one@<EAX>, void *pSecond_one@<EDX>)
// FUNCTION: CARM95 0x004320a9
int SortNetHeadDescending(void* pFirst_one, void* pSecond_one) {

    if (HEADUP1->out_of_game) {
        if (HEADUP2->out_of_game) {
            return HEADUP1->out_of_game - HEADUP2->out_of_game;
        } else {
            return INT_MAX;
        }
    } else if (HEADUP2->out_of_game) {
        return -INT_MAX;
    } else if (HEADUP2->score == HEADUP1->score) {
        return gNet_players[HEADUP1->player_index].last_score_index
            - gNet_players[HEADUP2->player_index].last_score_index;
    } else {
        return HEADUP2->score - HEADUP1->score;
    }
}

#undef HEADUP2
#undef HEADUP1

// IDA: void __usercall ClipName(char *pName@<EAX>, tDR_font *pFont@<EDX>, int pMax_width@<EBX>)
// FUNCTION: CARM95 0x0043215a
void ClipName(char* pName, tDR_font* pFont, int pMax_width) {

    while (DRTextWidth(pFont, pName) > pMax_width) {
        pName[strlen(pName) - 1] = 0;
    }
}

// IDA: void __usercall DoNetScores2(int pOnly_sort_scores@<EAX>)
// FUNCTION: CARM95 0x004315c0
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
    // GLOBAL: CARM95 0x50c614
    static tU32 last_flash;
    // GLOBAL: CARM95 0x50c618
    static int flash_state;
    tHeadup_pair headup_pairs[6];

    ascending_order = gCurrent_net_game->type == eNet_game_type_checkpoint || gCurrent_net_game->type == eNet_game_type_tag;
    for (i = 0; i < gNumber_of_net_players; i++) {
        if (gNet_players[i].player_status < ePlayer_status_racing) {
            headup_pairs[i].player_index = -1;
            if (ascending_order) {
                headup_pairs[i].score = 1000001;
            } else {
                headup_pairs[i].score = -1000001;
            }
            headup_pairs[i].out_of_game = 1000;
        } else {
            headup_pairs[i].player_index = i;
            headup_pairs[i].score = gNet_players[i].score;
            if (abs(gNet_players[i].score) != 1000000 && (gNet_players[i].score >= 0 || gCurrent_net_game->type == eNet_game_type_car_crusher)) {
                headup_pairs[i].out_of_game = 0;
            } else {
                headup_pairs[i].out_of_game = gNet_players[i].last_score_index + 1;
            }
        }
    }
    for (i = gNumber_of_net_players; i < COUNT_OF(headup_pairs); i++) {
        headup_pairs[i].player_index = -1;
        if (ascending_order) {
            headup_pairs[i].score = 1000001;
        } else {
            headup_pairs[i].score = -1000001;
        }
        headup_pairs[i].out_of_game = 1000;
    }
    qsort(headup_pairs, COUNT_OF(headup_pairs), sizeof(tHeadup_pair), (int (*)(const void*, const void*))(ascending_order ? SortNetHeadAscending : SortNetHeadDescending));

    right_edge = gCurrent_graf_data->net_head_box_x + gCurrent_graf_data->net_head_box_width + 5 * gCurrent_graf_data->net_head_box_pitch;
    for (i = 0; i < COUNT_OF(headup_pairs); i++) {
        index = headup_pairs[i].player_index;
        if (index >= 0) {
            gNet_players[index].last_score_index = i;
        }
        if (pOnly_sort_scores) {
            continue;
        }
        x = gCurrent_graf_data->net_head_box_x + i * gCurrent_graf_data->net_head_box_pitch;
        if (gCurrent_graf_data->net_head_box_bot > gProgram_state.current_render_top) {
            DimRectangle(gBack_screen, x, gCurrent_graf_data->net_head_box_top, x + gCurrent_graf_data->net_head_box_width, gCurrent_graf_data->net_head_box_bot, 1);
        }
        if (index >= 0) {
            Flash(200, &last_flash, &flash_state);
            if (flash_state
                || (gCurrent_net_game->type != eNet_game_type_tag && gCurrent_net_game->type != eNet_game_type_foxy)
                || index != gIt_or_fox) {
                if (gNet_players[index].name_not_clipped) {
                    ClipName(gNet_players[index].player_name, &gFonts[6], gCurrent_graf_data->net_head_box_width - gCurrent_graf_data->net_head_name_x_marg - 2);
                    gNet_players[index].name_not_clipped = 0;
                }
                TransDRPixelmapText(gBack_screen, x + gCurrent_graf_data->net_head_name_x_marg, gCurrent_graf_data->net_head_name_y, &gFonts[6], gNet_players[index].player_name, right_edge);
            }
            if (abs(gNet_players[index].score) == 1000000) {
                if (flash_state) {
                    strcpy(s, GetMiscString(kMiscString_WASTED_173));
                } else {
                    s[0] = '\0';
                }
            } else {
                switch (gCurrent_net_game->type) {
                case eNet_game_type_fight_to_death:
                    sprintf(s, "%d%%", gNet_players[index].score);
                    break;
                case eNet_game_type_car_crusher:
                case eNet_game_type_carnage:
                    sprintf(s, "%d", gNet_players[index].score);
                    break;
                case eNet_game_type_checkpoint:
                    sprintf(s, "%d left", gNet_players[index].score >> 16);
                    break;
                case eNet_game_type_sudden_death:
                    if (gNet_players[index].score < 0) {
                        if (flash_state) {
                            sprintf(s, "%s", GetMiscString(kMiscString_OUT));
                        } else {
                            s[0] = '\0';
                        }
                    } else {
                        score = gNet_players[index].score;
                        sprintf(s, "%s -%d-", GetMiscString(kMiscString_IN), score);
                    }
                    break;
                case eNet_game_type_tag:
                case eNet_game_type_foxy:
                    if (gNet_players[index].score >= 0) {
                        if (index == gIt_or_fox && !flash_state) {
                            s[0] = '\0';
                        } else {
                            TimerString(gNet_players[index].score, s, 0, 1);
                        }
                    } else {
                        sprintf(s, "%s", GetMiscString(kMiscString_OUT));
                    }
                    break;
                default:
                    break;
                }
            }
            len = DRTextWidth(&gFonts[6], s);
            TransDRPixelmapText(gBack_screen, x + gCurrent_graf_data->net_head_score_x - len, gCurrent_graf_data->net_head_score_y, &gFonts[6], s, right_edge);
            DRPixelmapRectangleMaskedCopy(gBack_screen, x + gCurrent_graf_data->net_head_num_x, gCurrent_graf_data->net_head_num_y, gDigits_pix, 0, i * gCurrent_graf_data->net_head_num_height, gDigits_pix->width, gCurrent_graf_data->net_head_num_height);
            DRPixelmapRectangleMaskedCopy(gBack_screen, x + gCurrent_graf_data->net_head_icon_x, gCurrent_graf_data->net_head_icon_y, gIcons_pix, 0, gCurrent_graf_data->net_head_icon_height * gNet_players[index].car_index, gIcons_pix->width, gCurrent_graf_data->net_head_icon_height);
            if (gNet_players[index].ID == gLocal_net_ID) {
                BrPixelmapLine(gBack_screen, x, gCurrent_graf_data->net_head_box_top, x, gCurrent_graf_data->net_head_box_bot - 1, 0);
                BrPixelmapLine(gBack_screen, x + gCurrent_graf_data->net_head_box_width - 1, gCurrent_graf_data->net_head_box_top, x + gCurrent_graf_data->net_head_box_width - 1, gCurrent_graf_data->net_head_box_bot - 1, 0);
                BrPixelmapLine(gBack_screen, x + 1, gCurrent_graf_data->net_head_box_bot, x + gCurrent_graf_data->net_head_box_width - 2, gCurrent_graf_data->net_head_box_bot, 0);
                BrPixelmapLine(gBack_screen, x - 1, gCurrent_graf_data->net_head_box_top, x - 1, gCurrent_graf_data->net_head_box_bot - 1, 255);
                BrPixelmapLine(gBack_screen, x + gCurrent_graf_data->net_head_box_width, gCurrent_graf_data->net_head_box_top, x + gCurrent_graf_data->net_head_box_width, gCurrent_graf_data->net_head_box_bot - 1, 255);
                BrPixelmapLine(gBack_screen, x + 1, gCurrent_graf_data->net_head_box_bot + 1, x + gCurrent_graf_data->net_head_box_width - 2, gCurrent_graf_data->net_head_box_bot + 1, 255);
                BrPixelmapPixelSet(gBack_screen, x, gCurrent_graf_data->net_head_box_bot, 255);
                BrPixelmapPixelSet(gBack_screen, x + gCurrent_graf_data->net_head_box_width - 1, gCurrent_graf_data->net_head_box_bot, 255);
            }
        }
    }
}

// IDA: void __cdecl DoNetScores()
// FUNCTION: CARM95 0x004321a5
void DoNetScores(void) {

    DoNetScores2(0);
}

// IDA: void __cdecl InitNetHeadups()
// FUNCTION: CARM95 0x004321ba
void InitNetHeadups(void) {

    gIcons_pix = LoadPixelmap("CARICONS.PIX");
    if (gIcons_pix != NULL) {
        BrMapAdd(gIcons_pix);
    }
    gDigits_pix = LoadPixelmap("HDIGITS.PIX");
    if (gDigits_pix != NULL) {
        BrMapAdd(gDigits_pix);
    }
    /* The Windows version does not use gIcons_pix_low_res. */
    if (gGraf_data_index != 0) {
        SwitchToLoresMode();
        gIcons_pix_low_res = LoadPixelmap("CARICONS.PIX");
        SwitchToRealResolution();
    } else {
        gIcons_pix_low_res = gIcons_pix;
    }
}

// IDA: void __cdecl DisposeNetHeadups()
// FUNCTION: CARM95 0x0043221f
void DisposeNetHeadups(void) {

    /* Windows version does not use gIcons_pix_low_res. */
    if (gIcons_pix_low_res != NULL && gIcons_pix_low_res != gIcons_pix) {
        BrPixelmapFree(gIcons_pix_low_res);
    }

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
// FUNCTION: CARM95 0x00433554
void EverybodysLost(void) {
    tNet_message* the_message;
    int i;

    for (i = 0; i < gNumber_of_net_players; i++) {
        gNet_players[i].played += 1;
        the_message = NetBuildMessage(NETMSGID_RACEOVER, 0);
        the_message->contents.data.race_over.reason = eRace_over_network_loss;
        NetGuaranteedSendMessageToPlayer(gCurrent_net_game, the_message, gNet_players[i].ID, NULL);
    }
}

// IDA: void __usercall DeclareWinner(int pWinner_index@<EAX>)
// FUNCTION: CARM95 0x0043227c
void DeclareWinner(int pWinner_index) {
    tNet_message* the_message;
    int i;
    int j;
    int best_score_index;
    char s[256];

    DoNetScores2(1);
    the_message = NetBuildMessage(NETMSGID_RACEOVER, 0);
    the_message->contents.data.race_over.reason = eRace_over_network_victory;
    NetGuaranteedSendMessageToPlayer(gCurrent_net_game, the_message, gNet_players[pWinner_index].ID, NULL);
    gNet_players[pWinner_index].won++;
    if (gCurrent_net_game->type == eNet_game_type_sudden_death
        || gCurrent_net_game->type == eNet_game_type_tag
        || gCurrent_net_game->type == eNet_game_type_fight_to_death) {
        gNet_players[pWinner_index].games_score += gGame_scores[5];
    }
    sprintf(s, "%s %s", gNet_players[pWinner_index].player_name, GetMiscString(kMiscString_IS_THE_WINNER));
    for (i = 0; i < gNumber_of_net_players; i++) {
        if (gCurrent_net_game->type != eNet_game_type_sudden_death && gCurrent_net_game->type != eNet_game_type_tag && gCurrent_net_game->type != eNet_game_type_fight_to_death) {
            best_score_index = gNet_players[i].last_score_index;
            for (j = 0; j < gNumber_of_net_players; j++) {
                if (gNet_players[i].score == gNet_players[j].score && gNet_players[j].last_score_index < best_score_index) {
                    best_score_index = gNet_players[j].last_score_index;
                }
            }
            gNet_players[i].games_score += gGame_scores[5 - best_score_index];
        }
        gNet_players[i].played++;
        if (i != pWinner_index) {
            the_message = NetBuildMessage(NETMSGID_RACEOVER, 0);
            the_message->contents.data.race_over.reason = eRace_over_network_loss;
            NetGuaranteedSendMessageToPlayer(gCurrent_net_game, the_message, gNet_players[i].ID, NULL);
            NetSendHeadupToPlayer(s, gNet_players[i].ID);
        }
    }
    gReceived_game_scores = 1;
    the_message = NetBuildMessage(NETMSGID_GAMESCORES, 0);
    for (i = 0; i < gNumber_of_net_players; i++) {
        the_message->contents.data.game_scores.scores[i].played = gNet_players[i].played;
        the_message->contents.data.game_scores.scores[i].won = gNet_players[i].won;
        the_message->contents.data.game_scores.scores[i].score = gNet_players[i].games_score;
    }
    NetGuaranteedSendMessageToAllPlayers(gCurrent_net_game, the_message, NULL);
}

// IDA: void __usercall PlayerIsIt(tNet_game_player_info *pPlayer@<EAX>)
// FUNCTION: CARM95 0x004325be
void PlayerIsIt(tNet_game_player_info* pPlayer) {
    int i;
    char s[256];

    if (pPlayer - gNet_players == gIt_or_fox) {
        return;
    }
    for (i = 0; i < gNumber_of_net_players; i++) {
        StopCarBeingIt(gNet_players[i].car);
    }
    if (gCurrent_net_game->type == eNet_game_type_foxy) {
        pPlayer->car->power_up_levels[1] = 0;
    } else if (gCurrent_net_game->type == eNet_game_type_tag && gIt_or_fox >= 0) {
        gNet_players[gIt_or_fox].car->power_up_levels[1] = 0;
    }
    MakeCarIt(pPlayer->car);
    gIt_or_fox = pPlayer - gNet_players;
    sprintf(s, "%s", GetMiscString(gCurrent_net_game->type == eNet_game_type_tag ? kMiscString_HA_HA_YOU_ARE_IT : kMiscString_YOU_ARE_THE_FOX));
    NetSendHeadupToPlayer(s, pPlayer->ID);
    for (i = 0; i < gNumber_of_net_players; i++) {
        if (&gNet_players[i] != pPlayer) {
            sprintf(s, "%s %s", pPlayer->player_name, GetMiscString(gCurrent_net_game->type == eNet_game_type_tag ? 185 : 187));
            NetSendHeadupToPlayer(s, gNet_players[i].ID);
        }
    }
}

// IDA: int __usercall FarEnoughAway@<EAX>(tNet_game_player_info *pPlayer_1@<EAX>, tNet_game_player_info *pPlayer_2@<EDX>)
// FUNCTION: CARM95 0x004335cd
int FarEnoughAway(tNet_game_player_info* pPlayer_1, tNet_game_player_info* pPlayer_2) {
    br_vector3 difference;

    BrVector3Sub(&difference, &pPlayer_1->car->pos, &pPlayer_2->car->pos);
    return BrVector3LengthSquared(&difference) >= 4.0;
}

// IDA: void __usercall CarInContactWithItOrFox(tNet_game_player_info *pPlayer@<EAX>)
// FUNCTION: CARM95 0x004327a5
void CarInContactWithItOrFox(tNet_game_player_info* pPlayer) {

    if (gCurrent_net_game->type == eNet_game_type_tag || gCurrent_net_game->type == eNet_game_type_foxy) {
        if (PDGetTotalTime() - gLast_it_change > 500) {
            gLast_it_change = PDGetTotalTime();
            if (gIt_or_fox >= 0) {
                gLast_lepper = &gNet_players[gIt_or_fox];
            }
            PlayerIsIt(pPlayer);
        }
    }
}

// IDA: void __usercall SelectRandomItOrFox(int pNot_this_one@<EAX>)
// FUNCTION: CARM95 0x00433676
void SelectRandomItOrFox(int pNot_this_one) {
    int i;
    int new_choice;

    new_choice = 0;
    gLast_lepper = NULL;
    if (gCurrent_net_game->type == eNet_game_type_tag) {
        for (i = 0; i < gNumber_of_net_players; i++) {
            if (gNet_players[i].last_score_index == 0) {
                new_choice = i;
                break;
            }
        }
    } else {
        for (i = 0; i < gNumber_of_net_players; i++) {
            if (i != pNot_this_one && gNet_players[i].last_score_index == gNumber_of_net_players - 1) {
                PlayerIsIt(&gNet_players[i]);
                return;
            }
        }
        do {
            new_choice = IRandomBetween(0, gNumber_of_net_players - 1);
        } while (new_choice == pNot_this_one && !gNet_players[new_choice].car->knackered);
    }
    PlayerIsIt(&gNet_players[new_choice]);
}

// IDA: void __cdecl CalcPlayerScores()
// FUNCTION: CARM95 0x00432865
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

    time = GetTotalTime();

    if (gCurrent_net_game->type == eNet_game_type_carnage) {
        highest = 0;
        next_highest = 0;
        for (i = 0; i < gNumber_of_net_players; ++i) {
            if (gNet_players[i].score > highest) {
                next_highest = highest;
                highest = gNet_players[i].score;
            } else if (gNet_players[i].score > next_highest) {
                next_highest = gNet_players[i].score;
            }
        }
        gPed_target = (gTotal_peds - (gProgram_state.peds_killed - highest - next_highest)) / 2 + 1;
        if (gCurrent_net_game->options.race_end_target < gPed_target) {
            gPed_target = gCurrent_net_game->options.race_end_target;
        }
    } else if (gCurrent_net_game->type == eNet_game_type_tag || gCurrent_net_game->type == eNet_game_type_foxy) {
        if (gIt_or_fox < 0) {
            SelectRandomItOrFox(-1);
        }
        if (gLast_lepper != NULL && gIt_or_fox >= 0 && FarEnoughAway(gLast_lepper, &gNet_players[gIt_or_fox])) {
            gLast_lepper = NULL;
        }
    }
    lowest_score = 9999;
    lowest_score_player = NULL;
    for (i = 0; i < gNumber_of_net_players; i++) {
        car = gNet_players[i].car;
        if (gNet_players[i].reposition_time != 0 && gNet_players[i].reposition_time <= time && (!gRace_finished || gRace_over_reason == -1)) {
            RepositionPlayer(i);
        }
        if (gNet_players[i].last_waste_message != 0
            && !gNet_players[i].wasteage_attributed
            && time - gNet_players[i].last_waste_message > 500) {
            sprintf(s, "%s %s", gNet_players[i].player_name, GetMiscString(kMiscString_COMMITTED_SUICIDE));
            gNet_players[i].last_waste_message = 0;
            gNet_players[i].wasteage_attributed = 0;
            if (gCurrent_net_game->type == eNet_game_type_car_crusher) {
                gNet_players[i].score--;
            }
            NetSendHeadupToEverybody(s);
        }
        SetKnackeredFlag(car);
        if (car->knackered && !gNet_players[i].wasted) {
            gNet_players[i].wasted = 1;
            message = NetBuildMessage(NETMSGID_WASTED, 0);
            message->contents.data.wasted.victim = gNet_players[i].ID;
            message->contents.data.wasted.culprit = -1;
            NetGuaranteedSendMessageToEverybody(gCurrent_net_game, message, 0);
            switch (gCurrent_net_game->type) {
            case eNet_game_type_fight_to_death:
                cars_left = 0;
                for (j = 0; j < gNumber_of_net_players; j++) {
                    if (!gNet_players[j].wasted) {
                        cars_left++;
                        car_left = j;
                    }
                }
                gNet_players[i].games_score += gGame_scores[5 - cars_left];
                if (cars_left == 1) {
                    DeclareWinner(car_left);
                } else if (cars_left <= 0) {
                    EverybodysLost();
                }
                break;
            case eNet_game_type_car_crusher:
            case eNet_game_type_sudden_death:
                gNet_players[i].reposition_time = GetTotalTime() + 5000;
                break;
            case eNet_game_type_carnage:
                gNet_players[i].reposition_time = GetTotalTime() + 5000;
                gNet_players[i].score /= 2;
                break;
            case eNet_game_type_checkpoint:
                if (gNet_players[i].score >> 16 != gCurrent_race.check_point_count) {
                    knock_out_bit = IRandomBetween(0, gCurrent_race.check_point_count - 1);
                    while (((1 << knock_out_bit) & gNet_players[i].score) != 0) {
                        knock_out_bit++;
                        if (gCurrent_race.check_point_count <= knock_out_bit) {
                            knock_out_bit = 0;
                        }
                    }
                    gNet_players[i].score |= 1 << knock_out_bit;
                }
                gNet_players[i].reposition_time = GetTotalTime() + 5000;
                break;
            case eNet_game_type_tag:
                gNet_players[i].reposition_time = GetTotalTime() + 5000;
                PlayerIsIt(&gNet_players[i]);
                break;
            case eNet_game_type_foxy:
                gNet_players[i].reposition_time = GetTotalTime() + 5000;
                gNet_players[i].score /= 2;
                if (gNumber_of_net_players > 1 && i == gIt_or_fox) {
                    SelectRandomItOrFox(i);
                }
                break;
            default:
                break;
            }
        }
        switch (gCurrent_net_game->type) {
        case eNet_game_type_fight_to_death:
            if (car->knackered) {
                if (gCurrent_net_game->type == eNet_game_type_checkpoint
                    || gCurrent_net_game->type == eNet_game_type_tag) {
                    gNet_players[i].score = 1000000;
                } else {
                    gNet_players[i].score = -1000000;
                }
            } else {
                e_dam = car->damage_units[eDamage_engine].damage_level;
                t_dam = car->damage_units[eDamage_transmission].damage_level;
                d_dam = car->damage_units[eDamage_driver].damage_level;
                w_dam = (car->damage_units[eDamage_lr_wheel].damage_level
                            + car->damage_units[eDamage_lf_wheel].damage_level
                            + car->damage_units[eDamage_rr_wheel].damage_level
                            + car->damage_units[eDamage_rf_wheel].damage_level)
                    / 4;

                if (e_dam >= t_dam && e_dam >= d_dam && e_dam >= w_dam) {
                    gNet_players[i].score = 100 - e_dam;
                } else if (t_dam >= d_dam && t_dam >= w_dam) {
                    gNet_players[i].score = 100 - t_dam;
                } else if (w_dam >= d_dam) {
                    gNet_players[i].score = 100 - w_dam;
                } else {
                    gNet_players[i].score = 100 - d_dam;
                }
            }
            break;
        case eNet_game_type_carnage:
            if (gNet_players[i].score >= gPed_target && !gRace_finished) {
                DeclareWinner(i);
            }
            break;
        case eNet_game_type_checkpoint:
            score = 0;
            gNet_players[i].score = gNet_players[i].score & 0xffff;
            flags = gNet_players[i].score;
            for (j = 0; j < gCurrent_race.check_point_count; j++) {
                if ((flags & 1) != 0) {
                    score++;
                }
                flags >>= 1;
            }
            gNet_players[i].score |= score << 16;
            if (!score && !gRace_finished) {
                DeclareWinner(i);
            }
            break;
        case eNet_game_type_sudden_death:
            if (gNet_players[i].score >= 0) {
                if (gNet_players[i].score >= lowest_score) {
                    if (gNet_players[i].score == lowest_score) {
                        lowest_score_player = 0;
                    }
                } else {
                    lowest_score = gNet_players[i].score;
                    lowest_score_player = &gNet_players[i];
                }
            }
            break;
        case eNet_game_type_tag:
            if (i == gIt_or_fox && !gCountdown && gNet_players[i].score >= 0) {
                gNet_players[i].score += gFrame_period;
                if (gNet_players[i].score >= gCurrent_net_game->options.race_end_target) {
                    lowest_score_player = &gNet_players[i];
                }
            }
            break;
        case eNet_game_type_foxy:
            if (i == gIt_or_fox && !gCountdown && !gRace_finished) {
                gNet_players[i].score += gFrame_period;
                if (gNet_players[i].score >= gCurrent_net_game->options.race_end_target) {
                    DeclareWinner(i);
                }
            }
            break;
        default:
            continue;
        }
    }
    if ((gCurrent_net_game->type == eNet_game_type_sudden_death || gCurrent_net_game->type == eNet_game_type_tag)
        && lowest_score_player != NULL
        && lowest_score_player->score >= 0) {
        player_left = -1;
        cars_left = 0;
        for (i = 0; i < gNumber_of_net_players; ++i) {
            if (gNet_players[i].score >= 0 && &gNet_players[i] != lowest_score_player) {
                ++cars_left;
                if (player_left == -1) {
                    player_left = i;
                } else {
                    player_left = -2;
                }
            }
        }
        if (cars_left) {
            lowest_score_player->car->knackered = 1;
            lowest_score_player->wasted = 1;
            lowest_score_player->games_score += gGame_scores[5 - cars_left];
            lowest_score_player->score = -1;
            if (player_left == -1) {
                EverybodysLost();
            } else if (player_left < 0) {
                if (gCurrent_net_game->type == eNet_game_type_tag) {
                    SelectRandomItOrFox(i);
                }
                SendGameplay(lowest_score_player->ID, eNet_gameplay_suddenly_death, 0, 0, 0, 0);
                sprintf(s, "%s %s", lowest_score_player->player_name, GetMiscString(kMiscString_IS_OUT));
                NetSendHeadupToEverybody(s);
            } else {
                DeclareWinner(player_left);
            }
        }
    }
}

// IDA: void __cdecl SendPlayerScores()
// FUNCTION: CARM95 0x004337a4
void SendPlayerScores(void) {
    tNet_contents* the_contents;
    int i;

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
// FUNCTION: CARM95 0x00432843
void DoNetGameManagement(void) {

    if (gNet_mode == eNet_mode_host) {
        CalcPlayerScores();
        SendPlayerScores();
    }
}

// IDA: void __usercall InitialisePlayerScore(tNet_game_player_info *pPlayer@<EAX>)
// FUNCTION: CARM95 0x0043385c
void InitialisePlayerScore(tNet_game_player_info* pPlayer) {

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
// FUNCTION: CARM95 0x00433937
void InitPlayers(void) {
    int i;

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
// FUNCTION: CARM95 0x004339be
void BuyPSPowerup(int pIndex) {
    char s[256];
    char s2[256];

    if (gNet_mode == eNet_mode_none) {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 3000, -4, GetMiscString(kMiscString_ONLY_AVAILABLE_IN_NET_GAMES));
    } else if (gProgram_state.current_car.power_up_levels[pIndex] < 4) {
        if (gNet_mode == eNet_mode_none || gPowerup_cost[gProgram_state.current_car.power_up_levels[pIndex]] <= (gProgram_state.credits_earned - gProgram_state.credits_lost)) {
            SpendCredits(gPowerup_cost[gProgram_state.current_car.power_up_levels[pIndex]]);
            ImprovePSPowerup(&gProgram_state.current_car, pIndex);
        } else {
            strcpy(s, GetMiscString(kMiscString_CANNOT_AFFORD_IT));
            sprintf(s2, "%d", gPowerup_cost[gProgram_state.current_car.power_up_levels[pIndex]]);
            SubsStringJob(s, s2);
            NewTextHeadupSlot(eHeadupSlot_misc, 0, 3008, -4, s);
        }
    } else {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 3000, -4, GetMiscString(kMiscString_YOU_ARE_ALREADY_AT_MAX));
    }
}

// IDA: void __cdecl BuyArmour()
// FUNCTION: CARM95 0x00433b29
void BuyArmour(void) {

    BuyPSPowerup(0);
}

// IDA: void __cdecl BuyPower()
// FUNCTION: CARM95 0x00433b3e
void BuyPower(void) {

    if (gNet_mode != eNet_mode_none && gCurrent_net_game->type == eNet_game_type_foxy && gThis_net_player_index == gIt_or_fox) {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 1000, -4, GetMiscString(kMiscString_THE_FOX_CANNOT_DO_THAT));
    } else if (gNet_mode != eNet_mode_none && gCurrent_net_game->type == eNet_game_type_tag && gThis_net_player_index != gIt_or_fox) {
        NewTextHeadupSlot(eHeadupSlot_misc, 0, 1000, -4, GetMiscString(kMiscString_ONLY_IT_CAN_DO_THAT));
    } else {
        BuyPSPowerup(1);
    }
}

// IDA: void __cdecl BuyOffense()
// FUNCTION: CARM95 0x00433bf9
void BuyOffense(void) {

    BuyPSPowerup(2);
}

// IDA: void __usercall UseGeneralScore(int pScore@<EAX>)
// FUNCTION: CARM95 0x00433c0e
void UseGeneralScore(int pScore) {
    int i;

    if (gCurrent_net_game->type == eNet_game_type_carnage) {
        gPed_target = pScore;
    } else if ((gCurrent_net_game->type == eNet_game_type_tag || gCurrent_net_game->type == eNet_game_type_foxy) && gNet_players[gIt_or_fox].ID != pScore) {
        for (i = 0; i < gNumber_of_net_players; i++) {
            StopCarBeingIt(gNet_players[i].car);
        }
        for (i = 0; i < gNumber_of_net_players; i++) {
            if (gNet_players[i].ID == pScore) {
                MakeCarIt(gNet_players[i].car);
                gIt_or_fox = i;
            }
        }
    }
}

// IDA: void __usercall NetSendEnvironmentChanges(tNet_game_player_info *pPlayer@<EAX>)
// FUNCTION: CARM95 0x00433d0c
void NetSendEnvironmentChanges(tNet_game_player_info* pPlayer) {

    SendAllPedestrianPositions(pPlayer->ID);
    SendAllNonCarPositions();
}

// IDA: void __cdecl UpdateEnvironments()
// FUNCTION: CARM95 0x00433e1b
void UpdateEnvironments(void) {
    int i;

    for (i = 1; i < gNumber_of_net_players; i++) {
        if (!gNet_players[i].race_stuff_initialised) {
            NetSendEnvironmentChanges(&gNet_players[i]);
            gNet_players[i].race_stuff_initialised = 1;
        }
        NetSendMessageStacks();
        SendGameplay(gNet_players[i].ID, eNet_gameplay_go_for_it, 0, 0, 0, 0);
    }
}

// IDA: void __usercall ReceivedGameplay(tNet_contents *pContents@<EAX>, tNet_message *pMessage@<EDX>, tU32 pReceive_time@<EBX>)
// FUNCTION: CARM95 0x00433eac
void ReceivedGameplay(tNet_contents* pContents, tNet_message* pMessage, tU32 pReceive_time) {
    int must_revert_reentrancy;
    int gPixel_buffer_size;
    char* gPixels_copy;
    char* gPalette_copy;
    // GLOBAL: CARM95 0x50c61c
    static int pause_semaphore;

    switch (pContents->data.gameplay.mess) {
    case eNet_gameplay_host_paused:
        if (!pause_semaphore) {
            gPixel_buffer_size = gBack_screen->row_bytes * gBack_screen->height;
            gPixels_copy = BrMemAllocate(gPixel_buffer_size, kMem_quit_vfy_pixels);
            gPalette_copy = BrMemAllocate(1024, kMem_quit_vfy_pal);
            memcpy(gPixels_copy, gBack_screen->pixels, gPixel_buffer_size);
            memcpy(gPalette_copy, gCurrent_palette_pixels, 1024);
            pause_semaphore = 1;
            NetFullScreenMessage(kMiscString_PLEASE_WAIT_HOST_HAS_PAUSED, 1);
            must_revert_reentrancy = PermitNetServiceReentrancy();
            do {
                NetService(0);
                if (CheckQuit()) {
                    NetFullScreenMessage(kMiscString_PLEASE_WAIT_HOST_HAS_PAUSED, 1);
                }
            } while (gWaiting_for_unpause
                && gProgram_state.prog_status != eProg_idling
                && (!gRace_finished || gRace_over_reason != eRace_over_abandoned));
            if (must_revert_reentrancy) {
                HaltNetServiceReentrancy();
            }
            gWaiting_for_unpause = 1;
            FadePaletteDown();
            memcpy(gBack_screen->pixels, gPixels_copy, gPixel_buffer_size);
            memcpy(gCurrent_palette_pixels, gPalette_copy, 1024);
            g16bit_palette_valid = 0;
            BrMemFree(gPixels_copy);
            BrMemFree(gPalette_copy);
            PDScreenBufferSwap(0);
            FadePaletteUp();
            pause_semaphore = 0;
        }
        break;
    case eNet_gameplay_earn_credits:
        EarnCredits(pContents->data.gameplay.param_1);
        break;
    case eNet_gameplay_host_unpaused:
        gWaiting_for_unpause = 0;
        break;
    case eNet_gameplay_suicide:
        KnackerThisCar(NetCarFromPlayerID(pMessage->sender));
        break;
    case eNet_gameplay_go_for_it:
        gWait_for_it = 0;
        break;
    default:
        if (gCurrent_net_game->type == eNet_game_type_checkpoint || gCurrent_net_game->type == eNet_game_type_sudden_death || gCurrent_net_game->type == eNet_game_type_tag) {
            switch (pContents->data.gameplay.mess) {
            case eNet_gameplay_checkpoint:
                Checkpoint(pContents->data.gameplay.param_1, 1);
                break;
            case eNet_gameplay_wrong_checkpoint:
                WrongCheckpoint(pContents->data.gameplay.param_1);
                break;
            case eNet_gameplay_suddenly_death:
                DoFancyHeadup(kFancyHeadupNetworkRaceOverNetworkLoss);
                ChangeAmbientPratcam(kPratcam_network_timeout);
                gRace_finished = 1;
                break;
            default:
                break;
            }
        }
        break;
    }
}

// IDA: void __usercall SendGameplay(tPlayer_ID pPlayer@<EAX>, tNet_gameplay_mess pMess@<EDX>, int pParam_1@<EBX>, int pParam_2@<ECX>, int pParam_3, int pParam_4)
// FUNCTION: CARM95 0x00434179
void SendGameplay(tPlayer_ID pPlayer, tNet_gameplay_mess pMess, int pParam_1, int pParam_2, int pParam_3, int pParam_4) {
    tNet_message* the_message;

    the_message = NetBuildMessage(NETMSGID_GAMEPLAY, 0);
    the_message->contents.data.gameplay.mess = pMess;
    the_message->contents.data.gameplay.param_1 = pParam_1;
    the_message->contents.data.gameplay.param_2 = pParam_2;
    the_message->contents.data.gameplay.param_3 = pParam_3;
    the_message->contents.data.gameplay.param_4 = pParam_4;
    NetGuaranteedSendMessageToPlayer(gCurrent_net_game, the_message, pPlayer, 0);
}

// IDA: void __usercall SendGameplayToAllPlayers(tNet_gameplay_mess pMess@<EAX>, int pParam_1@<EDX>, int pParam_2@<EBX>, int pParam_3@<ECX>, int pParam_4)
// FUNCTION: CARM95 0x004341db
void SendGameplayToAllPlayers(tNet_gameplay_mess pMess, int pParam_1, int pParam_2, int pParam_3, int pParam_4) {
    tNet_message* the_message;

    the_message = NetBuildMessage(NETMSGID_GAMEPLAY, 0);
    the_message->contents.data.gameplay.mess = pMess;
    the_message->contents.data.gameplay.param_1 = pParam_1;
    the_message->contents.data.gameplay.param_2 = pParam_2;
    the_message->contents.data.gameplay.param_3 = pParam_3;
    the_message->contents.data.gameplay.param_4 = pParam_4;
    NetGuaranteedSendMessageToAllPlayers(gCurrent_net_game, the_message, NULL);
}

// IDA: void __usercall SendGameplayToHost(tNet_gameplay_mess pMess@<EAX>, int pParam_1@<EDX>, int pParam_2@<EBX>, int pParam_3@<ECX>, int pParam_4)
// FUNCTION: CARM95 0x00434239
void SendGameplayToHost(tNet_gameplay_mess pMess, int pParam_1, int pParam_2, int pParam_3, int pParam_4) {
    tNet_message* the_message;

    if (gNet_mode == eNet_mode_client) {
        the_message = NetBuildMessage(NETMSGID_GAMEPLAY, 0);
        the_message->contents.data.gameplay.mess = pMess;
        the_message->contents.data.gameplay.param_1 = pParam_1;
        the_message->contents.data.gameplay.param_2 = pParam_2;
        the_message->contents.data.gameplay.param_3 = pParam_3;
        the_message->contents.data.gameplay.param_4 = pParam_4;
        NetGuaranteedSendMessageToHost(gCurrent_net_game, the_message, NULL);
    }
}

// IDA: void __cdecl InitNetGameplayStuff()
// FUNCTION: CARM95 0x004342a4
void InitNetGameplayStuff(void) {
}

// IDA: void __cdecl DefaultNetName()
// FUNCTION: CARM95 0x004342cc
void DefaultNetName(void) {
    NetObtainSystemUserName(gNet_player_name, 32);
}

// IDA: void __usercall NetSendPointCrush(tCar_spec *pCar@<EAX>, tU16 pCrush_point_index@<EDX>, br_vector3 *pEnergy_vector@<EBX>)
// FUNCTION: CARM95 0x004342e6
void NetSendPointCrush(tCar_spec* pCar, tU16 pCrush_point_index, br_vector3* pEnergy_vector) {
    tNet_contents* contents;

    contents = NetGetBroadcastContents(NETMSGID_CRUSHPOINT, 0);
    contents->data.crush.id = NetPlayerFromCar(pCar)->ID;
    contents->data.crush.vertex = pCrush_point_index;
    BrVector3Copy(&contents->data.crush.energy_vector, pEnergy_vector);
}

// IDA: void __usercall RecievedCrushPoint(tNet_contents *pContents@<EAX>)
// FUNCTION: CARM95 0x00434346
void RecievedCrushPoint(tNet_contents* pContents) {
    tCar_spec* car;

    car = NetCarFromPlayerID(pContents->data.crush.id);
    if (car == NULL || gNet_mode == eNet_mode_host || car->active || gArrow_mode) {
        return;
    }
    if (car->car_model_actors[car->principal_car_actor].crush_data.number_of_crush_points == 0) {
        return;
    }

    CrushModelPoint(
        car,
        car->principal_car_actor,
        car->car_model_actors[car->principal_car_actor].actor->model,
        pContents->data.crush.vertex,
        &pContents->data.crush.energy_vector,
        BrVector3Length(&pContents->data.crush.energy_vector) + 0.06f,
        &car->car_model_actors[car->principal_car_actor].crush_data);
    SetModelForUpdate(car->car_model_actors[car->principal_car_actor].actor->model, car, 0);
}

// IDA: void __usercall GetReducedMatrix(tReduced_matrix *m1@<EAX>, br_matrix34 *m2@<EDX>)
// FUNCTION: CARM95 0x0043447f
void GetReducedMatrix(tReduced_matrix* m1, br_matrix34* m2) {

    m1->row1.v[0] = m2->m[0][0];
    m1->row1.v[1] = m2->m[0][1];
    m1->row1.v[2] = m2->m[0][2];
    m1->row2.v[0] = m2->m[1][0];
    m1->row2.v[1] = m2->m[1][1];
    m1->row2.v[2] = m2->m[1][2];
    m1->translation.v[0] = m2->m[3][0];
    m1->translation.v[1] = m2->m[3][1];
    m1->translation.v[2] = m2->m[3][2];
}

// IDA: void __usercall GetExpandedMatrix(br_matrix34 *m1@<EAX>, tReduced_matrix *m2@<EDX>)
// FUNCTION: CARM95 0x004344f4
void GetExpandedMatrix(br_matrix34* m1, tReduced_matrix* m2) {

    m1->m[0][0] = m2->row1.v[0];
    m1->m[0][1] = m2->row1.v[1];
    m1->m[0][2] = m2->row1.v[2];

    m1->m[1][0] = m2->row2.v[0];
    m1->m[1][1] = m2->row2.v[1];
    m1->m[1][2] = m2->row2.v[2];

    m1->m[3][0] = m2->translation.v[0];
    m1->m[3][1] = m2->translation.v[1];
    m1->m[3][2] = m2->translation.v[2];

    m1->m[2][0] = m2->row2.v[2] * m2->row1.v[1] - m2->row2.v[1] * m2->row1.v[2];
    m1->m[2][1] = m2->row1.v[2] * m2->row2.v[0] - m2->row2.v[2] * m2->row1.v[0];
    m1->m[2][2] = m2->row2.v[1] * m2->row1.v[0] - m2->row1.v[1] * m2->row2.v[0];
}

// IDA: void __usercall NetEarnCredits(tNet_game_player_info *pPlayer@<EAX>, tS32 pCredits@<EDX>)
// FUNCTION: CARM95 0x004345c7
void NetEarnCredits(tNet_game_player_info* pPlayer, tS32 pCredits) {

    // empty function
}
