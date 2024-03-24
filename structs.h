#ifndef _STRUCTS__H
#define _STRUCTS__H

#include <SDL.h>
#include "defs.h"
#include <iostream>

using namespace std;

struct MapTile
{
    pair<float, float> points[6];
    int colorId;

    MapTile ()
    {
        colorId = MAP_COLOR_GRASS;
    }
};

struct Entity {

};

struct App {

};
#endif // _APP__H
