-- Bisic Dethrace Wireshark dissector
--
-- Installation instructions:
-- 1. Open Wireshark GUI
-- 2. Navigate to "Help -> About Wireshark" menu
-- 3. Clock on "Folder" tab andclick in the list of paths on "Personal Lua Plugins".
--    If the folder does not yet exist, a dialog will ask you to.
-- 4. Paste this lua script in the "Personal Lua Plugins" directory, or create a symlink.
-- 5. When modifying this script, reload all plug-ins with CTRL+SHIRT+L

dethrace_protocol = Proto("dethrace",  "Dethrace Protocol")
DETHRACE_PORT = 12286

broadcast_types = {
    [0x31] = "PING Request",
    [0x32] = "PONG Response",
}

content_types = {
    [0x00] = "SendMeDetails",
    [0x01] = "Details",
    [0x02] = "Join",
    [0x03] = "NewPlayerList",
    [0x04] = "GuaranteeReply",
    [0x05] = "CarDetailsReq",
    [0x06] = "CarDetails",
    [0x07] = "Leave",
    [0x08] = "Hosticide",
    [0x09] = "RaceOver",
    [0x0a] = "StatusReport",
    [0x0b] = "StartRace",
    [0x0c] = "HeadUp",
    [0x0d] = "HostQuery",
    [0x0e] = "HostReply",
    [0x0f] = "Mechanics",
    [0x10] = "NonCar_Info",
    [0x11] = "TimeSync",
    [0x12] = "Confirm",
    [0x13] = "DisableCar",
    [0x14] = "EnableCar",
    [0x15] = "PowerUp",
    [0x16] = "Recover",
    [0x17] = "Scores",
    [0x18] = "Wasted",
    [0x19] = "Pedestrian",
    [0x1a] = "Gameplay",
    [0x1b] = "NonCarPosition",
    [0x1c] = "CopInfo",
    [0x1d] = "GameScores",
    [0x1e] = "OilSpill",
    [0x1f] = "CrushPoint",
    [0x20] = "None",
}

f_checksum = ProtoField.uint32("dethrace.checksum", "Checksum")
f_broadcast_magic = ProtoField.string("dethrace.broadcast_magic", "Broadcast Magic", base.ASCII)
f_broadcast_type = ProtoField.char("dethrace.broadcast_type", "Broadcast Type", base.NONE, broadcast_types)

f_terminator = ProtoField.char("dethrace.terminator", "Terminator")

f_message_magic = ProtoField.uint32("dethrace.magic", "Magic", base.HEX)
f_message_guarantee = ProtoField.uint32("dethrace.guarantee", "Guarantee Number")
f_message_sender = ProtoField.uint32("dethrace.sender", "Sender Player ID")
f_message_version = ProtoField.int32("dethrace.version", "Version")
f_message_timestamp = ProtoField.uint32("dethrace.time", "Sender Time Stamp")
f_message_count = ProtoField.uint16("dethrace.count", "Count Contents")
f_message_size = ProtoField.uint16("dethrace.size", "Overall Size")

f_contents_size = ProtoField.uint8("dethrace.content_size", "Content Size")
f_contents_type = ProtoField.uint8("dethrace.content_type", "Content Type", base.DEC, content_types)

function create_message_size_type(parent, message, name)
    local message_type = message(1, 1):uint()
    local tree = parent:add(dethrace_protocol, message(), string.format("%s (%d)", name, message_type))
    tree:add_le(f_contents_size, message(0, 1))
    tree:add_le(f_contents_type, message(1, 1))
    return tree
end

f_pd_player_info = ProtoField.bytes("dethrace.pd_info", "PD Net Info")

function dissect_pd_player_info(tree, data)
    tree:add_le(f_pd_player_info, data(0, 18))
end

function dissect_message_sendmedetails(tree, message)
    -- tNet_message_send_me_details (0)
    create_message_size_type(tree, message, "Send Me Details")
    -- finished
end

race_tracks = {
    [0] = "Sumo",
    [1] = "Coliseum",
    [2] = "Maim Street",
    [3] = "Coastal Carnage",
    [4] = "Fridge Racer",
    [5] = "Death Valley",
    [6] = "Off Quay",
    [7] = "Industrial Injury",
    [8] = "Avenue of Atrocities",
    [9] = "Terror on the Trails",
    [10] = "MagnaChem Mayhem",
    [11] = "Downtown Devastation",
}

game_stages = {
    [0] = "Starting",
    [1] = "Ready",
    [2] = "Playing",
}

game_types = {
    [0] = "Driven To Destruction",
    [1] = "Car Crusher",
    [2] = "Carnage",
    [3] = "Checkpoint Stampede",
    [4] = "Sudden Death",
    [5] = "Terminal Tag",
    [6] = "Fox 'n' Hounds",
}

details_options_start_types = {
    [0] = "Random Start",
    [1] = "Grid Start",
}

details_options_starting_money = {
    [0] = "$0",
    [1] = "$2,000",
    [2] = "$5,000",
    [3] = "$10,000",
    [4] = "$20,000",
}

details_options_car_choices = {
    [0] = "Eagle",
    [1] = "Hawk",
    [2] = "Both",
    [3] = "All",
}

f_details_host_name = ProtoField.stringz("dethrace.details.host_name", "Host Name")
f_details_host_id = ProtoField.uint32("dethrace.details.host_id", "Host ID")
f_details_num_players = ProtoField.uint32("dethrace.details.num_players", "Number Players")
f_details_start_race = ProtoField.uint32("dethrace.details.start_race", "Start Race", base.DEC, race_tracks)
f_details_no_races_yet = ProtoField.bool("dethrace.details.no_races_yet", "No Races Yet")
f_details_status = ProtoField.uint32("dethrace.details.game_status", "Status", base.DEC, game_stages)
f_details_type = ProtoField.uint32("dethrace.details.type", "Type", base.DEC, game_types)

f_details_option_show_players_on_map = ProtoField.bool("dethrace.details.options.players_on_map", "Show Players On Map")
f_details_option_show_peds_on_map = ProtoField.bool("dethrace.details.options.peds_on_map", "Show Peds On Map")
f_details_option_enable_text_messages = ProtoField.bool("dethrace.details.options.text_messages", "Enable Text Messages")
f_details_option_show_powerups_on_map = ProtoField.bool("dethrace.details.options.powerups_on_map", "Show Powerups On Map")
f_details_option_powerup_respawn = ProtoField.bool("dethrace.details.options.powerup_respawn", "Powerup Respawn")
f_details_option_open_game = ProtoField.bool("dethrace.details.options.open_game", "Open Game")
f_details_option_starting_money_index = ProtoField.uint32("dethrace.details.options.starting_money", "Starting Money", base.DEC, details_options_starting_money)
f_details_option_start_type = ProtoField.uint32("dethrace.details.options.start_Type", "Start Type", base.DEC, details_options_start_types)
f_details_option_race_end_target = ProtoField.uint32("dethrace.details.options.end_target", "Race End Target")
f_details_option_random_car_choice = ProtoField.bool("dethrace.details.options.random_car", "Random Cars")
f_details_option_race_sequence_type = ProtoField.uint32("dethrace.details.options.race_sequence_type", "Race Sequence Type")
f_details_option_car_choice = ProtoField.uint32("dethrace.details.options.car_choice", "Car Choice", base.DEC, details_options_car_choices)

function dissect_game_details(tree, data)
    -- tNet_game_options
    tree:add_le(f_details_option_show_players_on_map, data(0, 4))
    tree:add_le(f_details_option_show_peds_on_map, data(4, 4))
    tree:add_le(f_details_option_enable_text_messages, data(8, 4))
    tree:add_le(f_details_option_show_powerups_on_map, data(12, 4))
    tree:add_le(f_details_option_powerup_respawn, data(16, 4))
    tree:add_le(f_details_option_open_game, data(20, 4))
    tree:add_le(f_details_option_starting_money_index, data(24, 4))
    tree:add_le(f_details_option_start_type, data(28, 4))
    tree:add_le(f_details_option_race_end_target, data(32, 4))
    tree:add_le(f_details_option_random_car_choice, data(36, 4))
    tree:add_le(f_details_option_race_sequence_type, data(40, 4))
    tree:add_le(f_details_option_car_choice, data(44, 4))
end

function dissect_message_details(tree, message)
    -- tNet_message_my_details (1)
    local t = create_message_size_type(tree, message, "Details")
    dissect_pd_player_info(t, message(2))
    t:add(f_details_host_name, message(20, 32))
    t:add_le(f_details_host_id, message(52, 4))
    t:add_le(f_details_num_players, message(56, 4))
    t:add_le(f_details_start_race, message(60, 4))
    t:add_le(f_details_no_races_yet, message(64, 4))
    t:add_le(f_details_status, message(68, 4))
    do
        local option_data = message(72, 48)
        dissect_game_details(t:add(option_data, "Options"), option_data)
    end
    t:add_le(f_details_type, message(120, 4))
end

function dissect_game_player_info(tree, message)
    -- tNet_game_player_info
end

function dissect_message_join(tree, message)
    -- tNet_message_join (2)
    local t = create_message_size_type(tree, message, "Join (INCOMPLETE)")
    local game_player_info_data = message(4)
    dissect_game_player_info(t, game_player_info_data)
end

function dissect_message_newplayerlist(tree, message)
    -- tNet_message_new_player_list (3)
    local t = create_message_size_type(tree, message, "New Player List")
    -- FIXME
end

f_guaranteereply_number = ProtoField.uint32("dethrace.guaranteereply.number", "Guarantee Number")

function dissect_message_guaranteereply(tree, message)
    -- tNet_message_guarantee_reply (4)
    local t = create_message_size_type(tree, message, "Guarantee Reply")
    t:add_le(f_guaranteereply_number, message(4, 4))
    -- FIXME: add reference to packet with this reply number
    -- finished
end

function dissect_message_cardetailsreq(tree, message)
    -- tNet_message_car_details_req (5)
    create_message_size_type(tree, message, "Car Details Req")
    -- finished
end

f_cardetails_count = ProtoField.uint32("dethrace.cardetails.count", "Car Count")
f_cardetails_car = ProtoField.uint32("dethrace.cardetails.car_index", "Car")
f_cardetails_owner = ProtoField.stringz("dethrace.cardetails.owner", "Owner")

function dissect_message_cardetails(tree, message)
    -- tNet_message_car_details (6)
    local t = create_message_size_type(tree, message, "Car Details")
    t:add_le(f_cardetails_count, message(4, 4))
    for i = 0, 5 do
        local car_detail = message(8 + 20 * i, 20)
        local subtree = t:add(car_detail, string.format("Car %d", i))
        subtree:add_le(f_cardetails_car, car_detail(0, 4))
        subtree:add(f_cardetails_owner, car_detail(4, 16))
    end
    -- finished
end

function dissect_message_leave(tree, message)
    -- tNet_message_leave (7)
    create_message_size_type(tree, message, "Leave")
    -- finished
end

function dissect_message_hosticide(tree, message)
    -- tNet_message_host_pissing_off (8)
    create_message_size_type(tree, message, "Hosticide")
    -- finished
end

raceover_reasons = {
    [-1] = eRace_not_over_yet,
    [ 0] = eRace_over_laps,
    [ 1] = eRace_over_peds,
    [ 2] = eRace_over_opponents,
    [ 3] = eRace_over_abandoned,
    [ 4] = eRace_over_out_of_time,
    [ 5] = eRace_over_demo,
    [ 6] = eRace_over_network_victory,
    [ 7] = eRace_over_network_loss,
}

f_raceover_reason = ProtoField.uint32("dethrace.raceover.reason", "Reason", base.DEC, raceover_reasons)

function dissect_message_raceover(tree, message)
    -- tNet_message_race_over (9)
    local t = create_message_size_type(tree, message, "Race Over (UNTESTED)")
    t:add_le(f_raceover_reason, message(4, 4))
    -- finished
end

player_statuses = {
    [0] = "Unknown",
    [1] = "Ready",
    [2] = "Loading",
    [3] = "Wrecks Gallery",
    [4] = "Summary",
    [5] = "Not Responding",
    [6] = "Racing",
    [7] = "Main Menu",
    [8] = "Recovering",
    [9] = "Action Replay",
}

f_statusreport_status = ProtoField.uint32("dethrace.statusreport.status", "Player Status", base.DEC, player_statuses)

function dissect_message_statusreport(tree, message)
    -- tNet_message_status_report (10)
    local t = create_message_size_type(tree, message, "Status Report (UNTESTED)")
    t:add_le(f_statusReport_status, message(4, 4))
    -- finished
end

f_startrace_count = ProtoField.uint32("dethrace.startrace.count", "Car Count", base.DEC)
f_startrace_racing = ProtoField.bool("dethrace.startrace.racing", "Racing")
f_startrace_next = ProtoField.uint32("dethrace.startrace.next", "Next Race", base.DEC, race_tracks)

function dissect_message_startrace(tree, message)
    -- tNet_message_start_race (11)
    local t = create_message_size_type(tree, message, "Start Race (UNTESTED/INCOMPLETE)")
    t:add_le(f_startrace_count, message(4, 4))
    t:add_le(f_startrace_racing, message(8, 4))
    t:add_le(f_startrace_racing, message(8, 4))
    t:add_le(f_startrace_next, message(12, 4))
    for i = 0, 6 do
        specdata = message(16 + i * 56, 56)
        local subtree = t:add(specdata, string.format("Grid Car %d", i))
        -- FIXME
    end
    -- FIXME
end

f_headup_text = ProtoField.stringz("dethrace.headup.message", "Message")

function dissect_message_headup(tree, message)
    -- tNet_message_headup (12)
    local t = create_message_size_type(tree, message, "Head-Up (UNTESTED)")
    t:add(f_headup_text, message(4, 128))
    -- finished
end

function dissect_message_hostquery(tree, message)
    -- tNet_message_host_query (13)
    create_message_size_type(tree, message, "Host Query")
    -- finished
end

f_hostreply_started = ProtoField.bool("dethrace.hostreply.started", "Race Started")
f_hostreply_race = ProtoField.uint32("dethrace.hostreply.race", "Race", base.DEC, race_tracks)
f_hostreply_pending = ProtoField.uint32("dethrace.hostreply.pending", "Pending Race", base.DEC, race_tracks)

function dissect_message_hostreply(tree, message)
    -- tNet_message_host_reply (14)
    local t = create_message_size_type(tree, message, "Host Reply (UNTESTED)")
    t:add(f_hostreply_started, message(4, 4))
    t:add(f_hostreply_race, message(8, 4))
    t:add(f_hostreply_pending, message(12, 4))
    -- finished
end

function dissect_message_mechanics(tree, message)
    -- tNet_message_mechanics_info (15)
    local t = create_message_size_type(tree, message, "Mechanics")
    -- FIXME
end

function dissect_message_noncar_info(tree, message)
    -- tNet_message_non_car_info (16)
    local t = create_message_size_type(tree, message, "Non-Car Info")
    -- FIXME
end

function dissect_message_timesync(tree, message)
    -- tNet_message_time_sync (17)
    local t = create_message_size_type(tree, message, "Time Sync")
    -- FIXME
end

function dissect_message_confirm(tree, message)
    -- tNet_message_players_confirm (18)
    local t = create_message_size_type(tree, message, "Confirm")
    -- FIXME
end

function dissect_message_disablecar(tree, message)
    -- tNet_message_disable_car (19)
    local t = create_message_size_type(tree, message, "Disable Car")
    -- FIXME
end

function dissect_message_enablecar(tree, message)
    -- tNet_message_enable_car (20)
    local t = create_message_size_type(tree, message, "Enable Car")
    -- FIXME
end

function dissect_message_powerup(tree, message)
    -- tNet_message_powerup (21)
    local t = create_message_size_type(tree, message, "Power-Up")
    -- FIXME
end

function dissect_message_recover(tree, message)
    -- tNet_message_recover (22)
    local t = create_message_size_type(tree, message, "Recover")
    -- FIXME
end

function dissect_message_scores(tree, message)
    -- tNet_message_game_scores (23)
    local t = create_message_size_type(tree, message, "Scores")
    -- FIXME
end

function dissect_message_wasted(tree, message)
    -- tNet_message_wasted (24)
    local t = create_message_size_type(tree, message, "Wasted")
    -- FIXME
end

function dissect_message_pedestrian(tree, message)
    -- tNet_message_pedestrian (25)
    local t = create_message_size_type(tree, message, "Pedestrian")
    -- FIXME
end

function dissect_message_gameplay(tree, message)
    -- tNet_message_gameplay (26)
    local t = create_message_size_type(tree, message, "Gameplay")
    -- FIXME
end

function dissect_message_noncarposition(tree, message)
    -- tNet_message_non_car_position (27)
    local t = create_message_size_type(tree, message, "Non-Car Position")
    -- FIXME
end

function dissect_message_copinfo(tree, message)
    -- tNet_message_cop_info (28)
    local t = create_message_size_type(tree, message, "Cop Info")
    -- FIXME
end

function dissect_message_gamescores(tree, message)
    -- tNet_message_oil_spill (29)
    local t = create_message_size_type(tree, message, "Game Scores")
    -- FIXME
end

function dissect_message_oilspill(tree, message)
    -- tNet_message_oil_spill (30)
    local t = create_message_size_type(tree, message, "Oil Spill")
    -- FIXME
end

function dissect_message_crushpoint(tree, message)
    -- tNet_message_crush_point (31)
    local t = create_message_size_type(tree, message, "Crush Point")
    -- FIXME
end

function dissect_message_none(tree, message)
    -- none (32)
    create_message_size_type(tree, message, "None")
    -- finished
end

dethrace_protocol.fields = {
    f_checksum, f_broadcast_magic, f_broadcast_type, f_terminator,

    f_pd_player_info,

    f_message_magic, f_message_guarantee, f_message_sender, f_message_version, f_message_timestamp, f_message_count, f_message_size,
    f_contents_size, f_contents_type,

    f_details_host_name, f_details_host_id, f_details_num_players, f_details_start_race, f_details_no_races_yet, f_details_status,
    f_details_option_show_players_on_map, f_details_option_show_peds_on_map, f_details_option_enable_text_messages, f_details_option_show_powerups_on_map, f_details_option_powerup_respawn, f_details_option_open_game, f_details_option_starting_money_index, f_details_option_start_type, f_details_option_race_end_target, f_details_option_random_car_choice, f_details_option_race_sequence_type, f_details_option_car_choice,
    f_details_type,

    f_guaranteereply_number,

    f_cardetails_count, f_cardetails_car, f_cardetails_owner,

    f_raceover_reason,

    f_statusreport_status,

    f_startrace_count, f_startrace_racing, f_startrace_next,

    f_headup_text,

    f_hostreply_started, f_hostreply_race, f_hostreply_pending,
}

dethrace_message_dissectors = {
    [ 0] = dissect_message_sendmedetails,
    [ 1] = dissect_message_details,
    [ 2] = dissect_message_join,
    [ 3] = dissect_message_newplayerlist,
    [ 4] = dissect_message_guaranteereply,
    [ 5] = dissect_message_cardetailsreq,
    [ 6] = dissect_message_cardetails,
    [ 7] = dissect_message_leave,
    [ 8] = dissect_message_hosticide,
    [ 9] = dissect_message_raceover,
    [10] = dissect_message_statusreport,
    [11] = dissect_message_startrace,
    [12] = dissect_message_headup,
    [13] = dissect_message_hostquery,
    [14] = dissect_message_hostreply,
    [15] = dissect_message_mechanics,
    [16] = dissect_message_noncar_info,
    [17] = dissect_message_timesync,
    [18] = dissect_message_confirm,
    [19] = dissect_message_disablecar,
    [20] = dissect_message_enablecar,
    [21] = dissect_message_powerup,
    [22] = dissect_message_recover,
    [23] = dissect_message_scores,
    [24] = dissect_message_wasted,
    [25] = dissect_message_pedestrian,
    [26] = dissect_message_gameplay,
    [27] = dissect_message_noncarposition,
    [28] = dissect_message_copinfo,
    [29] = dissect_message_gamescores,
    [30] = dissect_message_oilspill,
    [31] = dissect_message_crushpoint,
    [32] = dissect_message_none,
}

function dethrace_protocol.dissector(buffer, pinfo, tree)
    if buffer:bytes(4, 7) == ByteArray.new("43 57 39 35 4d 53 47") then -- "CW95MSG"
        pinfo.cols.protocol = "DRBC"
        local t = tree:add(dethrace_protocol, buffer(), "DethRace Broadcast Protocol")
        t:add(f_checksum, buffer(0, 4))
        t:add(f_broadcast_magic, buffer(4, 7))
        t:add(f_broadcast_type, buffer(11, 1))
        t:add(f_terminator, buffer(12, 1))
    elseif buffer(4, 4):le_uint() == 0x763a5058 then -- "XP:v"
        pinfo.cols.protocol = "DR";
        do
            local t = tree:add(dethrace_protocol, buffer(0, 28), "DethRace Message Protocol")

            t:add(f_checksum, buffer(0, 4))
            t:add_le(f_message_magic, buffer(4, 4))
            t:add_le(f_message_guarantee, buffer(8, 4))
            t:add_le(f_message_sender, buffer(12, 4))
            t:add_le(f_message_version, buffer(16, 4))
            t:add_le(f_message_timestamp, buffer(20, 4))
            t:add_le(f_message_count, buffer(24, 2))
            t:add_le(f_message_size, buffer(26, 2))
        end
        local message = buffer(28)
        local message_type = message(1, 1):uint()
        dethrace_message_dissectors[message_type](tree, message)
    end
end

-- load the udp.port table
udp_table = DissectorTable.get("udp.port")
-- register our protocol to handle udp port 39456
udp_table:add(DETHRACE_PORT, dethrace_protocol)

