#include "player.h"


bool PlayerCanMove(int x, int y, int tileInfoX, int tileInfoY, tileMap_t* actualMap)
{
    x -= tileInfoX;
    y -= tileInfoY;

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

int GetColitionsInt(int x, int y, int tileInfoX, int tileInfoY, tileMap_t* actualMap)
{
    x -= tileInfoX;
    y -= tileInfoY;

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
