#include "playState.h"
#include "worldState.h"
#include "combatState.h"

global_variable state_t gameState;

void PlayStateInit()
{
    WorldStateInit();
    CombatStateInit();
    gameState = WORLD;
}

void PlayStateInput(float deltaTime, float timePass)
{
    if(gameState == COMBAT)
    { 
        CombatStateInput(deltaTime, timePass);
    } 

    if(gameState == WORLD)
    {
        WorldStateInput(deltaTime);
    }

    if(KeyUp(0x20))
    {
        if(gameState == WORLD)
        {   
            gameState = COMBAT;
        }
        else
        { 
            gameState = WORLD;
        }       
    }
}

void PlayStateUpdate(float deltaTime, float timePass)
{
    if(gameState == WORLD)
    {
        WorldStateUpdate(deltaTime, timePass);
    }
    
    if(gameState == COMBAT)
    {
        CombatStateUpdate(deltaTime, timePass);
    }
}

void PlayStateRender(win32BackBuffer_t* backBuffer)
{
    if(gameState == WORLD)
    {
        WorldStateRender(backBuffer);
    }
    
    if(gameState == COMBAT)
    {
        CombatStateRender(backBuffer);
    }
}

void PlayStateClear()
{
    WorldStateClear(); 
    CombatStateClear();
}
