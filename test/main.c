#include "harness/hooks.h"
#include "tests.h"
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define UNITY_USE_COMMAND_LINE_ARGS 1

#include "framework/unity.h"

#include "CORE/PIXELMAP/pixelmap.h"
#include "CORE/V1DB/actsupt.h"
#include "CORE/V1DB/dbsetup.h"
#include "common/newgame.h"
#include "common/utility.h"

#include "common/drmem.h"
#include "common/globvars.h"
#include "common/grafdata.h"
#include "harness.h"
#include "harness/config.h"

#define debug(format_, ...) fprintf(stderr, format_, __VA_ARGS__)

#define debug(format_, ...) fprintf(stderr, format_, __VA_ARGS__)

extern int _unittest_do_not_exit;

extern void test_utility_suite();
extern void test_loading_suite();
extern void test_controls_suite();
extern void test_input_suite();
extern void test_errors_suite();
extern void test_dossys_suite();
extern void test_init_suite();
extern void test_brlists_suite();
extern void test_fwsetup_suite();
extern void test_resource_suite();
extern void test_actsupt_suite();
extern void test_genclip_suite();
extern void test_datafile_suite();
extern void test_v1dbfile_suite();
extern void test_register_suite();
extern void test_scratch_suite();
extern void test_pattern_suite();
extern void test_pmfile_suite();
extern void test_fixed_suite();
extern void test_matrix23_suite();
extern void test_matrix34_suite();
extern void test_matrix4_suite();
extern void test_quat_suite();
extern void test_graphics_suite();
extern void test_regsupt_suite();
extern void test_vector_suite();
extern void test_powerup_suite();
extern void test_flicplay_suite();

char* root_dir;

void setUp(void) {
    gEncryption_method = 0;
}

void tearDown(void) {
}

static const char* temp_folder;
static char temp_folder_buffer[PATH_MAX + 1];

static void setup_temp_folder() {
#ifdef _WIN32
    DWORD res;
    char tmpBuffer[PATH_MAX + 1];
    res = GetTempPathA(sizeof(tmpBuffer), tmpBuffer);
    if (res == 0) {
        abort();
    }
    sprintf(temp_folder_buffer, "%s\\dethrace_test_%d", tmpBuffer, GetCurrentProcessId());
#else
    sprintf(temp_folder_buffer, "/tmp/dethrace_test_%d", getpid());
#endif
    temp_folder = temp_folder_buffer;
}

void TEST_ASSERT_EQUAL_FILE_CONTENTS_BINARY(const uint8_t* expected, char* filename, int len) {
    FILE* f;
    long filesize;
    int res;
    f = fopen(filename, "rb");
    TEST_ASSERT_NOT_NULL(f);
    res = fseek(f, 0, SEEK_END);
    TEST_ASSERT_NOT_EQUAL(-1, res);
    filesize = ftell(f);
    TEST_ASSERT_NOT_EQUAL(-1, filesize);
    TEST_ASSERT_EQUAL(len, filesize);
    fseek(f, 0, SEEK_SET);
    uint8_t* tmpBuffer = (uint8_t*)malloc(filesize);
    res = fread(tmpBuffer, filesize, 1, f);
    TEST_ASSERT_EQUAL_INT(1, res);
    fclose(f);
    TEST_ASSERT_EQUAL_MEMORY(expected, tmpBuffer, len);
    free(tmpBuffer);
}

void TEST_ASSERT_EQUAL_FILE_TEXT(const char* expected, char* filename) {
    FILE* f;
    char* tmpBuffer;
    long filesize;
    int res;
    int len;

    len = strlen(expected);
    f = fopen(filename, "rb");
    TEST_ASSERT_NOT_NULL(f);
    res = fseek(f, 0, SEEK_END);
    TEST_ASSERT_NOT_EQUAL(-1, res);
    filesize = ftell(f);
    TEST_ASSERT_NOT_EQUAL(-1, filesize);
    fseek(f, 0, SEEK_SET);
    tmpBuffer = (char*)malloc(filesize + 1);
    TEST_ASSERT_NOT_NULL(tmpBuffer);
    res = fread(tmpBuffer, 1, filesize, f);
    tmpBuffer[filesize] = '\0';
    fclose(f);
    TEST_ASSERT_EQUAL_STRING(expected, tmpBuffer);
    TEST_ASSERT_EQUAL_INT(filesize, res);
    TEST_ASSERT_EQUAL_INT(len, filesize);
    TEST_ASSERT_EQUAL_INT(filesize, strlen(tmpBuffer));
    free(tmpBuffer);
}

void setup_global_vars(int argc, char* argv[]) {
    strcpy(gDir_separator, "/");

    root_dir = getenv("DETHRACE_ROOT_DIR");
    if (root_dir != NULL) {
        printf("DETHRACE_ROOT_DIR: %s\n", root_dir);
        chdir(root_dir);
        strncpy(gApplication_path, root_dir, 256);
        strcat(gApplication_path, "/DATA");
    } else {
        printf("WARN: DETHRACE_ROOT_DIR is not defined. Skipping tests which require it\n");
        strcpy(gApplication_path, ".");
    }

    BrV1dbBeginWrapper_Float();
    CreateStainlessClasses();

    gCurrent_graf_data = &gGraf_data[0];
    gGraf_data_index = 0;
    gGraf_spec_index = 0;
    gRender_screen = BrPixelmapAllocate(BR_PMT_INDEX_8, 320, 200, NULL, 0);

    gSelf = BrActorAllocate(BR_ACTOR_NONE, NULL);
    gUniverse_actor = BrActorAllocate(BR_ACTOR_NONE, NULL);
    gNon_track_actor = BrActorAllocate(BR_ACTOR_NONE, NULL);

    strcpy(gBasic_car_names[0], "BLKEAGLE.TXT");

    setup_temp_folder();
    printf("INFO: temp folder is \"%s\"\n", temp_folder);

    _unittest_do_not_exit = 1;
    harness_debug_level = 7;
    harness_game_info.mode = eGame_carmageddon;

    int fake_argc = 2;
    char* fake_argv[2];
    fake_argv[0] = argv[0];
    fake_argv[1] = "--platform=null";
    Harness_Init(&fake_argc, fake_argv);
}

int has_data_directory() {
    return root_dir != NULL;
}

void sleep_s(int sec) {
#ifdef _WIN32
    Sleep(1000 * sec);
#else
    sleep(sec);
#endif
}

void create_temp_file(char buffer[PATH_MAX + 1], const char* prefix) {
#ifdef _WIN32
    DWORD attributes;
    UINT res;
    BOOL success;

    attributes = GetFileAttributesA(temp_folder);
    if ((attributes == INVALID_FILE_ATTRIBUTES) || ((attributes & FILE_ATTRIBUTE_DIRECTORY) == 0)) {
        LOG_TRACE("Temporary folder does not exist => creating");
        success = CreateDirectoryA(temp_folder, NULL);
        if (success == 0) {
            abort();
        }
    }
    res = GetTempFileNameA(temp_folder, prefix, 0, buffer);
    if (res == 0) {
        abort();
    }
    strcat(buffer, prefix);
#else
    int fdres;
    struct stat sb;
    int res;

    res = stat(temp_folder, &sb);
    if (res == -1 || !S_ISDIR(sb.st_mode)) {
        res = mkdir(temp_folder, 0770);
        if (res == -1) {
            fprintf(stderr, "mmkdir(\"%s\") failed: %s\n", temp_folder, strerror(errno));
            abort();
        }
    }
    strcpy(buffer, temp_folder);
    strcat(buffer, "/");
    strcat(buffer, prefix);
    strcat(buffer, "XXXXXX");
    fdres = mkstemp(buffer);
    if (fdres == -1) {
        fprintf(stderr, "mkstemp(\"%s\") failed: %s\n", buffer, strerror(errno));
        abort();
    }
    close(fdres);
#endif
}

int main(int argc, char** argv) {

    UNITY_BEGIN();

    if (UnityParseOptions(argc, argv) != 0) {
        exit(1);
    }

    setup_global_vars(argc, argv);

    printf("Completed setup\n");

    // BRSRC13
    test_matrix23_suite();
    test_matrix34_suite();
    test_matrix4_suite();
    test_vector_suite();
    test_quat_suite();
    test_fixed_suite();

    test_brlists_suite();
    test_pattern_suite();
    test_register_suite();

    test_fwsetup_suite();
    test_scratch_suite();
    test_resource_suite();
    test_actsupt_suite();
    test_genclip_suite();
    test_datafile_suite();

    test_pmfile_suite();
    test_v1dbfile_suite();
    test_regsupt_suite();

    // DETHRACE
    test_utility_suite();
    test_init_suite();
    test_loading_suite();
    test_controls_suite();
    test_input_suite();
    test_errors_suite();
    test_dossys_suite();
    test_graphics_suite();
    test_powerup_suite();
    test_flicplay_suite();

    return UNITY_END();
}
