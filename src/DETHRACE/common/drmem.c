#include "drmem.h"
#include "brender/brender.h"
#include "errors.h"
#include "harness/trace.h"
#include <stdlib.h>

br_allocator gAllocator = { "Death Race", DRStdlibAllocate, DRStdlibFree, DRStdlibInquire, Claim4ByteAlignment };
int gNon_fatal_allocation_errors = 0;
char* gMem_names[247] = {
    "",
    "BR_MEMORY_SCRATCH",
    "BR_MEMORY_PIXELMAP",
    "BR_MEMORY_PIXELS",
    "BR_MEMORY_VERTICES",
    "BR_MEMORY_FACES",
    "BR_MEMORY_GROUPS",
    "BR_MEMORY_MODEL",
    "BR_MEMORY_MATERIAL",
    "BR_MEMORY_MATERIAL_INDEX",
    "BR_MEMORY_ACTOR",
    "BR_MEMORY_PREPARED_VERTICES",
    "BR_MEMORY_PREPARED_FACES",
    "BR_MEMORY_LIGHT",
    "BR_MEMORY_CAMERA",
    "BR_MEMORY_BOUNDS",
    "BR_MEMORY_CLIP_PLANE",
    "BR_MEMORY_STRING",
    "BR_MEMORY_REGISTRY",
    "BR_MEMORY_TRANSFORM",
    "BR_MEMORY_RESOURCE_CLASS",
    "BR_MEMORY_FILE",
    "BR_MEMORY_ANCHOR",
    "BR_MEMORY_POOL",
    "BR_MEMORY_RENDER_MATERIAL",
    "BR_MEMORY_DATAFILE",
    "BR_MEMORY_IMAGE",
    "BR_MEMORY_IMAGE_ARENA",
    "BR_MEMORY_IMAGE_SECTIONS",
    "BR_MEMORY_IMAGE_NAMES",
    "BR_MEMORY_EXCEPTION_HANDLER",
    "BR_MEMORY_RENDER_DATA",
    "BR_MEMORY_TOKEN",
    "BR_MEMORY_TOKEN_MAP",
    "BR_MEMORY_OBJECT",
    "BR_MEMORY_OBJECT_DATA",
    "BR_MEMORY_DRIVER",
    "BR_MEMORY_LEXER",
    "BR_MEMORY_OBJECT_LIST",
    "BR_MEMORY_OBJECT_LIST_ENTRY",
    "BR_MEMORY_ENABLED_ACTORS",
    "BR_MEMORY_FMT_RESULTS",
    "BR_MEMORY_PREPARED_MODEL",
    "BR_MEMORY_ORDER_TABLE",
    "BR_MEMORY_TOKEN_VALUE",
    "BR_MEMORY_TOKEN_TEMPLATE",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "kMem_intf_pix_copy",
    "kMem_intf_pal_copy",
    "kMem_nodes_array",
    "kMem_sections_array",
    "kMem_key_names",
    "kMem_columns_z",
    "kMem_columns_x",
    "kMem_non_car_list",
    "kMem_simp_level",
    "kMem_crush_data",
    "kMem_crush_neighbours",
    "kMem_temp_fs",
    "kMem_error_pix_copy",
    "kMem_error_pal_copy",
    "kMem_flic_pal",
    "kMem_flic_data",
    "kMem_flic_data_2",
    "kMem_queued_flic",
    "kFlic_panel_pixels",
    "kMem_translations",
    "kMem_translations_text",
    "kMem_cur_pal_pixels",
    "kMem_render_pal_pixels",
    "kMem_scratch_pal_pixels",
    "kMem_shade_table_copy",
    "kMem_rear_screen_pixels",
    "kMem_rolling_letters",
    "kMem_intf_copy_areas",
    "kMem_strip_image",
    "kMem_strip_image_perm",
    "kMem_damage_clauses",
    "kMem_undamaged_vertices",
    "kMem_race_text_chunk",
    "kMem_race_text_str",
    "kMem_oppo_array",
    "kMem_oppo_text_chunk",
    "kMem_oppo_text_str",
    "kMem_br_font",
    "kMem_br_font_wid",
    "kMem_br_font_enc",
    "kMem_br_font_glyphs",
    "kMem_oppo_car_spec",
    "kMem_misc_string",
    "kMem_mac_host_buffer_1",
    "kMem_mac_host_buffer_2",
    "kMem_mac_net_details",
    "kMem_back_pixels",
    "kMem_quit_vfy_pixels",
    "kMem_quit_vfy_pal",
    "kMem_net_min_messages",
    "kMem_net_mid_messages",
    "kMem_net_max_messages",
    "kMem_net_pid_details",
    "kMem_net_car_spec",
    "kMem_dynamic_message",
    "kMem_player_list_join",
    "kMem_player_list_leave",
    "kMem_oppo_new_nodes",
    "kMem_oppo_new_sections",
    "kMem_cop_car_spec",
    "kMem_oppo_bit_per_node",
    "kMem_ped_action_list",
    "kMem_ped_sequences",
    "kMem_ped_array_stain",
    "kMem_ped_array",
    "kMem_ped_instructions",
    "kMem_ped_new_instruc",
    "kMem_pipe_model_geometry",
    "kMem_powerup_array",
    "kMem_powerup_float_parms",
    "kMem_powerup_int_parms",
    "kMem_drugs_palette",
    "kMem_pratcam_flic_array",
    "kMem_pratcam_flic_data",
    "kMem_pratcam_sequence_array",
    "kMem_pratcam_pixelmap",
    "kMem_video_pixels",
    "kMem_funk_prox_array",
    "kMem_new_mat_id",
    "kMem_new_mat_id_2",
    "kMem_new_mat_id_3",
    "kMem_special_volume",
    "kMem_special_screen",
    "kMem_new_special_vol",
    "kMem_saved_game",
    "kMem_new_save_game",
    "kMem_stor_space_pix",
    "kMem_stor_space_tab",
    "kMem_stor_space_mat",
    "kMem_stor_space_mod",
    "kMem_stor_space_save",
    "kMem_funk_spec",
    "kMem_groove_spec",
    "kMem_non_car_spec",
    "kMem_S3_scan_name",
    "kMem_S3_sound_header",
    "kMem_S3_sample",
    "kMem_S3_mac_channel",
    "kMem_S3_mac_path",
    "kMem_S3_sentinel",
    "kMem_S3_outlet",
    "kMem_S3_channel",
    "kMem_S3_descriptor",
    "kMem_S3_reverse_buffer",
    "kMem_S3_source",
    "kMem_S3_DOS_SOS_channel",
    "kMem_S3_PC_DOS_path",
    "kMem_S3_DOS_SOS_patch",
    "kMem_S3_DOS_SOS_song_structure",
    "kMem_S3_DOS_SOS_song_data",
    "kMem_S3_Windows_95_load_WAV_file",
    "kMem_S3_Windows_95_create_temp_buffer_space_to_reverse_sample",
    "kMem_S3_Windows_95_path",
    "kMem_DOS_HMI_file_open",
    "kMem_abuse_text",
    "kMem_action_replay_buffer",
    "kMem_misc",
    NULL,
};
br_resource_class gStainless_classes[118];

// IDA: void __cdecl SetNonFatalAllocationErrors()
void SetNonFatalAllocationErrors() {
    LOG_TRACE("()");

    gNon_fatal_allocation_errors = 1;
}

// IDA: void __cdecl ResetNonFatalAllocationErrors()
void ResetNonFatalAllocationErrors() {
    LOG_TRACE("()");

    gNon_fatal_allocation_errors = 0;
}

// IDA: int __cdecl AllocationErrorsAreFatal()
int AllocationErrorsAreFatal() {
    return gNon_fatal_allocation_errors == 0;
}

// IDA: void __cdecl MAMSInitMem()
void MAMSInitMem() {
    int i;
    FILE* f;
    tPath_name the_path;
    LOG_TRACE("()");
}

// IDA: void __usercall PrintMemoryDump(int pFlags@<EAX>, char *pTitle@<EDX>)
void PrintMemoryDump(int pFlags, char* pTitle) {
    LOG_TRACE("(%d, \"%s\")", pFlags, pTitle);
}

// IDA: void* __cdecl DRStdlibAllocate(br_size_t size, br_uint_8 type)
void* DRStdlibAllocate(br_size_t size, br_uint_8 type) {
    void* p;
    int i;
    char s[256];

    if (size == 0) {
        return NULL;
    }
    p = malloc(size);
    if (p == NULL && !gNon_fatal_allocation_errors) {
        sprintf(s, "%s/%d", gMem_names[type], (int)size);
        FatalError(94, s);
    }
    return p;
}

// IDA: void __cdecl DRStdlibFree(void *mem)
void DRStdlibFree(void* mem) {
    int i;
    free(mem);
}

// IDA: br_size_t __cdecl DRStdlibInquire(br_uint_8 type)
br_size_t DRStdlibInquire(br_uint_8 type) {
    return 0;
}

// IDA: br_uint_32 __cdecl Claim4ByteAlignment(br_uint_8 type)
br_uint_32 Claim4ByteAlignment(br_uint_8 type) {
    return 4;
}

// IDA: void __cdecl InstallDRMemCalls()
void InstallDRMemCalls() {
    BrAllocatorSet(&gAllocator);
}

// IDA: void __usercall MAMSUnlock(void **pPtr@<EAX>)
void MAMSUnlock(void** pPtr) {
    DRStdlibFree(*pPtr);
    *pPtr = NULL;
}

// IDA: void __usercall MAMSLock(void **pPtr@<EAX>)
void MAMSLock(void** pPtr) {
    LOG_TRACE("(%p)", pPtr);
}

// IDA: void __cdecl CreateStainlessClasses()
void CreateStainlessClasses() {
    int i;

    for (i = 129; i < 246; i++) {
        gStainless_classes[i - 129].res_class = i;
        if (!BrResClassAdd(&gStainless_classes[i - 129])) {
            FatalError(94);
        }
    }
}

// IDA: void __cdecl CheckMemory()
void CheckMemory() {
}
