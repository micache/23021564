#ifndef _STRUCTS__H
#define _STRUCTS__H

#include <SDL.h>
#include "defs.h"
#include <iostream>

using namespace std;

struct MapTile
{
    float points[6];

    MapTile (float _points*)
    {
        for (int i = 0; i < 6; i++)
        {
            points[i] = _points[i];
        }
    }

};

struct Entity {

};

struct App {

};
#endif // _APP__H
