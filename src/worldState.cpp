#include "worldState.h"
#include "player.h"
#include "tileMap.h"

global_variable texture_t magoTexture;
global_variable texture_t tilesheetTexture;
global_variable texture_t fontTexture;
global_variable texture_t enemyTexture;

global_variable bool showColitions = false;
global_variable player_t mago;

global_variable tileMapData_t mapData;
global_variable tileMap_t actualMap;
global_variable int actualMapInt;


void WorldStateInit()
{
    showColitions = false;
    tilesheetTexture = LoadBMP("./assets/10x10.bmp");
    magoTexture = LoadBMP("./assets/mago.bmp");
    fontTexture = LoadBMP("./assets/font23.bmp");
    enemyTexture = LoadBMP("./assets/enemy.bmp");
    
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

void WorldStateInput(float deltaTime)
{
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

void WorldStateUpdate(float deltaTime, float timePass)
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
    if(PlayerColitionEnemy(mago.x, mago.y, mapData.x, mapData.y, &actualMap))
    {
        
    }
    
    mago.sprite.x = mago.x;
    mago.sprite.y = mago.y;
    mapData.sprite.x = mapData.x;
    mapData.sprite.y = mapData.y;
}

void WorldStateRender(win32BackBuffer_t* backBuffer)
{
    DrawTileMapInt(10, 10, actualMap.bottomLayer, mapData.sprite, 4, tilesheetTexture, backBuffer);
    DrawFrameTexture(mago.sprite, mago.scale, magoTexture, backBuffer);
    //DrawRect((mago.x + 20), (mago.y), 4, 4, 0xFFFF00FF, backBuffer);
    //DrawRect((mago.x + 40), (mago.y), 4, 4, 0xFFFF00FF, backBuffer);
    //DrawRect((mago.x + 20), (mago.y + 40), 4, 4, 0xFFFF00FF, backBuffer);
    //DrawRect((mago.x + 40), (mago.y + 40), 4, 4, 0xFFFF00FF, backBuffer);
    DrawTileMapInt(10, 10, actualMap.topLayer, mapData.sprite, 4, tilesheetTexture, backBuffer);
    if(showColitions)
        DrawTileMapInt(10, 10, actualMap.colitions, mapData.sprite, 4, tilesheetTexture, backBuffer);
    DrawTileMapInt(2, 2, actualMap.enemyLayer, mapData.sprite, 4, enemyTexture, backBuffer);
    //DrawRect(0, windowHeight - 48, 304, 48, 0xFFFFFFFF, backBuffer);
    //DrawString("POKEMON SOUL SILVER", 0, windowHeight - 16, fontTexture, backBuffer);
    //DrawString("AGUANTE LUGIA PAPA", 0, windowHeight - 32, fontTexture, backBuffer);
    //DrawString("HO OH SE LA COMEEE", 0, windowHeight - 48, fontTexture, backBuffer);
    DrawString("MANUTO", mago.sprite.x - 16, mago.sprite.y + 64, fontTexture, backBuffer); 
}

void WorldStateClear()
{

}
