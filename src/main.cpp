#include "display.h"
#include <stdlib.h>     
#include <time.h>
#include <stdio.h>

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
rect_t mago;
rect_t tileInfo;

// TileSheet
vec2_t tiles[9][16] = {
    {{5, 4}, {5, 4}, {5, 4}, {5, 4}, {5, 4}, {5, 4}, {5, 4}, {0, 3}, {0, 3}, {5, 4}, {5, 4}, {5, 4}, {5, 4}, {5, 4}, {5, 4}, {5, 4}},
    {{5, 4}, {3, 3}, {4, 3}, {4, 3}, {4, 3}, {4, 3}, {4, 3}, {0, 3}, {0, 3}, {4, 3}, {4, 3}, {4, 3}, {4, 3}, {4, 3}, {5, 3}, {5, 4}},
    {{5, 4}, {3, 2}, {4, 2}, {4, 2}, {4, 2}, {4, 2}, {4, 2}, {4, 2}, {4, 2}, {4, 2}, {4, 2}, {4, 2}, {4, 2}, {4, 2}, {5, 2}, {5, 4}},
    {{5, 4}, {3, 2}, {4, 2}, {4, 2}, {4, 2}, {4, 2}, {4, 2}, {4, 2}, {4, 2}, {4, 2}, {4, 2}, {4, 2}, {4, 2}, {4, 2}, {1, 2}, {1, 2}},
    {{5, 4}, {3, 2}, {4, 2}, {4, 2}, {4, 2}, {4, 2}, {4, 2}, {4, 2}, {4, 2}, {4, 2}, {4, 2}, {4, 2}, {4, 2}, {4, 2}, {1, 0}, {1, 0}},
    {{5, 4}, {3, 2}, {4, 2}, {4, 2}, {4, 2}, {4, 2}, {4, 2}, {4, 2}, {4, 2}, {4, 2}, {4, 2}, {4, 2}, {4, 2}, {4, 2}, {5, 2}, {5, 4}},
    {{5, 4}, {3, 2}, {4, 2}, {4, 2}, {4, 2}, {4, 2}, {4, 2}, {4, 2}, {4, 2}, {4, 2}, {4, 2}, {4, 2}, {4, 2}, {4, 2}, {5, 2}, {5, 4}},
    {{5, 4}, {3, 1}, {4, 1}, {4, 1}, {4, 1}, {4, 1}, {4, 1}, {4, 1}, {4, 1}, {4, 1}, {4, 1}, {4, 1}, {4, 1}, {4, 1}, {5, 1}, {5, 4}},
    {{5, 4}, {3, 0}, {4, 0}, {4, 0}, {4, 0}, {4, 0}, {4, 0}, {4, 0}, {4, 0}, {4, 0}, {4, 0}, {4, 0}, {4, 0}, {4, 0}, {5, 0}, {5, 4}}
};

int tilesInt[144] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

};


bool PlayerCanMove(int x, int y)
{
    vec2_t tiles2[9][16];
    int y2 = 0;
    for(int y = 9; y > 0; y--)
    {
        int x2 = 0;
        for(int x = 0; x < 16; x++)
        {
            tiles2[y2][x2] = tiles[y - 1][x];
            x2++;
        }
        y2++;   
    }
    
    int tileX1 = tiles2[y / 64][(x + 20) / 64].x;
    int tileY1 = tiles2[y / 64][(x + 20) / 64].y;
    
    int tileX2 = tiles2[y / 64][(x + 40) / 64].x;
    int tileY2 = tiles2[y / 64][(x + 40) / 64].y;
    
    int tileX3 = tiles2[(y + 40) / 64][(x + 20) / 64].x;
    int tileY3 = tiles2[(y + 40) / 64][(x + 20) / 64].y;
    
    int tileX4 = tiles2[(y + 40) / 64][(x + 40) / 64].x;
    int tileY4 = tiles2[(y + 40) / 64][(x + 40) / 64].y;


    if(
       (tileY1 == 4 && tileX1 == 5 ||
        tileY1 == 0 && tileX1 == 3 ||
        tileY1 == 0 && tileX1 == 4 ||
        tileY1 == 0 && tileX1 == 5) ||
        
       (tileY2 == 4 && tileX2 == 5 ||
        tileY2 == 0 && tileX2 == 3 ||
        tileY2 == 0 && tileX2 == 4 ||
        tileY2 == 0 && tileX2 == 5) ||
        
       (tileY3 == 4 && tileX3 == 5 ||
        tileY3 == 0 && tileX3 == 3 ||
        tileY3 == 0 && tileX3 == 4 ||
        tileY3 == 0 && tileX3 == 5) ||
        
       (tileY4 == 4 && tileX4 == 5 ||
        tileY4 == 0 && tileX4 == 3 ||
        tileY4 == 0 && tileX4 == 4 ||
        tileY4 == 0 && tileX4 == 5)) 
    {
        return false;

    }
    return true;
}


void Init(void)
{
    mago.x = 64;
    mago.y = 64;
    mago.width = 16;
    mago.height = 16;
    mago.row = 0;
    mago.column = 2;
    magoIncX = 64;
    magoIncY = 64;
    magoScale = 4;

    tileInfo.x = 0;
    tileInfo.y = 0;
    tileInfo.width = 16;
    tileInfo.height = 16;
}

void InputHandler(HWND hwnd)
{
    Win32InputHandler(hwnd);
    
    magoIncX = mago.x;
    magoIncY = mago.y;

    if(KeyDown(VK_UP))
    {
        magoIncY += 1.0f;
        mago.column = 0;
        magoNumFrames = 6;
    }   
    else if(KeyDown(VK_DOWN))
    {
        magoIncY -= 1.0f;
        mago.column = 1;
        magoNumFrames = 6;
    }
    else if(KeyDown(VK_RIGHT))
    {
        magoIncX += 1.0f;
        mago.column = 2;
        magoNumFrames = 6;
    }
    else if(KeyDown(VK_LEFT))
    {
        magoIncX -= 1.0f;
        mago.column = 3;
        magoNumFrames = 6;
    }
    else
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
        mago.x = magoIncX;
        mago.y = magoIncY;
    }
}

void Update(void)
{
    mago.row = (int)timePass % magoNumFrames;
}

void Render(win32BackBuffer_t* backBuffer, HWND hwnd)
{
    //DrawTexture(0, 0, &backgroundHeader, backgroundTexture, backBuffer);
    DrawTileMap(tiles, tileInfo, 4, tilesheetTexture, backBuffer);
    DrawFrameTexture(mago, magoScale, magoTexture, backBuffer);
    DrawRect((mago.x + 20), (mago.y), 4, 4, 0xFFFF00FF, backBuffer);
    DrawRect((mago.x + 40), (mago.y), 4, 4, 0xFFFF00FF, backBuffer);
    DrawRect((mago.x + 20), (mago.y + 40), 4, 4, 0xFFFF00FF, backBuffer);
    DrawRect((mago.x + 40), (mago.y + 40), 4, 4, 0xFFFF00FF, backBuffer);
    DrawString("POKEMON SOUL SILVER", 0, windowHeight - 32, fontTexture, backBuffer);

    ClearBackBuffer(0xFF000000, backBuffer, hwnd);
}


int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int cmdShow)
{ 
    srand (time(NULL));

    backgroundTexture = LoadBMP("./assets/test_background.bmp");
    tilesheetTexture = LoadBMP("./assets/tilesheet.bmp");
    magoTexture = LoadBMP("./assets/mago.bmp");
    fontTexture = LoadBMP("./assets/font.bmp");

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

        // free back buffer
    }
    return(0); 
}



