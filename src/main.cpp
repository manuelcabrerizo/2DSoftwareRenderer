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
vec2_t magoPos;
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
    {{5, 4}, {3, 0}, {4, 0}, {4, 0}, {4, 0}, {0, 3}, {0, 3}, {4, 0}, {4, 0}, {4, 0}, {4, 0}, {4, 0}, {4, 0}, {4, 0}, {5, 0}, {5, 4}}
};

int tilesInt[600] = {
    29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
    29, 21, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 23, 29,
    29, 15, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 17, 29,
    29, 15, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 17, 29,
    29, 15, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 17, 29,
    29, 15, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 17, 29,
    29, 15, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 17, 29,
    29, 15, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 17, 29,
    29, 15, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 17, 29,
    29, 15, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 17, 29,
    29, 15, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 17, 29,
    29, 15, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 17, 29,
    29, 15, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 17, 29,
    29, 15, 16, 16, 16, 16, 16, 16, 16, 16, 16, 10, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 17, 29,
    29, 15, 16, 16, 16, 16, 16, 16, 16, 16, 17, 4,  15, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 17, 29,
    29, 15, 16, 16, 16, 16, 16, 16, 16, 16, 17, 29, 15, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 17, 29,
    29, 15, 16, 16, 16, 16, 16, 16, 16, 16, 17, 29, 15, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 17, 29,
    29, 15, 16, 16, 16, 16, 16, 16, 16, 16, 17, 29, 15, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 17, 29,
    29, 9,  10, 10, 10, 10, 10, 10, 10, 10, 11, 29,  9, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 11, 29,
    29, 3,  4,  4,  4,  4,  4,  4,  4,  4,  5,  29,  3,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  5,  29
};


bool PlayerCanMove(int x, int y)
{
    x -= tileInfo.x;
    y -= tileInfo.y;

    int tiles2[600];
    int y2 = 0;
    for(int y = 20; y > 0; y--)
    {
        int x2 = 0;
        for(int x = 0; x < 30; x++)
        {
            tiles2[(y2 * 30) + x2] = tilesInt[((y - 1) * 30) + x];
            x2++;
        }
        y2++;   
    }

    int tile1 = tiles2[((y / 64) * 30) + ((x + 20) / 64)];
    int tile2 = tiles2[((y / 64) * 30) + ((x + 40) / 64)];
    int tile3 = tiles2[(((y + 40) / 64) * 30) + ((x + 20) / 64)];    
    int tile4 = tiles2[(((y + 40) / 64) * 30) + ((x + 40) / 64)];

    if((tile1 == 29 ||
        tile1 == 5 ||
        tile1 == 4 ||
        tile1 == 3) ||
        
       (tile2 == 29 ||
        tile2 == 5 ||
        tile2 == 4 ||
        tile2 == 3) ||
        
       (tile3 == 29 ||
        tile3 == 5 ||
        tile3 == 4 ||
        tile3 == 3) ||
        
       (tile4 == 29 ||
        tile4 == 5 ||
        tile4 == 4 ||
        tile4 == 3)) 
    {
        return false;

    }
    return true;
}


void Init(void)
{
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

    tileInfo.x = 0;
    tileInfo.y = 0;
    tileInfo.width = 16;
    tileInfo.height = 16;
}

void InputHandler(HWND hwnd)
{
    Win32InputHandler(hwnd);

    magoPos.x = mago.x;
    magoPos.y = mago.y;
    
    magoIncX = magoPos.x;
    magoIncY = magoPos.y;

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
        magoPos.x = magoIncX;
        magoPos.y = magoIncY;
    }
}

void Update(void)
{
    mago.row = (int)timePass % magoNumFrames;
    tileInfo.x += (mago.x - magoPos.x);
    tileInfo.y += (mago.y - magoPos.y);
}

void Render(win32BackBuffer_t* backBuffer, HWND hwnd)
{
    //DrawTexture(0, 0, &backgroundHeader, backgroundTexture, backBuffer);
    DrawTileMapInt(tilesInt, tileInfo, 4, tilesheetTexture, backBuffer);
    DrawFrameTexture(mago, magoScale, magoTexture, backBuffer);
    DrawRect((mago.x + 20), (mago.y), 4, 4, 0xFFFF00FF, backBuffer);
    DrawRect((mago.x + 40), (mago.y), 4, 4, 0xFFFF00FF, backBuffer);
    DrawRect((mago.x + 20), (mago.y + 40), 4, 4, 0xFFFF00FF, backBuffer);
    DrawRect((mago.x + 40), (mago.y + 40), 4, 4, 0xFFFF00FF, backBuffer);
    DrawString("POKEMON SOUL SILVER", 0, windowHeight - 16, fontTexture, backBuffer);
    DrawString("AGUANTE LUGIA PAPA", 0, windowHeight - 32, fontTexture, backBuffer);
    DrawString("HOO HOO SE LA COMEEE", 0, windowHeight - 48, fontTexture, backBuffer);
    ClearBackBuffer(0xFF004400, backBuffer, hwnd);
}


int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int cmdShow)
{ 
    srand (time(NULL));

    backgroundTexture = LoadBMP("./assets/test_background.bmp");
    tilesheetTexture = LoadBMP("./assets/tilesheet.bmp");
    magoTexture = LoadBMP("./assets/mago.bmp");
    fontTexture = LoadBMP("./assets/font23.bmp");

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



