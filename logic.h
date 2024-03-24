#ifndef _LOGIC__H
#define _LOGIC__H

#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>
#include "defs.h"
#include "graphics.h"
#include "structs.h"

struct Game
{
    vector<MapTile*> mapTile;



    void draw(Graphics& graphics)
    {
        drawMap(graphics.renderer)
    }
};

#endif // _LOGIC__H
