#ifndef COMBATSTATE_H
#define COMBATSTATE_H

#include "display.h"
#include "player.h"
#include "playState.h"

typedef struct{
    rect_t sprite;
    int life;
} lifeBar_t;

void CombatStateInit(player_t* mago);
void CombatStateInput(float deltaTime, float timePass, player_t* mago);
void CombatStateUpdate(float deltaTime, float timePass, player_t* mago, state_t* gameState);
void CombatStateRender(win32BackBuffer_t* backBuffer, player_t* mago);
void CombatStateClear();
void SetEnemyAlive(int* enemyAlive);


#endif
