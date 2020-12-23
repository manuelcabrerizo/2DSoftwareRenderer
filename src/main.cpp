#include "display.h"
#include <stdlib.h>     
#include <time.h>
#include <stdio.h>
#include <string.h>
#include "player.h"
#include "tileMap.h"

uint32_t MSPerFrame;
float timePass;
float deltaTime;

texture_t backgroundTexture;
texture_t magoTexture;
texture_t tilesheetTexture;
texture_t fontTexture;

player_t mago;

tileMapData_t mapData;
tileMap_t actualMap;
int actualMapInt;

void Init(void)
{
    deltaTime = 0.0f;

    tilesheetTexture = LoadBMP("./assets/10x10.bmp");
    magoTexture = LoadBMP("./assets/mago.bmp");
    fontTexture = LoadBMP("./assets/font23.bmp");

    mago.sprite.x = (windowWidth / 2) - 32;
    mago.sprite.y = (windowHeight / 2) - 32;
    mago.offset.x = mago.sprite.x;
    mago.offset.y = mago.sprite.y;
    mago.x = mago.sprite.x;
    mago.y = mago.sprite.y;
    mago.sprite.width = 16;
    mago.sprite.height = 16;
    mago.sprite.row = 0;
    mago.sprite.column = 2;
    mago.scale = 4;
    mago.numFrames = 1;

    mapData.sprite.x = 0;
    mapData.sprite.y = 0;
    mapData.sprite.width = 16;
    mapData.sprite.height = 16;
    mapData.x = mapData.sprite.x;
    mapData.y = mapData.sprite.y;
    LoadMaps();
    SetMapa(actualMap, 3);
    actualMapInt = 3;
}

void InputHandler(HWND hwnd)
{
    Win32InputHandler(hwnd);

    float magoIncX = mago.x;
    float magoIncY = mago.y;

    if(KeyDown(VK_UP))
    {
        magoIncY += 200 * deltaTime;
        mago.sprite.column = 0;
        mago.numFrames = 6;
    }   
    else if(KeyDown(VK_DOWN))
    {
        magoIncY -= 200 * deltaTime;
        mago.sprite.column = 1;
        mago.numFrames = 6;
    }
    else if(KeyDown(VK_RIGHT))
    {
        magoIncX += 200 * deltaTime;
        mago.sprite.column = 2;
        mago.numFrames = 6;
    } 
    else if(KeyDown(VK_LEFT))
    {
        magoIncX -= 200 * deltaTime; 
        mago.sprite.column = 3;
        mago.numFrames = 6;
    }
    
    if(!KeyDown(VK_UP) && !KeyDown(VK_DOWN) && !KeyDown(VK_RIGHT) && !KeyDown(VK_LEFT))
    {
        mago.numFrames = 1;
    }

    if(magoIncY + mago.sprite.height * mago.scale >= windowHeight)
    {
        magoIncY = windowHeight - mago.sprite.height * mago.scale;
    }
    if(magoIncY <= 0)
    {
        magoIncY = 0;
    }
    if(magoIncX + mago.sprite.width * mago.scale >= windowWidth)
    {
        magoIncX = windowWidth - mago.sprite.width * mago.scale;
    }
    if(magoIncX <= 0)
    {
        magoIncX = 0;
    }

    if(PlayerCanMove(magoIncX, magoIncY, mapData.x, mapData.y, &actualMap))
    {
        mago.offset.x = (mago.x - magoIncX);
        mago.offset.y = (mago.y - magoIncY);
    }
    else
    {
        mago.offset.x = 0;
        mago.offset.y = 0;
    }    
}

void Update(void)
{
    mago.sprite.row = (int)timePass % mago.numFrames;

    mapData.x += mago.offset.x;
    mapData.y += mago.offset.y;

    if(mapData.x > 0 || mago.x < (windowWidth / 2) - 32)
    {
        mapData.x = 0;
        mago.x -= mago.offset.x;
    } 
    if(mapData.x + (30 * (16 * 4)) < windowWidth || mago.x > (windowWidth / 2) + 32)
    {
        mapData.x = windowWidth - (30 * (16 * 4));
        mago.x -= mago.offset.x;
    }
    if(mapData.y > 0 || mago.y < (windowHeight / 2) - 32)
    {
        mapData.y = 0;
        mago.y-= mago.offset.y;
    }
    if(mapData.y + (20 * (16 * 4)) < windowHeight || mago.y > (windowHeight / 2) + 32)
    {
        mapData.y = windowHeight - (20 * (16 * 4));
        mago.y -= mago.offset.y;
    }
    
    MapHandler(mago.x, mago.y, mapData.x, mapData.y, actualMapInt, actualMap);
    
    mago.sprite.x = mago.x;
    mago.sprite.y = mago.y;
    mapData.sprite.x = mapData.x;
    mapData.sprite.y = mapData.y;
}


void Render(win32BackBuffer_t* backBuffer, HWND hwnd)
{
    DrawTileMapInt(10, 10, actualMap.bottomLayer, mapData.sprite, 4, tilesheetTexture, backBuffer);
    DrawFrameTexture(mago.sprite, mago.scale, magoTexture, backBuffer);

    DrawRect((mago.x + 20), (mago.y), 4, 4, 0xFFFF00FF, backBuffer);
    DrawRect((mago.x + 40), (mago.y), 4, 4, 0xFFFF00FF, backBuffer);
    DrawRect((mago.x + 20), (mago.y + 40), 4, 4, 0xFFFF00FF, backBuffer);
    DrawRect((mago.x + 40), (mago.y + 40), 4, 4, 0xFFFF00FF, backBuffer);

    DrawTileMapInt(10, 10, actualMap.topLayer, mapData.sprite, 4, tilesheetTexture, backBuffer);

    DrawRect(0, windowHeight - 48, 304, 48, 0xFFFFFFFF, backBuffer);
    DrawString("POKEMON SOUL SILVER", 0, windowHeight - 16, fontTexture, backBuffer);
    DrawString("AGUANTE LUGIA PAPA", 0, windowHeight - 32, fontTexture, backBuffer);
    DrawString("HO OH SE LA COMEEE", 0, windowHeight - 48, fontTexture, backBuffer);
    DrawString("MANUTO", mago.sprite.x - 16, mago.sprite.y + 64, fontTexture, backBuffer);
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





