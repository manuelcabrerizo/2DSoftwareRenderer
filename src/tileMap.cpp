#include "tileMap.h"
#include "player.h"

tileMap_t map0;
tileMap_t map1;
tileMap_t map2;

void LoadMaps()
{
    LoadMapFromFile("./assets/maps/test1.map", &map0);
    LoadMapFromFile("./assets/maps/test2.map", &map1);
    LoadMapFromFile("./assets/maps/test3.map", &map2);
}

bool ChangeMap(int x, int y, float xf, float yf, tileMap_t* actualMap)
{
    x -= xf;
    y -= yf;

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

void MapHandler(float& x, float& y, float& xf, float& yf, int& actualMapInt, tileMap_t& actualMap)
{
    if(ChangeMap(x, y, xf, yf, &actualMap))
    { 
        if(actualMapInt == 1)
        {
            if(GetColitionsInt(x, y, xf, yf, &actualMap) == 3)
            {
                actualMapInt = 3;
                actualMap = GetMap(3);
                yf = 0;
                y = 128;
            }
            else
            {
                actualMapInt = 2;
                actualMap = GetMap(2);
                y = 64;
                xf += 128;
            }
        }
        else if(actualMapInt == 3)
        {
            actualMapInt = 1;
            actualMap = GetMap(1);
            y = (windowHeight / 2) - 32;
            yf = -383;
        }
        else if(actualMapInt == 2)
        {
            actualMapInt = 1;
            actualMap = GetMap(1);
            y = windowHeight - 64;
            xf -= 128;
        } 
    }
}

void SetMapa(tileMap_t& map, int index)
{
    if (index == 1)
    {
        map = map0;
    }
    else if(index == 2)
    {
        map = map1;
    }
    else if(index == 3)
    {
        map = map2;
    }
    else
    {
        map = map0;
    }
}

tileMap_t GetMap(int index)
{
    if (index == 1)
    {
        return map0;
    }
    else if(index == 2)
    {
        return map1;
    }
    else if(index == 3)
    {
        return map2;
    }
    else
    {
        return map0;
    }
}
