#ifndef PAUSESTATE_H
#define PAUSESTATE_H
#include "display.h"

void PauseStateInit();
void PauseStateInput(float deltaTime, float timePass, global_state_t* state, state_t* gameState);
void PauseStateUpdate(float deltaTime, float timePass);
void PauseStateRender(win32BackBuffer_t* backBuffer);
void PauseStateClear();

#endif
