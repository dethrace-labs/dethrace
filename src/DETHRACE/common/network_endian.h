#ifndef NETWORK_ENDIAN_H
#define NETWORK_ENDIAN_H

#if BR_ENDIAN_BIG

#include "dr_types.h"

static void NetSwapVector3(br_vector3* pVector) {
    for (int i = 0; i < 3; i++) {
        pVector->v[i] = BrSwapFloat(pVector->v[i]);
    }
}

static void NetSwapMatrix34(br_matrix34* pMatrix) {
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 3; col++) {
            pMatrix->m[row][col] = BrSwapFloat(pMatrix->m[row][col]);
        }
    }
}

static void NetSwapMessage(tNet_message* pMessage) {
    pMessage->magic_number = BrSwap32(pMessage->magic_number);
    pMessage->guarantee_number = BrSwap32(pMessage->guarantee_number);
    pMessage->sender = BrSwap32(pMessage->sender);
    pMessage->version = BrSwap32(pMessage->version);
    pMessage->senders_time_stamp = BrSwap32(pMessage->senders_time_stamp);
    pMessage->num_contents = BrSwap16(pMessage->num_contents);
    // pMessage->overall_size = BrSwap16(pMessage->overall_size);
}

static void NetSwapPlayerInfo(tNet_game_player_info* pPlayer) {
    pPlayer->this_players_time_stamp = BrSwap32(pPlayer->this_players_time_stamp);
    pPlayer->last_heard_from_him = BrSwap32(pPlayer->last_heard_from_him);
    pPlayer->reposition_time = BrSwap32(pPlayer->reposition_time);
    pPlayer->last_waste_message = BrSwap32(pPlayer->last_waste_message);
    pPlayer->host = BrSwap32(pPlayer->host);
    pPlayer->ID = BrSwap32(pPlayer->ID);
    pPlayer->car_index = BrSwap32(pPlayer->car_index);
    pPlayer->grid_index = BrSwap32(pPlayer->grid_index);
    pPlayer->grid_position_set = BrSwap32(pPlayer->grid_position_set);
    pPlayer->opponent_list_index = BrSwap32(pPlayer->opponent_list_index);
    pPlayer->score = BrSwap32(pPlayer->score);
    pPlayer->credits = BrSwap32(pPlayer->credits);
    pPlayer->wasted = BrSwap32(pPlayer->wasted);
    pPlayer->wasteage_attributed = BrSwap32(pPlayer->wasteage_attributed);
    pPlayer->name_not_clipped = BrSwap32(pPlayer->name_not_clipped);
    pPlayer->race_stuff_initialised = BrSwap32(pPlayer->race_stuff_initialised);
    pPlayer->played = BrSwap32(pPlayer->played);
    pPlayer->won = BrSwap32(pPlayer->won);
    pPlayer->next_car_index = BrSwap32(pPlayer->next_car_index);
    pPlayer->last_score_index = BrSwap32(pPlayer->last_score_index);
    NetSwapMatrix34(&pPlayer->initial_position);
}

static void NetSwapOptions(tNet_game_options* pOptions) {
    pOptions->show_players_on_map = BrSwap32(pOptions->show_players_on_map);
    pOptions->show_peds_on_map = BrSwap32(pOptions->show_peds_on_map);
    pOptions->enable_text_messages = BrSwap32(pOptions->enable_text_messages);
    pOptions->show_powerups_on_map = BrSwap32(pOptions->show_powerups_on_map);
    pOptions->powerup_respawn = BrSwap32(pOptions->powerup_respawn);
    pOptions->open_game = BrSwap32(pOptions->open_game);
    pOptions->starting_money_index = BrSwap32(pOptions->starting_money_index);
    pOptions->grid_start = BrSwap32(pOptions->grid_start);
    pOptions->race_end_target = BrSwap32(pOptions->race_end_target);
    pOptions->random_car_choice = BrSwap32(pOptions->random_car_choice);
    pOptions->race_sequence_type = BrSwap32(pOptions->race_sequence_type);
    pOptions->car_choice = BrSwap32(pOptions->car_choice);
}

static void NetSwapGameDetails(tNet_game_details* pDetails) {
    pDetails->host_ID = BrSwap32(pDetails->host_ID);
    pDetails->num_players = BrSwap32(pDetails->num_players);
    pDetails->start_race = BrSwap32(pDetails->start_race);
    pDetails->no_races_yet = BrSwap32(pDetails->no_races_yet);
    pDetails->status.stage = BrSwap32(pDetails->status.stage);
    NetSwapOptions(&pDetails->options);
    pDetails->type = BrSwap32(pDetails->type);
}

static void NetSwapJoin(tNet_message_join* pJoin) {
    NetSwapPlayerInfo(&pJoin->player_info);
}

static void NetSwapPlayerList(tNet_message_new_player_list* pPlayerList) {
    pPlayerList->number_of_players = BrSwap32(pPlayerList->number_of_players);
    pPlayerList->this_index = BrSwap32(pPlayerList->this_index);
    pPlayerList->batch_number = BrSwap32(pPlayerList->batch_number);
    NetSwapPlayerInfo(&pPlayerList->player);
}

static void NetSwapGuaranteeReply(tNet_message_guarantee_reply* pReply) {
    pReply->guarantee_number = BrSwap32(pReply->guarantee_number);
}

static void NetSwapCarDetails(tNet_message_car_details* pCarDetails) {
    pCarDetails->count = BrSwap32(pCarDetails->count);
    for (int j = 0; j < 6; j++) {
        pCarDetails->details[j].car_index = BrSwap32(pCarDetails->details[j].car_index);
    }
}

static void NetSwapRaceOver(tNet_message_race_over* pRaceOver) {
    pRaceOver->reason = BrSwap32(pRaceOver->reason);
}

static void NetSwapStatusReport(tNet_message_status_report* pStatusReport) {
    pStatusReport->status = BrSwap32(pStatusReport->status);
}

static void NetSwapStartRace(tNet_message_start_race* pStartRace) {
    pStartRace->car_count = BrSwap32(pStartRace->car_count);
    pStartRace->racing = BrSwap32(pStartRace->racing);
    pStartRace->next_race = BrSwap32(pStartRace->next_race);
    for (int i = 0; i < 6; i++) {
        pStartRace->car_list[i].index = BrSwap32(pStartRace->car_list[i].index);
        pStartRace->car_list[i].next_car_index = BrSwap32(pStartRace->car_list[i].next_car_index);
        NetSwapMatrix34(&pStartRace->car_list[i].mat);
    }
}

static void NetSwapHostReply(tNet_message_host_reply* pHostReply) {
    pHostReply->race_has_started = BrSwap32(pHostReply->race_has_started);
    pHostReply->race_index = BrSwap32(pHostReply->race_index);
    pHostReply->pending_race = BrSwap32(pHostReply->pending_race);
}

static void NetSwapMechanics(tNet_message_mechanics_info* pMechanics) {
    pMechanics->ID = BrSwap32(pMechanics->ID);
    pMechanics->time = BrSwap32(pMechanics->time);
    *(br_uint_32*)&pMechanics->keys = BrSwap32(*(br_uint_32*)&pMechanics->keys);
    pMechanics->cc_coll_time = BrSwap32(pMechanics->cc_coll_time);
    pMechanics->curvature = BrSwap16(pMechanics->curvature);
    pMechanics->revs = BrSwap16(pMechanics->revs);
    pMechanics->front = BrSwapFloat(pMechanics->front);
    pMechanics->back = BrSwapFloat(pMechanics->back);
    pMechanics->repair_time = BrSwap32(pMechanics->repair_time);
    pMechanics->powerups = BrSwap16(pMechanics->powerups);
    NetSwapVector3(&pMechanics->mat.row1);
    NetSwapVector3(&pMechanics->mat.row2);
    NetSwapVector3(&pMechanics->mat.translation);
    NetSwapVector3(&pMechanics->v);
    NetSwapVector3(&pMechanics->omega);
    if (pMechanics->contents_size != sizeof(tNet_message_mechanics_info)) {
        return;
    }
    for (int i = 0; i < 4; i++) {
        pMechanics->wheel_dam_offset[i] = BrSwapFloat(pMechanics->wheel_dam_offset[i]);
    }
}

static void NetSwapNonCar(tNet_message_non_car_info* pNonCarInfo) {
    pNonCarInfo->ID = BrSwap32(pNonCarInfo->ID);
    pNonCarInfo->time = BrSwap32(pNonCarInfo->time);
    pNonCarInfo->cc_coll_time = BrSwap32(pNonCarInfo->cc_coll_time);
    pNonCarInfo->flags = BrSwap16(pNonCarInfo->flags);
    NetSwapVector3(&pNonCarInfo->mat.row1);
    NetSwapVector3(&pNonCarInfo->mat.row2);
    NetSwapVector3(&pNonCarInfo->mat.translation);
    NetSwapVector3(&pNonCarInfo->v);
    NetSwapVector3(&pNonCarInfo->omega);
}

static void NetSwapTimeSync(tNet_message_time_sync* pTimeSync) {
    pTimeSync->race_start_time = BrSwap32(pTimeSync->race_start_time);
}

static void NetSwapConfirm(tNet_message_players_confirm* pConfirm) {
    pConfirm->player = BrSwap32(pConfirm->player);
}

static void NetSwapPowerup(tNet_message_powerup* pPowerup) {
    pPowerup->player = BrSwap32(pPowerup->player);
    pPowerup->powerup_index = BrSwap32(pPowerup->powerup_index);
    pPowerup->time_left = BrSwap32(pPowerup->time_left);
    pPowerup->event = BrSwap32(pPowerup->event);
}

static void NetSwapRecover(tNet_message_recover* pRecover) {
    pRecover->ID = BrSwap32(pRecover->ID);
    pRecover->time_to_recover = BrSwap32(pRecover->time_to_recover);
}

static void NetSwapScores(tNet_message_scores* pScores) {
    pScores->general_score = BrSwap32(pScores->general_score);
    for (int i = 0; i < 6; i++) {
        pScores->scores[i] = BrSwap32(pScores->scores[i]);
    }
}

static void NetSwapWasted(tNet_message_wasted* pWasted) {
    pWasted->victim = BrSwap32(pWasted->victim);
    pWasted->culprit = BrSwap32(pWasted->culprit);
}

static void NetSwapPedestrian(tNet_message_pedestrian* pPedestrian) {
    pPedestrian->index = BrSwap16(pPedestrian->index);
    NetSwapVector3(&pPedestrian->pos);
    pPedestrian->speed = BrSwapFloat(pPedestrian->speed);
    if (!(pPedestrian->flags & 0x20)) {
        return;
    }
    NetSwapVector3(&pPedestrian->to_pos);
    if (!(pPedestrian->flags & 0x40)) {
        return;
    }
    NetSwapVector3(&pPedestrian->offset);
    pPedestrian->murderer = BrSwap32(pPedestrian->murderer);
    pPedestrian->respawn_time_or_spin_period = BrSwap32(pPedestrian->respawn_time_or_spin_period);
}

static void NetSwapGameplay(tNet_message_gameplay* pGameplay) {
    pGameplay->mess = BrSwap16(pGameplay->mess);
    pGameplay->param_1 = BrSwap16(pGameplay->param_1);
    pGameplay->param_2 = BrSwap16(pGameplay->param_2);
    pGameplay->param_3 = BrSwap16(pGameplay->param_3);
    pGameplay->param_4 = BrSwap16(pGameplay->param_4);
}

static void NetSwapNonCarPosition(tNet_message_non_car_position* pNonCarPosition) {
    pNonCarPosition->ID = BrSwap32(pNonCarPosition->ID);
    pNonCarPosition->flags = BrSwap16(pNonCarPosition->flags);
    NetSwapMatrix34(&pNonCarPosition->mat);
}

static void NetSwapCopInfo(tNet_message_cop_info* pCopInfo) {
    pCopInfo->ID = BrSwap32(pCopInfo->ID);
    pCopInfo->time = BrSwap32(pCopInfo->time);
    pCopInfo->curvature = BrSwapFloat(pCopInfo->curvature);
    NetSwapVector3(&pCopInfo->mat.row1);
    NetSwapVector3(&pCopInfo->mat.row2);
    NetSwapVector3(&pCopInfo->mat.translation);
    NetSwapVector3(&pCopInfo->v);
    NetSwapVector3(&pCopInfo->omega);
    for (int i = 0; i < 4; i++) {
        pCopInfo->d[i] = BrSwapFloat(pCopInfo->d[i]);
    }
}

static void NetSwapGameScores(tNet_message_game_scores* pGameScores) {
    for (int i = 0; i < 6; i++) {
        pGameScores->scores[i].played = BrSwap32(pGameScores->scores[i].played);
        pGameScores->scores[i].won = BrSwap32(pGameScores->scores[i].won);
        pGameScores->scores[i].score = BrSwap32(pGameScores->scores[i].score);
    }
}

static void NetSwapOilSpill(tNet_message_oil_spill* pOilSpill) {
    pOilSpill->player = BrSwap32(pOilSpill->player);
    pOilSpill->full_size = BrSwapFloat(pOilSpill->full_size);
    pOilSpill->grow_rate = BrSwapFloat(pOilSpill->grow_rate);
    pOilSpill->current_size = BrSwapFloat(pOilSpill->current_size);
}

static void NetSwapCrushPoint(tNet_message_crush_point* pCrushPoint) {
    pCrushPoint->id = BrSwap32(pCrushPoint->id);
    pCrushPoint->vertex = BrSwap16(pCrushPoint->vertex);
    NetSwapVector3(&pCrushPoint->energy_vector);
}

static void NetSwapContents(tNet_contents* pContents) {
    switch (pContents->header.type) {
    case NETMSGID_SENDMEDETAILS: // 0x00,
        break;
    case NETMSGID_DETAILS: // 0x01,
        NetSwapGameDetails(&pContents->data.details.details);
        break;
    case NETMSGID_JOIN: // 0x02,
        NetSwapJoin(&pContents->data.join);
        break;
    case NETMSGID_NEWPLAYERLIST: // 0x03,
        NetSwapPlayerList(&pContents->data.player_list);
        break;
    case NETMSGID_GUARANTEEREPLY: // 0x04,
        NetSwapGuaranteeReply(&pContents->data.reply);
        break;
    case NETMSGID_CARDETAILSREQ: // 0x05,
        break;
    case NETMSGID_CARDETAILS: // 0x06,
        NetSwapCarDetails(&pContents->data.car_details);
        break;
    case NETMSGID_LEAVE: // 0x07,
        break;
    case NETMSGID_HOSTICIDE: // 0x08,
        break;
    case NETMSGID_RACEOVER: // 0x09,
        NetSwapRaceOver(&pContents->data.race_over);
        break;
    case NETMSGID_STATUSREPORT: // 0x0a,
        NetSwapStatusReport(&pContents->data.report);
        break;
    case NETMSGID_STARTRACE: // 0x0b,
        NetSwapStartRace(&pContents->data.start_race);
        break;
    case NETMSGID_HEADUP: // 0x0c,
        break;
    case NETMSGID_HOSTQUERY: // 0x0d,
        break;
    case NETMSGID_HOSTREPLY: // 0x0e,
        NetSwapHostReply(&pContents->data.heres_where_we_at);
        break;
    case NETMSGID_MECHANICS: // 0x0f,
        NetSwapMechanics(&pContents->data.mech);
        break;
    case NETMSGID_NONCAR_INFO: // 0x10,
        NetSwapNonCar(&pContents->data.non_car);
        break;
    case NETMSGID_TIMESYNC: // 0x11,
        NetSwapTimeSync(&pContents->data.time_sync);
        break;
    case NETMSGID_CONFIRM: // 0x12,
        NetSwapConfirm(&pContents->data.confirm);
        break;
    case NETMSGID_DISABLECAR: // 0x13,
        break;
    case NETMSGID_ENABLECAR: // 0x14,
        break;
    case NETMSGID_POWERUP: // 0x15,
        NetSwapPowerup(&pContents->data.powerup);
        break;
    case NETMSGID_RECOVER: // 0x16,
        NetSwapRecover(&pContents->data.recover);
        break;
    case NETMSGID_SCORES: // 0x17,
        NetSwapScores(&pContents->data.scores);
        break;
    case NETMSGID_WASTED: // 0x18,
        NetSwapWasted(&pContents->data.wasted);
        break;
    case NETMSGID_PEDESTRIAN: // 0x19,
        NetSwapPedestrian(&pContents->data.pedestrian);
        break;
    case NETMSGID_GAMEPLAY: // 0x1a,
        NetSwapGameplay(&pContents->data.gameplay);
        break;
    case NETMSGID_NONCARPOSITION: // 0x1b,
        NetSwapNonCarPosition(&pContents->data.non_car_position);
        break;
    case NETMSGID_COPINFO: // 0x1c,
        NetSwapCopInfo(&pContents->data.cop_info);
        break;
    case NETMSGID_GAMESCORES: // 0x1d,
        NetSwapGameScores(&pContents->data.game_scores);
        break;
    case NETMSGID_OILSPILL: // 0x1e,
        NetSwapOilSpill(&pContents->data.oil_spill);
        break;
    case NETMSGID_CRUSHPOINT: // 0x1f,
        NetSwapCrushPoint(&pContents->data.crush);
        break;
    }
}

static void NetSwapMessageAndContents(tNet_message* pMessage) {
    tNet_contents* contents;
    int i;

    contents = &pMessage->contents;
    for (i = 0; i < pMessage->num_contents; i++) {
        NetSwapContents(contents);
        contents = (tNet_contents*)((tU8*)contents + contents->header.contents_size);
    }
    NetSwapMessage(pMessage);
}

#else

#define NetSwapMessage(a)
#define NetSwapContents(a)
#define NetSwapMessageAndContents(a)

#endif // BR_ENDIAN_BIG

#endif //  NETWORK_ENDIAN_H
