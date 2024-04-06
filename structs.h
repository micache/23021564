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

struct Sprite
{
    SDL_Texture* texture;
    std::vector<SDL_Rect> clips;
    int currentFrame = 0;

    void init(SDL_Texture* _texture, int frames, const int _clips [][4])
    {
        texture = _texture;
        SDL_Rect clip;
        for (int i = 0; i < frames; i++)
        {
            clip.x = _clips[i][0];
            clip.y = _clips[i][1];
            clip.w = _clips[i][2];
            clip.h = _clips[i][3];
            clips.push_back(clip);
        }
    }

    void tick()
    {
        currentFrame = (currentFrame + 1) % clips.size();
    }

    const SDL_Rect* getCurrentClip() const
    {
        return &(clips[currentFrame]);
    }

};

struct Unit
{
    string name;
    MapTile* curPos;
    int hp, dame, steps, id;
    bool player;
    Sprite* walk, atk, hit;

    Unit (int id, MapTile* _curPos, bool side, Sprite* walk, Sprite* atk, Sprite* hit)
    {
        this->id = id;
        name = CLASS_NAME[id];
        hp = CLASS_HP[id];
        dame = CLASS_DAME[id];
        steps = CLASS_STEP[id];
        curPos = _curPos;
        player = side;
        this.walk = walk;
        this.atk = atk;
        this.hit = hit;
    }

    SDL_Texture* getTexture(Graphics& graphics)
    {
        string filePath = "assets/" + name + "_icon_" + std::to_string(player) + ".png";
        return graphics.loadTexture(filePath.c_str());
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
