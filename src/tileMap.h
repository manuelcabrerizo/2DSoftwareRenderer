#ifndef TILEMAP_H
#define TILEMAP_H

#include "display.h"
typedef struct 
{
    float x;
    float y;
    rect_t sprite;
} tileMapData_t;

void LoadMaps();

bool ChangeMap(
        int x,
        int y,
        float xf,
        float yf,
        tileMap_t* actualMap
    );
void MapHandler(
        float& x,
        float& y,
        float& xf,
        float& yf,
        int& actualMapInt,
        tileMap_t&
        actualMap
    );

void SetMapa(tileMap_t& map, int index);
tileMap_t GetMap(int index);


#endif
