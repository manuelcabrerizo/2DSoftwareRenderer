#ifndef MENUSTATE_H
#define MENUSTATE_H
#include "display.h"

void MenuStateInit();
void MenuStateInput(float deltaTime, float timePass, global_state_t* state);
void MenuStateUpdate(float deltaTime, float timePass);
void MenuStateRender(win32BackBuffer_t* backBuffer);
void MenuStateClear();
void MenuStateReset();

#endif
