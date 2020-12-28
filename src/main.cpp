#include "display.h"
#include <stdlib.h>     
#include <time.h>
#include <stdio.h>
#include <string.h>
#include "playState.h"

global_variable uint32_t MSPerFrame;
global_variable float timePass;
global_variable float deltaTime;

void Init(void)
{
    deltaTime = 0.0f;
    PlayStateInit(); 
}

void InputHandler(HWND hwnd)
{
    Win32InputHandler(hwnd);
    PlayStateInput(deltaTime, timePass);
}

void Update(void)
{
    PlayStateUpdate(deltaTime, timePass);
}


void Render(win32BackBuffer_t* backBuffer, HWND hwnd)
{
    PlayStateRender(backBuffer);
    ClearBackBuffer(0xFF000000, backBuffer, hwnd);
}


int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int cmdShow)
{ 
    srand (time(NULL));

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





