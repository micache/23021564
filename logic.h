#ifndef _LOGIC__H
#define _LOGIC__H

#include <vector>
#include <ctime>
#include <chrono>
#include <random>
#include <SDL.h>
#include <SDL_image.h>
#include "defs.h"
#include "graphics.h"
#include "structs.h"

mt19937 rd(chrono::steady_clock::now().time_since_epoch().count());

struct Game
{
    vector<MapTile*> allMapTile;

    void genHexagon(float centerX, float centerY)
    {
        MapTile* cur = new MapTile();
        double angle = 0;
        for (int i = 0; i < 6; ++i)
        {
            float x1 = centerX + MAP_HEX_SIZE * cos(angle);
            float y1 = centerY + MAP_HEX_SIZE * sin(angle);
            angle += MAP_HEX_ANGLE;
            cur->points[i] = {x1, y1};
        }
        allMapTile.push_back(cur);
    }

    void initMap()
    {
        srand(time(NULL));

        ///Gen Hexagon
        bool fl = 0;
        for (float x = 0; x < SCREEN_WIDTH; x += MAP_HEX_DISTANCE_HORIZ)
        {
            for (float y = (fl ? MAP_HEX_DISTANCE_VERT / 2 : 0); y < SCREEN_HEIGHT; y += MAP_HEX_DISTANCE_VERT)
            {
                genHexagon(x, y);
            }
            fl = !fl;
        }

        ///Coloring Hexagon

        //color random between heavy and light grass ( heavy grass <= 20)
        int num_heavy = 20;
        while (num_heavy > 0)
        {
            int p = rd() % allMapTile.size();
            allMapTile[p]->colorId = MAP_COLOR_HGRASS;
            num_heavy--;
        }

        //Color the river
        int st1 = 0;
        while (st1 < allMapTile.size())
        {
            allMapTile[st1]->colorId = MAP_COLOR_WATER;
            st1 += 9;
        }
    }

    void drawMap(SDL_Renderer* renderer)
    {

        //Draw lines of hex
        for (const MapTile* hex : allMapTile)
        {
            //set color
            tuple<int, int, int> color = MAP_TILE_COLOR[hex->colorId];
            SDL_SetRenderDrawColor(renderer, std::get<0>(color), std::get<1>(color), std::get<2>(color), 0);

            //fill rect
            SDL_FRect rect;
            rect.x = hex->points[4].first;
            rect.y = hex->points[4].second;
            rect.w = MAP_HEX_SIZE;
            rect.h = MAP_HEX_DISTANCE_VERT;
            SDL_RenderFillRectF(renderer, &rect);

            //fill remain triangle
            colorTriangle(renderer, hex->points[2].first, hex->points[2].second,
                                    hex->points[3].first, hex->points[3].second,
                                    hex->points[4].first, hex->points[4].second);

            colorTriangle(renderer, hex->points[1].first, hex->points[1].second,
                                    hex->points[0].first, hex->points[0].second,
                                    hex->points[5].first, hex->points[5].second);

            //draw boundary
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
            for (int i = 0; i < 6; i++)
            {
                float x1 = hex->points[i].first, y1 = hex->points[i].second;
                float x2 = hex->points[(i + 1) % 6].first, y2 = hex->points[(i + 1) % 6].second;
                SDL_RenderDrawLineF(renderer, x1, y1, x2, y2);
            }
        }
    }

    void draw(Graphics& graphics)
    {
        drawMap(graphics.renderer);
    }
};

#endif // _LOGIC__H
