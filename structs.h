#ifndef _STRUCTS__H
#define _STRUCTS__H

#include <SDL.h>
#include <SDL_image.h>
#include "defs.h"
#include <iostream>
#include <string>

using namespace std;

struct MapTile
{
    pair<float, float> points[6], center;
    int colorId;

    MapTile ()
    {
        colorId = MAP_COLOR_GRASS;
    }

    bool operator == (const MapTile& other) const
    {
        return center == other.center;
    }
};

struct Unit
{
    string name;
    MapTile* curPos;
    int hp, dame, steps, id;
    bool player;
    SDL_Texture *texture;

    Unit (int id, MapTile* _curPos, bool side, Graphics& graphics)
    {
        this->id = id;
        name = CLASS_NAME[id];
        hp = CLASS_HP[id];
        dame = CLASS_DAME[id];
        steps = CLASS_STEP[id];
        curPos = _curPos;
        player = side;
        string filePath = "assets/" + name + "_icon_" + std::to_string(player) + ".png";
        texture = graphics.loadTexture(filePath.c_str());
    }

    void attack(Unit& other)
    {
        other.hp -= dame;
    }

    bool operator == (const Unit& other) const
    {
        return curPos == other.curPos && name == other.name && player == other.player;
    }
};

#endif
