#ifndef WORLDSTATE_H
#define WORLDSTATE_H

#include "display.h"
#include "player.h"
#include "playState.h"

void WorldStateInit(player_t* mago);
void WorldStateInput(float deltaTime, player_t* mago);
void WorldStateUpdate(float deltaTime, float timePass, player_t* mago, state_t* gameState);
void WorldStateRender(win32BackBuffer_t* backBuffer, player_t* mago);
void WorldStateClear();
void KillEnemy(player_t* mago);

#endif
