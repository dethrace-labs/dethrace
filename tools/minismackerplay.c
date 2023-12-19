#define SDL_MAIN_HANDLED
#include "SDL.h"
#include <miniaudio/miniaudio.h>
#include <smacker.h>

#include <stdio.h>

typedef struct {
    ma_paged_audio_buffer paged_audio_buffer;
    ma_paged_audio_buffer_data paged_audio_buffer_data;
    int frame_size_in_bytes;
} smacker_data_source;

static ma_engine engine;

void PlaySmackerFile(SDL_Window *window, SDL_Renderer *renderer, const char *path) {
    unsigned int i, j;
    smk s;

    unsigned long w, h, f;
    double usf;

    SDL_Event event;
    SDL_Colour palette[256];
    Uint32 real_format;
    int real_access;
    int real_w;
    int real_h;
    void* pixels;
    int pixel_pitch;

    Uint64 counter_frametime;
    Uint64 counter_deadline_next_frame;

    unsigned char track_mask;
    unsigned char channels_smk[7];
    unsigned char bitdepth_smk[7];
    unsigned long samplerate_smk[7];

    ma_result result_ma;
    ma_format audioformat_ma;
    int need_resampler;
    ma_sound sound;

    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Trying to open smack file '%s'", path);
    s = smk_open_file(path, SMK_MODE_MEMORY);
    if (s == NULL) {
        SDL_Log("smk_open_file failed");
        return;
    }
    smk_info_all(s, NULL, &f, &usf);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "frame count=%"SDL_PRIu64", frame time=%gus", (Uint64)f, usf);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "duration=%gs", (double)f * usf / 1000000.);

    counter_frametime = (Uint64)((double)SDL_GetPerformanceFrequency() * usf / 1000000);

    smk_info_video(s, &w, &h, NULL);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "size=%"SDL_PRIu64"x%"SDL_PRIu64"", (Uint64)w, (Uint64)h);

    smacker_data_source data_source;
    SDL_SetWindowTitle(window, path);

    smk_info_audio(s, &track_mask, channels_smk, bitdepth_smk, samplerate_smk);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "tracks mask = 0x%x", track_mask);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "#channels = %d %d %d %d %d %d %d", channels_smk[0], channels_smk[1], channels_smk[2], channels_smk[3], channels_smk[4], channels_smk[5], channels_smk[6]);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "sample bitsizes = %d %d %d %d %d %d %d", bitdepth_smk[0], bitdepth_smk[1], bitdepth_smk[2], bitdepth_smk[3], bitdepth_smk[4], bitdepth_smk[5], bitdepth_smk[6]);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "sample rates = %"SDL_PRIu64" %"SDL_PRIu64" %"SDL_PRIu64" %"SDL_PRIu64" %"SDL_PRIu64" %"SDL_PRIu64" %"SDL_PRIu64"", (Uint64)samplerate_smk[0], (Uint64)samplerate_smk[1], (Uint64)samplerate_smk[2], (Uint64)samplerate_smk[3], (Uint64)samplerate_smk[4], (Uint64)samplerate_smk[5], (Uint64)samplerate_smk[6]);

    switch (bitdepth_smk[0]) {
        case 8:
            audioformat_ma = ma_format_u8;
            data_source.frame_size_in_bytes = 1 * channels_smk[0];
            break;
        case 16:
            audioformat_ma = ma_format_s16;
            data_source.frame_size_in_bytes = 2 * channels_smk[0];
            break;
        case 24:
            audioformat_ma = ma_format_s24;
            data_source.frame_size_in_bytes = 3 * channels_smk[0];
            break;
        case 32:
            audioformat_ma = ma_format_s32;
            data_source.frame_size_in_bytes = 4 * channels_smk[0];
            break;
        default:
            audioformat_ma = ma_format_unknown;
            SDL_Log("Smacker audio stream has invalid bit depth: %d", bitdepth_smk[0]);
            break;
    }
    result_ma = ma_paged_audio_buffer_data_init(audioformat_ma, channels_smk[0], &data_source.paged_audio_buffer_data);
    if (result_ma != MA_SUCCESS) {
        SDL_Log("Failed to create paged audio buffer data (%s)", ma_result_description(result_ma));
    }

    ma_paged_audio_buffer_config paged_audio_buffer_config = ma_paged_audio_buffer_config_init(&data_source.paged_audio_buffer_data);
    result_ma = ma_paged_audio_buffer_init(&paged_audio_buffer_config, &data_source.paged_audio_buffer);
    if (result_ma != MA_SUCCESS) {
        SDL_Log("Failed to create paged audio buffer for smacker audio stream (%s)", ma_result_description(result_ma));
    }
    ma_sound_config soundConfig = ma_sound_config_init();
    soundConfig.pDataSource = &data_source;
    soundConfig.flags = MA_SOUND_FLAG_NO_PITCH | MA_SOUND_FLAG_NO_SPATIALIZATION;
    result_ma = ma_sound_init_ex(&engine, &soundConfig, &sound);
    if (result_ma != MA_SUCCESS) {
        SDL_Log("Failed to create sound from data source (%s)", ma_result_description(result_ma));
    }

    need_resampler = ma_engine_get_sample_rate(&engine) != samplerate_smk[0];

    ma_data_converter data_converter;
    if (need_resampler) {
        ma_data_converter_config dataConverterConfig = ma_data_converter_config_init(audioformat_ma, audioformat_ma, channels_smk[0], channels_smk[0], (ma_uint32)samplerate_smk[0],
                                                                                     ma_engine_get_sample_rate(&engine));
        result_ma = ma_data_converter_init(&dataConverterConfig, NULL, &data_converter);
        if (result_ma != MA_SUCCESS) {
            SDL_Log("Failed to create data converter (%s)", ma_result_description(result_ma));
        }
    }

    smk_enable_video(s, 1);
    smk_enable_audio(s, 0, 1);

    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, (int)w, (int)h);
    if (texture == NULL) {
        SDL_Log("SDL_CreateTexture returned NULL (%s)", SDL_GetError());
        return;
    }

    SDL_QueryTexture(texture, &real_format, &real_access, &real_w, &real_h);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "format: %s (requested=%s)", SDL_GetPixelFormatName(real_format), SDL_GetPixelFormatName(SDL_PIXELFORMAT_RGB24));
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "access: %d (requested=%d)", real_access, SDL_TEXTUREACCESS_STREAMING);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "size: %dx%d (requested=%"SDL_PRIu64"x%"SDL_PRIu64")", real_w, real_h, (Uint64)w, (Uint64)h);

    smk_first(s);
    counter_deadline_next_frame = SDL_GetPerformanceCounter();
    int audio_started = 0;
    do {
        const unsigned char* raw_palette = smk_get_palette(s);
        for (i = 0; i < 256; i++) {
            palette[i].r = raw_palette[3 * i + 0];
            palette[i].g = raw_palette[3 * i + 1];
            palette[i].b = raw_palette[3 * i + 2];
            palette[i].a = 0;
        }
        const unsigned char *audio_data = smk_get_audio(s, 0);
        unsigned long audio_data_size = smk_get_audio_size(s, 0);
        const unsigned char* frame = smk_get_video(s);

        if (audio_data != NULL) {
            if (need_resampler) {
                ma_uint64 framesIn = audio_data_size / (ma_uint64)data_source.frame_size_in_bytes;
                ma_uint64 framesOut = framesIn * ma_engine_get_sample_rate(&engine) / samplerate_smk[0];

                ma_paged_audio_buffer_page *newPage;
                result_ma = ma_paged_audio_buffer_data_allocate_page(&data_source.paged_audio_buffer_data, framesOut, NULL, NULL, &newPage);
                if (result_ma != MA_SUCCESS) {
                    SDL_Log("ma_paged_audio_buffer_data_allocate_page failed (%s)", ma_result_description(result_ma));
                }

                ma_data_converter_process_pcm_frames(&data_converter,
                                                audio_data,
                                                &framesIn,
                                                newPage->pAudioData,
                                                &framesOut);
                result_ma = ma_paged_audio_buffer_data_append_page(&data_source.paged_audio_buffer_data, newPage);
                if (result_ma != MA_SUCCESS) {
                    SDL_Log("ma_paged_audio_buffer_data_append_page failed (%s)", ma_result_description(result_ma));
                }
            } else {
                ma_paged_audio_buffer_data_allocate_and_append_page(
                        &data_source.paged_audio_buffer_data,
                        (ma_uint32)(audio_data_size / (ma_uint64)data_source.frame_size_in_bytes),
                        audio_data,
                        NULL);

            }
        }

        SDL_LockTexture(texture, NULL, &pixels, &pixel_pitch);
        Uint8* current_row = pixels;
        for (i = 0; i < h; i++) {
            Uint8* pixel_ptr = current_row;
            for (j = 0; j < w; j++) {
                pixel_ptr[0] = palette[frame[w * i + j]].r;
                pixel_ptr[1] = palette[frame[w * i + j]].g;
                pixel_ptr[2] = palette[frame[w * i + j]].b;
                pixel_ptr += 3;
            }
            current_row += pixel_pitch;
        }
        SDL_UnlockTexture(texture);
        while (SDL_GetPerformanceCounter() < counter_deadline_next_frame) {
            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                    case SDL_QUIT:
                        exit(0);
                        break;
                }
            }
            SDL_Delay(1);
        }
        counter_deadline_next_frame += counter_frametime;

        SDL_RenderCopy(renderer, texture, NULL, NULL);
        if (!audio_started) {
            ma_sound_start(&sound);
            audio_started = 1;
        }
        SDL_RenderPresent(renderer);
    } while (smk_next(s) == SMK_MORE);
    if (need_resampler) {
        ma_data_converter_uninit(&data_converter, NULL);
    }
    while (ma_sound_is_playing(&sound)) {
        SDL_Delay(1);
    }

    ma_sound_stop(&sound);
    ma_sound_uninit(&sound);
    ma_paged_audio_buffer_uninit(&data_source.paged_audio_buffer);
    ma_paged_audio_buffer_data_uninit(&data_source.paged_audio_buffer_data, NULL);
    smk_close(s);
}

static void PrintUsage(int argc, char *argv[]) {
    printf("Usage: %s [--debug] FILE [FILE ...]\n", argv[0]);
    printf("\n");
    printf("Required options:\n");
    printf("\n");
    printf("  FILE      Path to a smack video file.\n");
    printf("\n");
    printf("Optional options:\n");
    printf("\n");
    printf("  --debug   Print debug information.\n");
}

int main(int argc, char *argv[]) {
    int i;

    ma_result result_ma;
    ma_engine_config engineConfig;

    SDL_Window *window;
    SDL_Renderer *renderer;

    int start_i = 1;
    while (start_i < argc) {
        if (strcmp(argv[start_i], "--debug") == 0) {
            SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG);
        } else {
            break;
        }
        start_i++;
    }

    if (start_i >= argc) {
        PrintUsage(argc, argv);
        return 1;
    }


    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL_Init failed (%s)", SDL_GetError());
        return 1;
    }

    engineConfig = ma_engine_config_init();
    result_ma = ma_engine_init(&engineConfig, &engine);
    if (result_ma != MA_SUCCESS) {
        SDL_Log("ma_engine_init failed (%s)", ma_result_description(result_ma));
        return 1;
    }

    if (SDL_CreateWindowAndRenderer(640, 480, SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN, &window, &renderer) < 0) {
        SDL_Log("SDL_CreateWindowAndRenderer failed (%s)", SDL_GetError());
        return 1;
    }

    SDL_ShowWindow(window);
    for (i = start_i; i < argc; i++) {
        PlaySmackerFile(window, renderer, argv[i]);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    ma_engine_uninit(&engine);
    SDL_Quit();
    return 0;
}
