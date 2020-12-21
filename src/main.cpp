#include "display.h"
#include <stdlib.h>     
#include <time.h>
#include <stdio.h>
#include <string.h>

texture_t backgroundTexture;
texture_t magoTexture;
texture_t tilesheetTexture;
texture_t fontTexture;

uint32_t MSPerFrame;
float timePass;
int windowWidth = 1024;
int windowHeight = 576;

// Player
float magoIncX;
float magoIncY;
int magoNumFrames;
int magoScale;
vec2_t magoPos;
rect_t mago;
rect_t tileInfo;

tileMap_t map0;
tileMap_t map1;
tileMap_t map2;
tileMap_t* actualMap;
int actualMapInt = 1;

bool PlayerCanMove(int x, int y)
{
    x -= tileInfo.x;
    y -= tileInfo.y;

    int tiles[600];
    int y2 = 0;
    for(int y = 20; y > 0; y--)
    {
        int x2 = 0;
        for(int x = 0; x < 30; x++)
        {
            tiles[(y2 * 30) + x2] = actualMap->colitions[((y - 1) * 30) + x];
            x2++;
        }
        y2++;   
    }

    int tile1 = tiles[((y / 64) * 30) + ((x + 20) / 64)];
    int tile2 = tiles[((y / 64) * 30) + ((x + 40) / 64)];
    int tile3 = tiles[(((y + 40) / 64) * 30) + ((x + 20) / 64)];    
    int tile4 = tiles[(((y + 40) / 64) * 30) + ((x + 40) / 64)];

    if((tile1 == 1) || (tile2 == 1) || (tile3 == 1) || (tile4 == 1)) 
    {
        return false;
    }
    return true;
}

bool ChangeMap(int x, int y)
{
    x -= tileInfo.x;
    y -= tileInfo.y;

    int tiles[600];
    int y2 = 0;
    for(int y = 20; y > 0; y--)
    {
        int x2 = 0;
        for(int x = 0; x < 30; x++)
        {
            tiles[(y2 * 30) + x2] = actualMap->colitions[((y - 1) * 30) + x];
            x2++;
        }
        y2++;   
    }

    int tile1 = tiles[((y / 64) * 30) + ((x + 20) / 64)];
    int tile2 = tiles[((y / 64) * 30) + ((x + 40) / 64)];

    if(((tile1 == 2) || (tile2 == 2)) || ((tile1 == 3) || (tile2 == 3))) 
    {
        return true;
    }
    return false;
}

int GetColitionsInt(int x, int y)
{
    x -= tileInfo.x;
    y -= tileInfo.y;

    int tiles[600];
    int y2 = 0;
    for(int y = 20; y > 0; y--)
    {
        int x2 = 0;
        for(int x = 0; x < 30; x++)
        {
            tiles[(y2 * 30) + x2] = actualMap->colitions[((y - 1) * 30) + x];
            x2++;
        }
        y2++;   
    }

    int tile = tiles[((y / 64) * 30) + ((x + 30) / 64)];
    return tile;
}


void Init(void)
{
    tilesheetTexture = LoadBMP("./assets/10x10.bmp");
    magoTexture = LoadBMP("./assets/mago.bmp");
    fontTexture = LoadBMP("./assets/font23.bmp");

    mago.x = (windowWidth / 2) - 32;
    mago.y = (windowHeight / 2) - 32;
    mago.width = 16;
    mago.height = 16;
    mago.row = 0;
    mago.column = 2;
    magoIncX = 64;
    magoIncY = 64;
    magoScale = 4;
    magoPos.x = (windowWidth / 2) - 32;
    magoPos.y = (windowHeight / 2) - 32;
    magoNumFrames = 1;

    tileInfo.x = 0;
    tileInfo.y = 0;
    tileInfo.width = 16;
    tileInfo.height = 16;

    LoadMapFromFile("./assets/maps/test1.map", &map0);
    LoadMapFromFile("./assets/maps/test2.map", &map1);
    LoadMapFromFile("./assets/maps/test3.map", &map2);
    actualMap = &map0;
}

void InputHandler(HWND hwnd)
{
    Win32InputHandler(hwnd);

    magoPos.x = mago.x;
    magoPos.y = mago.y;
    
    magoIncX = magoPos.x;
    magoIncY = magoPos.y;


    if(KeyDown(0x31))
    {
        actualMap = &map0;
    }
    if(KeyDown(0x32))
    {
        actualMap = &map1;
    }
    if(KeyDown(0x33))
    {
        actualMap = &map2;
    }


    if(KeyDown(VK_UP))
    {
        magoIncY += 3.0f;
        mago.column = 0;
        magoNumFrames = 6;
    }   
    else if(KeyDown(VK_DOWN))
    {
        magoIncY -= 3.0f;
        mago.column = 1;
        magoNumFrames = 6;
    }
    else if(KeyDown(VK_RIGHT))
    {
        magoIncX += 3.0f;
        mago.column = 2;
        magoNumFrames = 6;
    } 
    else if(KeyDown(VK_LEFT))
    {
        magoIncX -= 3.0f;
        mago.column = 3;
        magoNumFrames = 6;
    }

    if(!KeyDown(VK_UP) && !KeyDown(VK_DOWN) && !KeyDown(VK_RIGHT) && !KeyDown(VK_LEFT))
    {
        magoNumFrames = 1;
    }
    
    if(magoIncY + mago.height * magoScale >= windowHeight)
    {
        magoIncY = windowHeight - mago.height * magoScale;
    }
    if(magoIncY <= 0)
    {
        magoIncY = 0;
    }
    if(magoIncX + mago.width * magoScale >= windowWidth)
    {
        magoIncX = windowWidth - mago.width * magoScale;
    }
    if(magoIncX <= 0)
    {
        magoIncX = 0;
    }

    if(PlayerCanMove(magoIncX, magoIncY))
    {
        magoPos.x = magoIncX;
        magoPos.y = magoIncY;
    }
}

void Update(void)
{
    mago.row = (int)timePass % magoNumFrames;
     
    tileInfo.x += (mago.x - magoPos.x);
    tileInfo.y += (mago.y - magoPos.y);
    if(tileInfo.x > 0 || mago.x < (windowWidth / 2) - 32)
    {
        tileInfo.x = 0;
        mago.x -= (mago.x - magoPos.x); 
    }
    
    if(tileInfo.x + (30 * (16 * 4)) < windowWidth || mago.x > (windowWidth / 2) + 32)
    {
        tileInfo.x = windowWidth - (30 * (16 * 4)); 
        mago.x -= (mago.x - magoPos.x);
    }

     
    if(tileInfo.y > 0 || mago.y < (windowHeight / 2) - 32)
    {
        tileInfo.y = 0;
        mago.y -= (mago.y - magoPos.y);
    }
    
    if(tileInfo.y + (20 * (16 * 4)) < windowHeight || mago.y > (windowHeight / 2) + 32)
    {
        tileInfo.y = windowHeight - (20 * (16 * 4)); 
        mago.y -= (mago.y - magoPos.y);
    }
    
    if(ChangeMap(mago.x, mago.y))
    { 
        if(actualMapInt == 1)
        {
            if(GetColitionsInt(mago.x, mago.y) == 3)
            {
                actualMapInt = 3;
                actualMap = &map2;
                mago.y = 128;
            }
            else
            {
                actualMapInt = 2;
                actualMap = &map1;
                mago.y = 64;
                tileInfo.x += 128;
            }
        }
        else if(actualMapInt == 3)
        {
            actualMapInt = 1;
            actualMap = &map0;
            mago.y = (windowHeight / 2) - 32;
            tileInfo.y = -384;
        }
        else if(actualMapInt == 2)
        {
            actualMapInt = 1;
            actualMap = &map0;
            mago.y = windowHeight - 64;
            tileInfo.x -= 128;
        } 
    }
}


void Render(win32BackBuffer_t* backBuffer, HWND hwnd)
{
    DrawTileMapInt(10, 10, actualMap->bottomLayer, tileInfo, 4, tilesheetTexture, backBuffer);
    DrawFrameTexture(mago, magoScale, magoTexture, backBuffer);
    DrawTileMapInt(10, 10, actualMap->topLayer, tileInfo, 4, tilesheetTexture, backBuffer);
    DrawString("POKEMON SOUL SILVER", 0, windowHeight - 16, fontTexture, backBuffer);
    DrawString("AGUANTE LUGIA PAPA", 0, windowHeight - 32, fontTexture, backBuffer);
    DrawString("HO OH SE LA COMEEE", 0, windowHeight - 48, fontTexture, backBuffer);
    DrawString("MANUTO", mago.x - 16, mago.y + 64, fontTexture, backBuffer);
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
            // TODO: desplay value
            uint64_t counterElapsed =  endCounter.QuadPart - lastCounter.QuadPart;  
            MSPerFrame = (uint32_t)(((1000 * counterElapsed) / frequency));
            timePass += (MSPerFrame / 40.0f);               
            lastCounter = endCounter;
        }
        VirtualFree(backBuffer->memory, 0, MEM_RELEASE);
    }
    return(0); 
}
