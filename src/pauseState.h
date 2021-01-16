#ifndef PAUSESTATE_H
#define PAUSESTATE_H
#include "display.h"


void PauseStateSetTexture();
void PauseStateInit();
void PauseStateInput(float deltaTime, float timePass, global_state_t* state, state_t* gameState, state_t lastState);
void PauseStateUpdate(float deltaTime, float timePass);
void PauseStateRender(win32BackBuffer_t* backBuffer);
void PauseStateClear();

#endif
