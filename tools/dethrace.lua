-- Basic Dethrace Wireshark dissector
--
-- Installation instructions:
-- 1. Open Wireshark GUI
-- 2. Navigate to "Help -> About Wireshark" menu
-- 3. Click on "Folder" tab and click in the list of paths on "Personal Lua Plugins".
--    If the folder does not yet exist, a dialog will ask you to create it.
-- 4. Paste this lua script in the "Personal Lua Plugins" directory, or create a symlink.
-- 5. When modifying this script, reload all plug-ins with CTRL+SHIFT+L

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

-- FIXME: missing table: index -> car name

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

f_player_status = ProtoField.uint32("dethrace.player_status", "Player Status", base.DEC, player_statuses)

function create_message_size_type(parent, message, name)
    local message_type = message(1, 1):uint()
    local tree = parent:add(dethrace_protocol, message(), string.format("%s (%d)", name, message_type))
    tree:add_le(f_contents_size, message(0, 1))
    tree:add_le(f_contents_type, message(1, 1))
    result = {}
    result["tree"] = tree
    result["size"] = message(0, 1):le_uint()
    result["type"] = message(1, 1):le_uint()
    return result
end

f_pd_player_info = ProtoField.bytes("dethrace.pd_info", "PD Net Info")

function dissect_pd_player_info(tree, data)
    tree:add_le(f_pd_player_info, data(0, 16))
    return 16
end

f_float = ProtoField.float("dethrace.float", "Floating value")

function dissect_br_matrix34(parent, data, name)
    local matrix_tree = parent:add(name, data(0, 48))
    local offset = 0
    for row_i = 0, 3 do
        local row_tree = matrix_tree:add(string.format("m[%d]", row_i), data(offset, 12))

        for col_i = 0, 2 do
            local f = data(offset, 4):le_float()
            row_tree:add_le(f_float, data(offset, 4)):set_text(string.format("m[%d][%d] = %f", row_i, col_i, f))
            offset = offset + 4
        end
    end
    return offset
end

function dissect_reduced_matrix(parent, data, name)
    local matrix_tree = parent:add(name, data(0, 36))
    local offset = 0
    for row_i = 0, 2 do
        local row_tree = matrix_tree:add(string.format("m[%d]", row_i), data(offset, 12))

        for col_i = 0, 2 do
            local f = data(offset, 4):le_float()
            row_tree:add_le(f_float, data(offset, 4)):set_text(string.format("m[%d][%d] = %f", row_i, col_i, f))
            offset = offset + 4
        end
    end
    return offset
end

function dissect_br_vector3(parent, data, name)
    local tree = parent:add(name, data(0, 12))
    local offset = 0

    for i = 0, 2 do
        local f = data(offset, 4):le_float()
        tree:add_le(f_float, data(offset, 4)):set_text(string.format("v[%d] = %f", i, f))
        offset = offset + 4
    end
    return offset
end

function dissect_message_sendmedetails(tree, message)
    -- tNet_message_send_me_details (0)
    local t = create_message_size_type(tree, message, "Send Me Details")["tree"]
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

function dissect_game_details(parent, data)
    -- tNet_game_options
    local tree = parent:add("Options", data(48))
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
    return 48
end

function dissect_message_details(tree, message)
    -- tNet_message_my_details (1)
    local t = create_message_size_type(tree, message, "Details")["tree"]
    local offset = 2

    offset = offset + dissect_pd_player_info(t, message(offset))

    t:add(f_details_host_name, message(offset, 32))
    offset = offset + 4

    t:add_le(f_details_host_id, message(offset, 4))
    offset = offset +  4

    t:add_le(f_details_num_players, message(offset, 4))
    offset = offset +  4

    t:add_le(f_details_start_race, message(offset, 4))
    offset = offset +  4

    t:add_le(f_details_no_races_yet, message(offset, 4))
    offset = offset +  4

    t:add_le(f_details_status, message(offset, 4))
    offset = offset +  4

    offset = offset +  dissect_game_details(t, message(offset))

    t:add_le(f_details_type, message(offset, 4))
    offset = offset +  4
end

f_join_timestamp = ProtoField.uint32("dethrace.join.timestamp", "Timestamp")
f_join_last_heard = ProtoField.uint32("dethrace.join.time_last_heard", "Last heard timestamp")
f_join_reposition_time = ProtoField.uint32("dethrace.join.time_reposition", "Reposition timestamp")
f_join_last_waste_time = ProtoField.uint32("dethrace.join.time_waste", "Last waste message timestamp")
f_join_host = ProtoField.bool("dethrace.join.host", "Host")
f_join_playerid = ProtoField.uint32("dethrace.join.playerid", "Player ID")
f_join_player_name = ProtoField.stringz("dethrace.join.player_name", "Player Name")
f_join_car = ProtoField.uint32("dethrace.join.car", "Car") -- FIXME: use table index -> car name
f_join_grid = ProtoField.uint32("dethrace.join.grid", "Grid Index")
f_join_grid_set = ProtoField.bool("dethrace.join.grid_set", "Grid Position Set")
f_join_opponent_index = ProtoField.uint32("dethrace.join.opponent_index", "Opponent List Index")
f_join_wait_confirm = ProtoField.uint32("dethrace.join.wait_confirm", "Awaiting Confirmation") -- FIXME: find out values
f_join_score = ProtoField.uint32("dethrace.join.score", "Score")
f_join_credits = ProtoField.uint32("dethrace.join.credits", "Credits")
f_join_wasted = ProtoField.bool("dethrace.join.wasted", "Wasted")
f_join_wasteage_attributed = ProtoField.bool("dethrace.join.wasteage_attributed", "Wasteage Attributed") -- FIXME: find out what this is
f_join_name_not_clipped = ProtoField.bool("dethrace.join.name_not_clipped", "Name Not Clipped") -- FIXME: find out what this is
f_join_race_stuff_initialized = ProtoField.bool("dethrace.join.race_stuff_initialized", "Race Stuff Initialized")
f_join_played = ProtoField.bool("dethrace.join.played", "Played")
f_join_won = ProtoField.bool("dethrace.join.won", "Won")
f_join_next_car_index = ProtoField.uint32("dethrace.join.next_car_index", "Next Car Index") -- FIXME: find out what this is
f_join_games_score = ProtoField.uint32("dethrace.join.games_score", "Games Score")
f_join_last_score_index = ProtoField.uint32("dethrace.join.last_score_index", "Last Score Index")
f_join_last_score_index = ProtoField.uint32("dethrace.join.last_score_index", "Last Score Index")
f_join_car_ptr = ProtoField.uint64("dethrace.join.car_ptr", "Car Spec (pointer)")

function dissect_game_player_info(tree, data)
    -- tNet_game_player_info
    local offset = dissect_pd_player_info(tree, data)

    tree:add_le(f_join_timestamp, data(offset, 4))
    offset = offset + 4

    tree:add_le(f_join_last_heard, data(offset, 4))
    offset = offset + 4

    tree:add_le(f_join_reposition_time, data(offset, 4))
    offset = offset + 4

    tree:add_le(f_join_last_waste_time, data(offset, 4))
    offset = offset + 4

    tree:add_le(f_join_host, data(offset, 4))
    offset = offset + 4

    tree:add_le(f_join_playerid, data(offset, 4))
    offset = offset + 4

    tree:add(f_join_player_name, data(offset, 32))
    offset = offset + 32

    tree:add_le(f_player_status, data(offset, 4))
    offset = offset + 4

    tree:add_le(f_join_car, data(offset, 4))
    offset = offset + 4

    tree:add_le(f_join_grid, data(offset, 4))
    offset = offset + 4

    tree:add_le(f_join_grid_set, data(offset, 4))
    offset = offset + 4

    tree:add_le(f_join_opponent_index, data(offset, 4))
    offset = offset + 4

    tree:add_le(f_join_wait_confirm, data(offset, 4))
    offset = offset + 4

    tree:add_le(f_join_score, data(offset, 4))
    offset = offset + 4

    tree:add_le(f_join_credits, data(offset, 4))
    offset = offset + 4

    tree:add_le(f_join_wasted, data(offset, 4))
    offset = offset + 4

    tree:add_le(f_join_wasteage_attributed, data(offset, 4))
    offset = offset + 4

    tree:add_le(f_join_name_not_clipped, data(offset, 4))
    offset = offset + 4

    tree:add_le(f_join_race_stuff_initialized, data(offset, 4))
    offset = offset + 4

    tree:add_le(f_join_played, data(offset, 4))
    offset = offset + 4

    tree:add_le(f_join_won, data(offset, 4))
    offset = offset + 4

    tree:add_le(f_join_next_car_index, data(offset, 4))
    offset = offset + 4

    tree:add_le(f_join_games_score, data(offset, 4))
    offset = offset + 4

    tree:add_le(f_join_last_score_index, data(offset, 4))
    offset = offset + 4

    offset = offset + dissect_br_matrix34(tree, data(offset), "Initial Position")

    assert(offset == 0xbc, "offset of car spec ptr")

    tree:add_le(f_join_car_ptr, data(offset, 4))

    tree:set_len(offset)
end

function dissect_message_join(tree, message)
    -- tNet_message_join (2)
    local t = create_message_size_type(tree, message, "Join")["tree"]
    local offset = 4

    dissect_game_player_info(t:add("Player Info", message(offset)), message(offset))
end

newerplayerlist_count_magics = {
    [-1] = "-1 (Selected Car Unavailable)",
    [0] = "0 (Game Already Started)",
}

f_newplayerlist_count = ProtoField.int32("dethrace.newplayerlist.count", "Number Of Players")--, base.DEC, newerplayerlist_count_magics)
f_newplayerlist_index = ProtoField.int32("dethrace.newplayerlist.index", "This Index")
f_newplayerlist_batch_number = ProtoField.int32("dethrace.newplayerlist.batch_number", "Batch Number")

function dissect_message_newplayerlist(tree, message)
    -- tNet_message_new_player_list (3)
    local t = create_message_size_type(tree, message, "New Player List")["tree"]
    local offset = 4

    local t_count = t:add_le(f_newplayerlist_count, message(offset, 4))
    if message(offset, 4):int() == 0 then
        t_count:set_text("Number Of Players: Game Already Started (0)")
    elseif message(offset, 4):int() == -1 then
        t_count:set_text("Number Of Players: Car Unavailable (-1)")
    end
    offset = offset + 4

    t:add_le(f_newplayerlist_index, message(offset, 4))
    offset = offset + 4

    t:add_le(f_newplayerlist_batch_number, message(offset, 4))
    offset = offset + 4

    dissect_game_player_info(t:add("Player", message(offset)), message(offset))
    -- finished
end

f_guaranteereply_number = ProtoField.uint32("dethrace.guaranteereply.number", "Guarantee Number")

function dissect_message_guaranteereply(tree, message)
    -- tNet_message_guarantee_reply (4)
    local t = create_message_size_type(tree, message, "Guarantee Reply")["tree"]
    t:add_le(f_guaranteereply_number, message(4, 4))
    -- FIXME: add reference to packet with this reply number
    -- finished
end

function dissect_message_cardetailsreq(tree, message)
    -- tNet_message_car_details_req (5)
    local t = create_message_size_type(tree, message, "Car Details Req")["tree"]
    -- finished
end

f_cardetails_count = ProtoField.uint32("dethrace.cardetails.count", "Car Count")
f_cardetails_car = ProtoField.uint32("dethrace.cardetails.car_index", "Car") -- FIXME: use table index -> car name
f_cardetails_owner = ProtoField.stringz("dethrace.cardetails.owner", "Owner")

function dissect_message_cardetails(tree, message)
    -- tNet_message_car_details (6)
    local t = create_message_size_type(tree, message, "Car Details")["tree"]
    local offset = 4

    t:add_le(f_cardetails_count, message(4, 4))
    offset = offset + 4

    for i = 0, 5 do
        local car_detail = message(offset, 20)
        local subtree = t:add(car_detail, string.format("Car[%d]", i))
        subtree:add_le(f_cardetails_car, car_detail(0, 4))
        subtree:add(f_cardetails_owner, car_detail(4, 16))

        offset = offset + 20
    end
    -- finished
end

function dissect_message_leave(tree, message)
    -- tNet_message_leave (7)
    local t = create_message_size_type(tree, message, "Leave")["tree"]
    -- finished
end

function dissect_message_hosticide(tree, message)
    -- tNet_message_host_pissing_off (8)
    local t = create_message_size_type(tree, message, "Hosticide")["tree"]
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
    local t = create_message_size_type(tree, message, "Race Over (UNTESTED)")["tree"]
    t:add_le(f_raceover_reason, message(4, 4))
    -- finished
end

function dissect_message_statusreport(tree, message)
    -- tNet_message_status_report (10)
    local t = create_message_size_type(tree, message, "Status Report")["tree"]
    t:add_le(f_player_status, message(4, 4))
    -- finished
end

f_startrace_count = ProtoField.uint32("dethrace.startrace.count", "Car Count", base.DEC)
f_startrace_racing = ProtoField.bool("dethrace.startrace.racing", "Racing")
f_startrace_next = ProtoField.uint32("dethrace.startrace.next", "Next Race", base.DEC, race_tracks)
f_startrace_grid_index = ProtoField.uint32("dethrace.startrace.grid.index", "Index", base.DEC)
f_startrace_grid_next_car_index = ProtoField.uint32("dethrace.startrace.grid.next_car_index", "Next Car Index", base.DEC)

function dissect_message_startrace(tree, message)
    -- tNet_message_start_race (11)
    local t = create_message_size_type(tree, message, "Start Race")["tree"]
    local offset = 4

    t:add_le(f_startrace_count, message(offset, 4))
    offset = offset + 4

    t:add_le(f_startrace_racing, message(offset, 4))
    offset = offset + 4

    t:add_le(f_startrace_next, message(offset, 4))
    offset = offset + 4

    for i = 0, 5 do
        local start = offset
        local subtree = t:add(message(offset), string.format("Grid Car[%d]", i))
        subtree:add_le(f_startrace_grid_index, message(offset, 4))
        offset = offset + 4

        subtree:add_le(f_startrace_grid_next_car_index, message(offset, 4))
        offset = offset + 4

        offset = offset + dissect_br_matrix34(subtree, message(offset), "Matrix")
        subtree:set_len(offset - start)
    end
    -- FIXME
end

f_headup_text = ProtoField.stringz("dethrace.headup.message", "Message")

function dissect_message_headup(tree, message)
    -- tNet_message_headup (12)
    local t = create_message_size_type(tree, message, "Head-Up (UNTESTED)")["tree"]
    t:add(f_headup_text, message(4, 128))
    -- finished
end

function dissect_message_hostquery(tree, message)
    -- tNet_message_host_query (13)
    local t = create_message_size_type(tree, message, "Host Query")["tree"]
    -- finished
end

f_hostreply_started = ProtoField.bool("dethrace.hostreply.started", "Race Started")
f_hostreply_race = ProtoField.uint32("dethrace.hostreply.race", "Race", base.DEC, race_tracks)
f_hostreply_pending = ProtoField.uint32("dethrace.hostreply.pending", "Pending Race", base.DEC, race_tracks)

function dissect_message_hostreply(tree, message)
    -- tNet_message_host_reply (14)
    local t = create_message_size_type(tree, message, "Host Reply (UNTESTED)")["tree"]
    t:add(f_hostreply_started, message(4, 4))
    t:add(f_hostreply_race, message(8, 4))
    t:add(f_hostreply_pending, message(12, 4))
    -- finished
end

f_mechanics_id = ProtoField.uint32("dethrace.mechanics.id", "ID")
f_mechanics_time = ProtoField.uint32("dethrace.mechanics.time", "Time")
f_mechanics_d = ProtoField.uint8("dethrace.mechanics.d", "D")
f_mechanics_carcontrols = ProtoField.uint32("dethrace.mechanics.carcontrols", "Car Controls") -- FIXME: bitmask
f_mechanics_coll_time = ProtoField.uint32("dethrace.mechanics.coll_time", "Car Control Collision Time")
f_mechanics_curvature = ProtoField.int16("dethrace.mechanics.curvature", "Curvature")
f_mechanics_revs = ProtoField.uint16("dethrace.mechanics.revs", "Revs")
f_mechanics_front = ProtoField.float("dethrace.mechanics.front", "Front")
f_mechanics_back = ProtoField.float("dethrace.mechanics.back", "Back")
f_mechanics_repair_time = ProtoField.uint32("dethrace.mechanics.repair_time", "Repair Time")
f_mechanics_powerups = ProtoField.int16("dethrace.mechanics.powerups", "Powerups")
f_mechanics_damage = ProtoField.uint8("dethrace.mechanics.damage", "Damage")

function dissect_message_mechanics(tree, message)
    -- tNet_message_mechanics_info (15)
    local t = create_message_size_type(tree, message, "Mechanics")["tree"]
    local offset = 4

    t:add_le(f_mechanics_id, message(offset, 4))
    offset = offset + 4

    t:add_le(f_mechanics_time, message(offset, 4))
    offset = offset + 4

    offset = offset + dissect_reduced_matrix(t, message(offset), "Matrix")

    offset = offset + dissect_br_vector3(t, message(offset), "Velocity")

    offset = offset + dissect_br_vector3(t, message(offset), "Omega")

    do
        local sub = t:add("d", message(offset, 4))
        for i = 0, 3 do
            local item = sub:add_le(f_mechanics_d, message(offset, 1))
            item:set_text(string.format("d[%d] = %d", i, message(offset, 1):le_uint()))
            offset = offset + 1
        end
    end

    t:add_le(f_mechanics_carcontrols, message(offset, 4))
    offset = offset + 4

    t:add_le(f_mechanics_coll_time, message(offset, 4))
    offset = offset + 4

    t:add_le(f_mechanics_curvature, message(offset, 2))
    offset = offset + 2

    t:add_le(f_mechanics_revs, message(offset, 2))
    offset = offset + 2

    t:add_le(f_mechanics_front, message(offset, 4))
    offset = offset + 4

    t:add_le(f_mechanics_back, message(offset, 4))
    offset = offset + 4

    t:add_le(f_mechanics_repair_time, message(offset, 4))
    offset = offset + 4

    t:add_le(f_mechanics_powerups, message(offset, 2))
    offset = offset + 2

    do
        local sub = t:add("Damage", message(offset, 4))
        -- FIXME: use names of damage system (in addition to index)
        for i = 0, 11 do
            local item = sub:add_le(f_mechanics_damage, message(offset, 1))
            item:set_text(string.format("damage[%d] = %d", i, message(offset, 1):le_uint()))
            offset = offset + 1
        end
    end

    t:add_le(f_mechanics_powerups, message(offset, 2))
    offset = offset + 2

    -- omit 2 bytes of padding
    offset = offset + 2

    if offset < message:len() then
        do
            local sub = t:add("Wheel Damage Offset", message(offset, 4))
            for i = 0, 3 do
                local item = sub:add_le(f_float, message(offset, 4))
                item:set_text(string.format("offset[%d] = %d", i, message(offset, 4):le_float()))
                offset = offset + 4
            end
        end
    end
end

function dissect_message_noncar_info(tree, message)
    -- tNet_message_non_car_info (16)
    local t = create_message_size_type(tree, message, "Non-Car Info (INCOMPLETE)")["tree"]
    -- FIXME
end

f_timesync_time = ProtoField.uint32("dethrace.timesync.start_time", "Race Start Time")

function dissect_message_timesync(tree, message)
    -- tNet_message_time_sync (17)
    local t = create_message_size_type(tree, message, "Time Sync")["tree"]
    local offset = 4

    t:add_le(f_timesync_time, message(offset, 4))
    offset = offset + 4
end

function dissect_message_confirm(tree, message)
    -- tNet_message_players_confirm (18)
    local t = create_message_size_type(tree, message, "Confirm (INCOMPLETE)")["tree"]
    -- FIXME
end

function dissect_message_disablecar(tree, message)
    -- tNet_message_disable_car (19)
    local t = create_message_size_type(tree, message, "Disable Car (INCOMPLETE)")["tree"]
    -- FIXME
end

function dissect_message_enablecar(tree, message)
    -- tNet_message_enable_car (20)
    local t = create_message_size_type(tree, message, "Enable Car (INCOMPLETE)")["tree"]
    -- FIXME
end

function dissect_message_powerup(tree, message)
    -- tNet_message_powerup (21)
    local t = create_message_size_type(tree, message, "Power-Up (INCOMPLETE)")["tree"]
    -- FIXME
end

function dissect_message_recover(tree, message)
    -- tNet_message_recover (22)
    local t = create_message_size_type(tree, message, "Recover (INCOMPLETE)")["tree"]
    -- FIXME
end

f_scores_played = ProtoField.bool("dethrace.score.played", "Played")
f_scores_won = ProtoField.bool("dethrace.scored.won", "Won")
f_scores_score = ProtoField.int32("dethrace.scored.score", "Score")

function dissect_message_scores(tree, message)
    -- tNet_message_game_scores (23)
    local header = create_message_size_type(tree, message, "Scores")
    local t = header["tree"]
    local offset = 4

    for i = 0, 5 do
        local subtree = t:add(string.format("Scores[%d]", i), message(offset, 12))

        subtree:add_le(f_scores_played, message(offset, 4))
        offset = offset + 4

        subtree:add_le(f_scores_won, message(offset, 4))
        offset = offset + 4

        subtree:add_le(f_scores_score, message(offset, 4))
        offset = offset + 4
    end
end

function dissect_message_wasted(tree, message)
    -- tNet_message_wasted (24)
    local t = create_message_size_type(tree, message, "Wasted (INCOMPLETE)")["tree"]
    -- FIXME
end

f_ped_instruction = ProtoField.int8("dethrace.ped.instruction", "Action Instruction")
f_ped_flags = ProtoField.int8("dethrace.ped.flags", "Flags")
f_ped_index = ProtoField.int16("dethrace.ped.index", "Index")

function dissect_message_pedestrian(tree, message)
    -- tNet_message_pedestrian (25)
    local t = create_message_size_type(tree, message, "Pedestrian (INCOMPLETE)")["tree"]
    local offset = 2

    t:add_le(f_ped_instruction, message(offset, 1))
    offset = offset + 1

    t:add_le(f_ped_flags, message(offset, 1))
    offset = offset + 1

    t:add_le(f_ped_index, message(offset, 2))
    offset = offset + 2

    -- padding
    offset = offset + 2

    offset = offset + dissect_br_vector3(t, message(offset, 12), "Position")

    t:add_le(f_float, message(offset, 4)):set_text(string.format("Speed: %f", message(offset, 4):le_float()))
    offset = offset + 4

    --offset = offset + dissect_br_vector3(t, message(offset, 12), "to_pos")
    -- FIXME
end

function dissect_message_gameplay(tree, message)
    -- tNet_message_gameplay (26)
    local t = create_message_size_type(tree, message, "Gameplay (INCOMPLETE)")["tree"]
    -- FIXME
end

function dissect_message_noncarposition(tree, message)
    -- tNet_message_non_car_position (27)
    local t = create_message_size_type(tree, message, "Non-Car Position (INCOMPLETE)")["tree"]
    -- FIXME
end

function dissect_message_copinfo(tree, message)
    -- tNet_message_cop_info (28)
    local t = create_message_size_type(tree, message, "Cop Info (INCOMPLETE)")["tree"]
    -- FIXME
end

function dissect_message_gamescores(tree, message)
    -- tNet_message_oil_spill (29)
    local t = create_message_size_type(tree, message, "Game Scores (INCOMPLETE)")["tree"]
    -- FIXME
end

function dissect_message_oilspill(tree, message)
    -- tNet_message_oil_spill (30)
    local t = create_message_size_type(tree, message, "Oil Spill (INCOMPLETE)")["tree"]
    -- FIXME
end

function dissect_message_crushpoint(tree, message)
    -- tNet_message_crush_point (31)
    local t = create_message_size_type(tree, message, "Crush Point (INCOMPLETE)")["tree"]
    -- FIXME
end

function dissect_message_none(tree, message)
    -- none (32)
    local t = create_message_size_type(tree, message, "None")["tree"]
    -- finished
end

dethrace_protocol.fields = {
    f_checksum, f_broadcast_magic, f_broadcast_type, f_terminator,
    f_float,

    f_pd_player_info,

    f_message_magic, f_message_guarantee, f_message_sender, f_message_version, f_message_timestamp, f_message_count, f_message_size,
    f_contents_size, f_contents_type,

    -- 2 (join)
    f_join_timestamp, f_join_last_heard, f_join_reposition_time, f_join_last_waste_time,
    f_join_host, f_join_playerid, f_join_player_name, f_join_car, f_join_grid, f_join_grid_set,
    f_join_opponent_index, f_join_wait_confirm, f_join_score, f_join_credits, f_join_wasted,
    f_join_wasteage_attributed, f_join_name_not_clipped, f_join_race_stuff_initialized, f_join_played,
    f_join_won, f_join_next_car_index, f_join_games_score,  f_join_last_score_index, f_join_car_ptr,

    -- 3 (newplayerlist)
    f_newplayerlist_count, f_newplayerlist_index, f_newplayerlist_batch_number,

    f_details_host_name, f_details_host_id, f_details_num_players, f_details_start_race, f_details_no_races_yet, f_details_status,
    f_details_option_show_players_on_map, f_details_option_show_peds_on_map, f_details_option_enable_text_messages, f_details_option_show_powerups_on_map, f_details_option_powerup_respawn, f_details_option_open_game, f_details_option_starting_money_index, f_details_option_start_type, f_details_option_race_end_target, f_details_option_random_car_choice, f_details_option_race_sequence_type, f_details_option_car_choice,
    f_details_type,

    f_guaranteereply_number,

    f_cardetails_count, f_cardetails_car, f_cardetails_owner,

    f_raceover_reason,

    f_statusreport_status,

    f_startrace_count, f_startrace_racing, f_startrace_next, f_startrace_grid_index, f_startrace_grid_next_car_index,

    f_headup_text,

    f_hostreply_started, f_hostreply_race, f_hostreply_pending,
    f_player_status,

    f_mechanics_id, f_mechanics_time, f_mechanics_d, f_mechanics_carcontrols, f_mechanics_coll_time,
    f_mechanics_curvature, f_mechanics_revs, f_mechanics_front, f_mechanics_back, f_mechanics_repair_time,
    f_mechanics_powerups, f_mechanics_damage,

    f_timesync_time,

    f_scores_played, f_scores_won, f_scores_score,

    f_ped_instruction, f_ped_flags, f_ped_index,
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
        local t = tree:add(dethrace_protocol, buffer(), "DethRace Broadcast Protocol")
        pinfo.cols.protocol = "DRBC"
        pinfo.cols.info = string.format("Broadcast: %s", broadcast_types[buffer(11, 1):uint()])
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
        local sender_type_str
        if (pinfo.src_port == DETHRACE_PORT) then
            sender_type_str = "HOST"
        else
            sender_type_str = "CLIENT"
        end
        local message = buffer(28)
        local message_type = message(1, 1):uint()
        pinfo.cols.info = string.format("[%s] %s (%d)", sender_type_str, content_types[message_type], message_type)
        dethrace_message_dissectors[message_type](tree, message)
    end
end

-- load the udp.port table
udp_table = DissectorTable.get("udp.port")
-- register our protocol to handle udp port 39456
udp_table:add(DETHRACE_PORT, dethrace_protocol)

