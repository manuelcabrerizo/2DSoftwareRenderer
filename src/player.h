#ifndef PLAYER_H
#define PLAYER_H

#include "display.h"

typedef struct 
{
    rect_t sprite;
    vec2_t offset;
    float x;
    float y;
    int numFrames;
    int scale;
} player_t;


bool PlayerCanMove(
        int x,
        int y,
        int tileInfoX,
        int tileInfoY,
        tileMap_t* actualMap
    );

bool PlayerColitionEnemy(
        int x,
        int y,
        int tileInfoX,
        int tileInfoY,
        tileMap_t* actualMap
    );

int GetColitionsInt(
        int x,
        int y,
        int tileInfoX,
        int tileInfoY,
        tileMap_t* actualMap
    );


#endif
