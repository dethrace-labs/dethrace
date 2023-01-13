#ifndef HARNESS_SDL_NETWORKS_H
#define HARNESS_SDL_NETWORKS_H

#include "dr_types.h"

int SDLNetwork_Init(void);

int SDLNetwork_Shutdown(void);

void SDLNetwork_StartProducingJoinList(void);

void SDLNetwork_NetEndJoinList(void);

int SDLNetwork_NetGetNextJoinGame(tNet_game_details* pDetails, int pIndex);

int SDLNetwork_NetSendMessageToAddress(tNet_game_details* pDetails, tNet_message* pMessage, void* pAddress);

//void SDLNetwork_DisposeGameDetails(tNet_game_details* pDetails);

char* SDLNetwork_FormatAddress(void* pAddress);

int SDLNetwork_BroadcastMessage(void);

int SDLNetwork_ReceiveHostResponses(void);

tNet_message* SDLNetwork_PDNetGetNextMessage(tNet_game_details* pDetails, void** pSender_address);

int SDLNetwork_PDNetHostGame(tNet_game_details* pDetails, char* pHost_name, void** pHost_address);

tPlayer_ID SDLNetwork_PDNetExtractPlayerID(tNet_game_details* pDetails);

int SDLNetwork_NetSendto(char* message, int size, void* pAddress);

void SDLNetwork_CopyAddress(void* pDest, const void* pSrc);

#endif
