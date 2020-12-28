#ifndef WORLDSTATE_H
#define WORLDSTATE_H

#include "display.h"

void WorldStateInit();
void WorldStateInput(float deltaTime);
void WorldStateUpdate(float deltaTime, float timePass);
void WorldStateRender(win32BackBuffer_t* backBuffer);
void WorldStateClear();

#endif
