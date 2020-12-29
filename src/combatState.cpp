#include "combatState.h"
#include <time.h>

global_variable texture_t magoTexture;
global_variable texture_t fontTexture;
global_variable texture_t arenaTexture;
global_variable texture_t enemyCombatTexture;

global_variable rect_t magoCombat;
global_variable lifeBar_t lifeBar;
global_variable lifeBar_t lifeBar2;

global_variable int inputOption;
global_variable int fireBall;
global_variable int iceBall;
global_variable int arcanWall;

global_variable int youTurn;
global_variable float timer;
global_variable bool UpWasPress;
global_variable bool DownWasPress;


void CombatStateInit()
{
    arenaTexture = LoadBMP("./assets/arena.bmp");
    enemyCombatTexture = LoadBMP("./assets/enemyCombat.bmp");
    magoTexture = LoadBMP("./assets/mago.bmp");
    fontTexture = LoadBMP("./assets/font23.bmp");

    inputOption = 1;

    fireBall = 20;
    iceBall = 10;
    arcanWall = 1;

    lifeBar.sprite.x = 32 + 550;
    lifeBar.sprite.y = 256;
    lifeBar.sprite.width = 252;
    lifeBar.sprite.height = 32;
    lifeBar.life = 100;

    lifeBar2.sprite.x = 75;
    lifeBar2.sprite.y = 500;
    lifeBar2.sprite.width = 252;
    lifeBar2.sprite.height = 32;
    lifeBar2.life = 100;

    magoCombat.x = 174;
    magoCombat.y = 158;
    magoCombat.width = 16;
    magoCombat.height = 16;
    magoCombat.column = 2;
    magoCombat.row = 0;

    youTurn = 1;
    timer = 0.0f;
    UpWasPress = false;
    DownWasPress = false;

    srand (time(NULL));

}

void CombatStateInput(float deltaTime, float timePass)
{
    if(youTurn == 1)
    {
        if(KeyDown(VK_UP) && UpWasPress == false)
        {
            if(inputOption > 1)
                inputOption--;
            UpWasPress = true;
        }
        else if(KeyDown(VK_DOWN) && DownWasPress == false)
        {
            if(inputOption < 3)
                inputOption++;
            DownWasPress = true;
        }

        if(KeyUp(VK_UP) && UpWasPress == true)
        {
            UpWasPress = false;
        }
        else if(KeyUp(VK_DOWN) && DownWasPress == true)
        {
            DownWasPress = false;
        }

        if(KeyDown(0x0D))
        {
            if(inputOption == 1)
                lifeBar2.life -= fireBall;
            if(inputOption == 2)
                lifeBar2.life -= iceBall;
            if(inputOption == 3)
                lifeBar2.life -= arcanWall;

            youTurn = 0; 
            timer = timePass;
        }
    }
}

void CombatStateUpdate(float deltaTime, float timePass)
{
    if(youTurn == 0)
    { 
        float currentTime  = timePass - timer;
        if(currentTime > 40.0f)
        { 
            int option = (rand() % 3) + 1;
            if(option == 1)
                lifeBar.life -= fireBall;
            else if(option == 2)
                lifeBar.life -= iceBall;
            else if(option == 3) 
                lifeBar.life -= arcanWall;
            youTurn = 1;
        }
    }

}

void CombatStateRender(win32BackBuffer_t* backBuffer)
{
    DrawTexture(0, 0, arenaTexture, backBuffer);
    DrawFrameTexture(magoCombat, 8, magoTexture, backBuffer);
    DrawTextureScale(750, 420, 8, enemyCombatTexture, backBuffer);

    DrawRect(550, 0, 320, 320, 0xFFFFFFFF, backBuffer);
    DrawLifeBar(lifeBar.sprite, lifeBar.life, backBuffer);

    if(inputOption == 1)
    {
        DrawRect(32 + 550, 232, 288 - 64, 16, 0xFFFFFF99, backBuffer);
    }
    else if(inputOption == 2)
    {
        DrawRect(32 + 550, 232 - 24, 288 - 64, 16, 0xFFFFFF99, backBuffer);
    }
    else if(inputOption == 3)
    {
        DrawRect(32 + 550, 232 - 48, 288 - 64, 16, 0xFFFFFF99, backBuffer);
    }

    DrawString("FIREBALL", 32 + 550, 232, fontTexture, backBuffer);
    DrawString("ICEBALL", 32 + 550, 232 - 24, fontTexture, backBuffer);
    DrawString("ARCAN WALL", 32 + 550, 232 - 48, fontTexture, backBuffer);

    
    DrawRect(40, 484, 320, 64, 0xFFFFFFFF, backBuffer);
    DrawLifeBar(lifeBar2.sprite, lifeBar2.life, backBuffer);
}

void CombatStateClear()
{

}
