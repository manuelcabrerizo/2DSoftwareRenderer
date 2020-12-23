#include "display.h"
#include <stdlib.h>     
#include <time.h>
#include <stdio.h>
#include <string.h>

typedef struct 
{
    rect_t sprite;
    vec2_t offset;
    float x;
    float y;
    int numFrames;
    int scale;
} player_t;

texture_t backgroundTexture;
texture_t magoTexture;
texture_t tilesheetTexture;
texture_t fontTexture;

uint32_t MSPerFrame;
float timePass;
float deltaTime;

int windowWidth = 1024;
int windowHeight = 576;

player_t mago;

float tileXf;
float tileYf;
rect_t tileInfo;


tileMap_t map0;
tileMap_t map1;
tileMap_t map2;
tileMap_t* actualMap;
int actualMapInt;

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

void MapHandler(float& x, float& y, float& tileXf, float& tileYf)
{
    if(ChangeMap(x, y))
    { 
        if(actualMapInt == 1)
        {
            if(GetColitionsInt(x, y) == 3)
            {
                actualMapInt = 3;
                actualMap = &map2;
                tileYf = 0;
                y = 128;
            }
            else
            {
                actualMapInt = 2;
                actualMap = &map1;
                y = 64;
                tileXf += 128;
            }
        }
        else if(actualMapInt == 3)
        {
            actualMapInt = 1;
            actualMap = &map0;
            y = (windowHeight / 2) - 32;
            tileYf = -383;
        }
        else if(actualMapInt == 2)
        {
            actualMapInt = 1;
            actualMap = &map0;
            y = windowHeight - 64;
            tileXf -= 128;
        } 
    }
}

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

    tileInfo.x = 0;
    tileInfo.y = 0;
    tileXf = tileInfo.x;
    tileYf = tileInfo.y;
    tileInfo.width = 16;
    tileInfo.height = 16;

    LoadMapFromFile("./assets/maps/test1.map", &map0);
    LoadMapFromFile("./assets/maps/test2.map", &map1);
    LoadMapFromFile("./assets/maps/test3.map", &map2);
    actualMap = &map1;
    actualMapInt = 2;
}

void InputHandler(HWND hwnd)
{
    Win32InputHandler(hwnd);

    float magoIncX = mago.x;
    float magoIncY = mago.y;

    if(KeyDown(VK_UP))
    {
        magoIncY += 150 * deltaTime;
        mago.sprite.column = 0;
        mago.numFrames = 6;
    }   
    else if(KeyDown(VK_DOWN))
    {
        magoIncY -= 150 * deltaTime;
        mago.sprite.column = 1;
        mago.numFrames = 6;
    }
    else if(KeyDown(VK_RIGHT))
    {
        magoIncX += 150 * deltaTime;
        mago.sprite.column = 2;
        mago.numFrames = 6;
    } 
    else if(KeyDown(VK_LEFT))
    {
        magoIncX -= 150 * deltaTime; 
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

    if(PlayerCanMove(magoIncX, magoIncY))
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

    tileXf += mago.offset.x;
    tileYf += mago.offset.y;

    if(tileXf > 0 || mago.x < (windowWidth / 2) - 32)
    {
        tileXf = 0;
        mago.x -= mago.offset.x;
    } 
    if(tileXf + (30 * (16 * 4)) < windowWidth || mago.x > (windowWidth / 2) + 32)
    {
        tileXf = windowWidth - (30 * (16 * 4)); 
        mago.x -= mago.offset.x;
    }
    if(tileYf > 0 || mago.y < (windowHeight / 2) - 32)
    {
        tileYf = 0;
        mago.y-= mago.offset.y;
    }
    if(tileYf + (20 * (16 * 4)) < windowHeight || mago.y > (windowHeight / 2) + 32)
    {
        tileYf = windowHeight - (20 * (16 * 4)); 
        mago.y -= mago.offset.y;
    }
    
    MapHandler(mago.x, mago.y, tileXf, tileYf);
    
    mago.sprite.x = mago.x;
    mago.sprite.y = mago.y;
    tileInfo.x = tileXf;
    tileInfo.y = tileYf;
}


void Render(win32BackBuffer_t* backBuffer, HWND hwnd)
{
    DrawTileMapInt(10, 10, actualMap->bottomLayer, tileInfo, 4, tilesheetTexture, backBuffer);
    DrawFrameTexture(mago.sprite, mago.scale, magoTexture, backBuffer);

    DrawRect((mago.x + 20), (mago.y), 4, 4, 0xFFFF00FF, backBuffer);
    DrawRect((mago.x + 40), (mago.y), 4, 4, 0xFFFF00FF, backBuffer);
    DrawRect((mago.x + 20), (mago.y + 40), 4, 4, 0xFFFF00FF, backBuffer);
    DrawRect((mago.x + 40), (mago.y + 40), 4, 4, 0xFFFF00FF, backBuffer);

    DrawTileMapInt(10, 10, actualMap->topLayer, tileInfo, 4, tilesheetTexture, backBuffer);
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





