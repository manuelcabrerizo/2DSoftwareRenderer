#ifndef PLAYSTATE_H
#define PLAYSTATE_H
#include "display.h"

enum state_t {
    WORLD,
    COMBAT
};

void PlayStateInit();
void PlayStateInput(float deltaTime, float timePass);
void PlayStateUpdate(float deltaTime, float timePass);
void PlayStateRender(win32BackBuffer_t* backBuffer);
void PlayStateClear();

#endif 
