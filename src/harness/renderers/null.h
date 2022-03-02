#include "renderer.h"

void Null_Init() {}
void Null_Shutdown() {}
void Null_BeginFrame(br_actor* camera, br_pixelmap* colour_buffer) {}
void Null_EndFrame() {}
void Null_SetPalette(uint8_t* palette) {}
void Null_RenderFullScreenQuad(uint8_t* src) {}
void Null_RenderModel(br_actor* actor, br_model* model, br_matrix34 model_matrix) {}
void Null_RenderFrameBuffer() {}
void Null_ClearBuffers() {}
void Null_BufferTexture(br_pixelmap* pm) {}
void Null_BufferMaterial(br_material* mat) {}
void Null_BufferModel(br_model* model) {}
void Null_FlushBuffers(br_pixelmap* color_buffer, br_pixelmap* depth_buffer) {}
void Null_GetRenderSize(int* width, int* height) {
    *width = 640;
    *height = 480;
}
void Null_GetWindowSize(int* width, int* height) {
    *width = 640;
    *height = 480;
}
void Null_SetWindowSize(int width, int height) {}
void Null_GetViewportSize(int* x, int* y, int* width, int* height) {
    *x = 0;
    *y = 0;
    *width = 640;
    *height = 480;
}

int Null_NetInit() { return -1; }
int Null_NetShutdown() { return 0; }
void Null_NetStartProducingJoinList() {}
void Null_NetEndJoinList() {}
int Null_NetGetNextjoinGame(tNet_game_details* pDetails, int pIndex) { return 0; }
int Null_NetSendMessageToAddress(tNet_game_details* pDetails, tNet_message* pMessage, void* pAddress) { return 0; }
//void Null_NetDisposeGameDetails(tNet_game_details* pDetails) { }
char* Null_NetFormatAddress(void* pAddress) { return ""; }
int Null_NetBroadcastMessage() { return 0; }
int Null_NetReceiveHostResponses() { return 0; }
tNet_message* Null_PDNetGetNextMessage(tNet_game_details* pDetails, void** pSender_address) { return NULL; }
int Null_PDNetHostGame(tNet_game_details* pDetails, char* pHost_name, void** pHost_address) { return 0; }
tPlayer_ID Null_PDNetExtractPlayerID(tNet_game_details* pDetails) { return 0; }
int Null_NetSendto(char* message, int size, void* pAddress) { return -1; }

tRenderer null_renderer = {
    Null_Init,
    Null_Shutdown,
    Null_BeginFrame,
    Null_EndFrame,
    Null_SetPalette,
    Null_RenderFullScreenQuad,
    Null_RenderModel,
    Null_ClearBuffers,
    Null_BufferTexture,
    Null_BufferMaterial,
    Null_BufferModel,
    Null_FlushBuffers,
    Null_GetRenderSize,
    Null_GetWindowSize,
    Null_SetWindowSize,
    Null_GetViewportSize,

    Null_NetInit,
    Null_NetShutdown,
    Null_NetStartProducingJoinList,
    Null_NetEndJoinList,
    Null_NetGetNextjoinGame,
    Null_NetSendMessageToAddress,
    //Null_NetDisposeGameDetails,
    Null_NetFormatAddress,
    Null_NetBroadcastMessage,
    Null_NetReceiveHostResponses,
    Null_PDNetGetNextMessage,
    Null_PDNetHostGame,
    Null_PDNetExtractPlayerID,
    Null_NetSendto,

};
