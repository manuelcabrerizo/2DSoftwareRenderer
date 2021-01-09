#include "pauseState.h"
#include "menuState.h"
#include "combatState.h"

global_variable texture_t resumeButtonTexture;
global_variable texture_t menuButtonTexture;
global_variable rect_t resumeButton;
global_variable rect_t menuButton;

global_variable int inputOption;
global_variable bool UpWasPress;
global_variable bool DownWasPress;

void PauseStateInit()
{
    resumeButtonTexture = LoadBMP("./assets/ResumeButton.bmp");
    menuButtonTexture = LoadBMP("./assets/MenuButton.bmp");

    resumeButton.x = (windowWidth / 2) - 16*6;
    resumeButton.y = (windowHeight / 2);
    resumeButton.width = 32;
    resumeButton.height = 16;
    resumeButton.row = 0;
    resumeButton.column = 0;

    menuButton.x = (windowWidth / 2) - 16*6;
    menuButton.y = (windowHeight / 2) - 24*6;
    menuButton.width = 32;
    menuButton.height = 16;
    menuButton.row = 0;
    menuButton.column = 0;

    inputOption = 0;
    UpWasPress = false;
    DownWasPress = false;
}

void PauseStateInput(float deltaTime, float timePass, global_state_t* state, state_t* gameState, state_t lastState)
{
    if(KeyDown(VK_UP) && UpWasPress == false)
    {
        if(inputOption > 1)
            inputOption--;
        UpWasPress = true;
    }
    else if(KeyDown(VK_DOWN) && DownWasPress == false)
    {
        if(inputOption < 2)
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
        {
            if(lastState == WORLD)
                *gameState = WORLD;
            if(lastState == COMBAT)
            { 
                *gameState = COMBAT;
            }
        }       
        if(inputOption == 2)
        {
            MenuStateReset();
            *state = MENU;
        }
        SetEnterWasPress(true);
    }
}

void PauseStateUpdate(float deltaTime, float timePass)
{
    if(inputOption == 1)
        resumeButton.row = 2;
    else
        resumeButton.row = 0;

    if(inputOption == 2)
        menuButton.row = 2;
    else
        menuButton.row = 0;
}

void PauseStateRender(win32BackBuffer_t* backBuffer)
{
    DrawFrameTexture(resumeButton, 6, resumeButtonTexture, backBuffer);
    DrawFrameTexture(menuButton, 6, menuButtonTexture, backBuffer);
}

void PauseStateClear()
{

}
