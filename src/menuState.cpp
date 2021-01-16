#include "menuState.h"
#include "playState.h"


global_variable texture_t playButtonTexture;
global_variable texture_t exitButtonTexture;
global_variable texture_t backTexture;
global_variable rect_t playButton;
global_variable rect_t exitButton;

global_variable int inputOption;
global_variable bool UpWasPress;
global_variable bool DownWasPress;

void MenuStateSetTexture()
{
    playButtonTexture = LoadBMP("./assets/PlayButton.bmp");
    exitButtonTexture = LoadBMP("./assets/ExitButton.bmp");
    backTexture = LoadBMP("./assets/backGround.bmp");
}

void MenuStateInit()
{

    playButton.x = (windowWidth / 2) - 16*6;
    playButton.y = (windowHeight / 2);
    playButton.width = 32;
    playButton.height = 16;
    playButton.row = 0;
    playButton.column = 0;

    exitButton.x = (windowWidth / 2) - 16*6;
    exitButton.y = (windowHeight / 2) - 24*6;
    exitButton.width = 32;
    exitButton.height = 16;
    exitButton.row = 0;
    exitButton.column = 0;

    inputOption = 0;
    UpWasPress = false;
    DownWasPress = false;

    PlayStateInit();
}
void MenuStateInput(float deltaTime, float timePass, global_state_t* state)
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
            PlayStateInit();
            *state = PLAY;
        }
        if(inputOption == 2)
            SetRunning(false);
    }
}
void MenuStateUpdate(float deltaTime, float timePass)
{
    if(inputOption == 1)
        playButton.row = 2;
    else
        playButton.row = 0;

    if(inputOption == 2)
        exitButton.row = 2;
    else
        exitButton.row = 0;
}
void MenuStateRender(win32BackBuffer_t* backBuffer)
{
    DrawTexture(0, 0, backTexture, backBuffer);
    DrawFrameTexture(playButton, 6, playButtonTexture, backBuffer);
    DrawFrameTexture(exitButton, 6, exitButtonTexture, backBuffer);
}

void MenuStateReset()
{
    inputOption = 0;
}
void MenuStateClear()
{

}
