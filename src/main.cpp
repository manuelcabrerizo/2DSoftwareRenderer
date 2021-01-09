#include "display.h"
#include <stdlib.h>     
#include <stdio.h>
#include <string.h>
#include "playState.h"
#include "menuState.h"

global_variable uint32_t MSPerFrame;
global_variable float timePass;
global_variable float deltaTime;
global_variable float timer;
global_variable texture_t logoTexture;

global_variable global_state_t state;

void Init(void)
{
    logoTexture = LoadBMP("./assets/logo.bmp");
    timer = timePass;
    state = INTRO;
    deltaTime = 0.0f;
    MenuStateInit(); 
}

void InputHandler(HWND hwnd)
{
    Win32InputHandler(hwnd);
    if(state == MENU)
        MenuStateInput(deltaTime, timePass, &state);

    if(state == PLAY)
        PlayStateInput(deltaTime, timePass, &state);
}

void Update(void)
{
    if(state == INTRO)
    {
        float currentTime  = timePass - timer;
            if(currentTime > 160.0f)  //tiempo final 160
            {
                state = MENU;
            }
    }

    if(state == MENU)
        MenuStateUpdate(deltaTime, timePass);

    if(state == PLAY)
        PlayStateUpdate(deltaTime, timePass);
}


void Render(win32BackBuffer_t* backBuffer, HWND hwnd)
{
    if(state == INTRO)
        DrawTextureScale(windowWidth / 2 - (16*8), windowHeight / 2 - (16*8), 8, logoTexture,backBuffer);
    if(state == MENU)
        MenuStateRender(backBuffer);
    if(state == PLAY)
        PlayStateRender(backBuffer);

    ClearBackBuffer(0xFF000000, backBuffer, hwnd);
}


int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int cmdShow)
{ 
    LARGE_INTEGER perfCountFrequency;
    QueryPerformanceFrequency(&perfCountFrequency);
    uint64_t frequency = (uint64_t)perfCountFrequency.QuadPart;

    HWND hwnd = Win32CreateWindow("2DSoftwareRenderer", 1024, 576, hInstance);
    if(hwnd != NULL)
    {
        Init();
        win32BackBuffer_t* backBuffer = CreateBackBuffer();
        Win32SetBackBuffer(backBuffer, hwnd);
        
        LARGE_INTEGER lastCounter;
        QueryPerformanceCounter(&lastCounter);
 
        while(GetRunning())
        {
            InputHandler(hwnd);
            Update();
            Render(backBuffer, hwnd);

            LARGE_INTEGER endCounter;
            QueryPerformanceCounter(&endCounter);
            uint64_t counterElapsed =  endCounter.QuadPart - lastCounter.QuadPart;  
            MSPerFrame = (uint32_t)(((1000 * counterElapsed) / frequency));
            deltaTime = (counterElapsed / (float)frequency);
            timePass += (MSPerFrame / 40.0);               
            lastCounter = endCounter;
        }
        VirtualFree(backBuffer->memory, 0, MEM_RELEASE);
    }
    return(0); 
}





