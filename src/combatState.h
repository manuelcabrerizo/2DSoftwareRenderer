#ifndef COMBATSTATE_H
#define COMBATSTATE_H

#include "display.h"

typedef struct{
    rect_t sprite;
    int life;
} lifeBar_t;

void CombatStateInit();
void CombatStateInput(float deltaTime, float timePass);
void CombatStateUpdate(float deltaTime, float timePass);
void CombatStateRender(win32BackBuffer_t* backBuffer);
void CombatStateClear();

#endif
