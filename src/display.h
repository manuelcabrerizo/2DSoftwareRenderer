#ifndef DISPLAY_H
#define DISPLAY_H

#include <windows.h>
#include "input.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define internal static 
#define local_persist static 
#define global_variable static

#define windowWidth 1024
#define windowHeight 576

typedef struct 
{
    int hp_now;
    int hp_max;
    int mp_now;
    int mp_max;
    int strong;
    int defense;
} stats_t;

typedef struct
{
    BITMAPINFO info;
    HBITMAP handle;
    void* memory;
    int width;
    int height;
    int bytesPerPixel = 4;
} win32BackBuffer_t;

#pragma pack(push, 1)
typedef struct
{
    uint16_t fileType;
    uint32_t fileSize;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t bitmapOffset;
    uint32_t size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bitsPerPixel;
} bitmapHeader;
#pragma pack(pop)

typedef struct
{
    uint32_t* pixels;
    int width;
    int height;
}texture_t;

typedef struct 
{
    int x;
    int y;
    int width;
    int height;
    int row;
    int column;
} rect_t;

typedef struct 
{
    int bottomLayer[600];
    int topLayer[600];
    int colitions[600];
    int enemyLayer[600];
    int indiaLayer[600];
    int lolaLayer[600];
} tileMap_t;

enum global_state_t {
    INTRO,
    MENU,
    PLAY
};

enum state_t {
    WORLD,
    COMBAT,
    PAUSE
};

HWND Win32CreateWindow(const char* windowName, int width, int height, HINSTANCE hInstance);
void Win32InputHandler(HWND hwnd);
void Win32SetBackBuffer(win32BackBuffer_t* backBuffer, HWND hwnd);
void Win32UpdateBackBuffer(HDC deviceContext, int x, int y, int width, int height, win32BackBuffer_t* backBuffer);
void Win32ClearBackBuffer(uint32_t color, win32BackBuffer_t* backBuffer);
void ClearBackBuffer(uint32_t color ,win32BackBuffer_t* backBuffer, HWND hwnd);
void DrawPixel(int x, int y, uint32_t color, win32BackBuffer_t* backBuffer);
bool GetRunning(void);
void SetRunning(bool value);
win32BackBuffer_t* CreateBackBuffer(void);
texture_t LoadBMP(const char* filename);
void DrawRect(int xPos, int yPos, int width, int height, uint32_t color, win32BackBuffer_t* backBuffer);
void DrawTexture(int xPos, int yPos, texture_t texture, win32BackBuffer_t* backBuffer);
void DrawTextureScale(int xPos, int yPos, int scale, texture_t texture, win32BackBuffer_t* backBuffer);
void DrawFrameTexture(rect_t srcRect, int scale, texture_t texture, win32BackBuffer_t* backBuffer);
void DrawTileMapInt(int columns, int rows, int tiles[], rect_t tileInfo, int scale, texture_t texture, win32BackBuffer_t* backBuffer);
void DrawString(const char* message, int posX, int posY, texture_t texture, win32BackBuffer_t* backBuffer);
void LoadMapFromFile(const char* filePath, tileMap_t* map);
void DrawLifeBar(rect_t sprite, int life, win32BackBuffer_t* backBuffer);


#endif

