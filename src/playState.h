#ifndef PLAYSTATE_H
#define PLAYSTATE_H
#include "display.h"

void PlayStateSetTexture();
void PlayStateInit();
void PlayStateInput(float deltaTime, float timePass, global_state_t* state);
void PlayStateUpdate(float deltaTime, float timePass);
void PlayStateRender(win32BackBuffer_t* backBuffer);
void PlayStateClear();

#endif 
