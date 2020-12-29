#include "playState.h"
#include "worldState.h"
#include "combatState.h"

global_variable state_t gameState;
global_variable player_t mago;
global_variable enemy_t enemy[6];
global_variable enemy_t* actualEnemy;

void PlayStateInit()
{
    WorldStateInit(&mago); 
    gameState = WORLD;
}

void PlayStateInput(float deltaTime, float timePass)
{
    if(gameState == COMBAT)
    { 
        CombatStateInput(deltaTime, timePass, &mago);
    } 

    if(gameState == WORLD)
    {
        WorldStateInput(deltaTime, &mago);
    }
}

void PlayStateUpdate(float deltaTime, float timePass)
{
    if(gameState == WORLD)
    {
        WorldStateUpdate(deltaTime, timePass, &mago, &gameState);
    }
    
    if(gameState == COMBAT)
    {
        CombatStateUpdate(deltaTime, timePass, &mago, &gameState);
    }
}

void PlayStateRender(win32BackBuffer_t* backBuffer)
{
    if(gameState == WORLD)
    {
        WorldStateRender(backBuffer, &mago);
    }
    
    if(gameState == COMBAT)
    {
        CombatStateRender(backBuffer, &mago);
    }
}

void PlayStateClear()
{
    WorldStateClear(); 
    CombatStateClear();
}
