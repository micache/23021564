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
#include "utils.h"
#include "input.h"

mt19937 rd(chrono::steady_clock::now().time_since_epoch().count());

struct Game
{
    vector<MapTile*> allMapTile;
    vector<Unit*> allUnits;

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
        cur->center = {centerX, centerY};
        allMapTile.push_back(cur);
    }

    void initMap()
    {
        srand(time(NULL));
        allMapTile.clear();

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
        while (st1 < (int)allMapTile.size())
        {
            allMapTile[st1]->colorId = MAP_COLOR_WATER;
            st1 += 9;
        }
    }

    Unit* center[2];

    void initUnits(Graphics& graphics)
    {
        allUnits.clear();

        center[0] = new Unit(0, allMapTile[16], 0, graphics);
        allUnits.push_back(center[0]);

        center[1] = new Unit(0, allMapTile[137], 1, graphics);
        allUnits.push_back(center[1]);
    }

    void init(Graphics& graphics)
    {
        initMap();
        initUnits(graphics);
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

    void drawUnits(bool turn, Graphics& graphics)
    {
        for (auto& unit : allUnits)
        {
            if (unit->curPos == center[turn]->curPos && unit->name != "center")
                continue;
            float cenX = unit->curPos->center.first, cenY = unit->curPos->center.second;
            int sz = unit->name == "center" ? 64 : ON_MAP_TEXTURE_SIZE;
            graphics.renderTexture(unit->texture, cenX - sz / 2, cenY - sz / 2, sz, sz, graphics.renderer);
        }
    }

    void draw(bool turn, Graphics& graphics)
    {
        drawMap(graphics.renderer);
        drawUnits(turn, graphics);
    }

    MapTile* tileClicked(int x, int y)
    {
        for (auto& pos : allMapTile)
        {
            for (int i = 0; i < 6; i++)
            {
                if (isInTriangle(x, y, pos->center.first, pos->center.second, pos->points[i].first, pos->points[i].second, pos->points[(i + 1) % 6].first, pos->points[(i + 1) % 6].second))
                {
                    return pos;
                }
            }
        }
        return NULL;
    }

    Unit* unitOnTile(MapTile* tile)
    {
        for (auto& unit : allUnits)
        {
            if (unit->curPos == tile)
            {
                cerr << "Clicked on " << unit->name << '\n';
                return unit;
            }
        }
        return NULL;
    }

    void removeUnit(Unit* unit)
    {
        for (auto& un : allUnits)
        {
            if (un == unit)
            {
                swap(un, allUnits.back());
                allUnits.pop_back();
                return;
            }
        }
    }

    void Move(Unit* unit)
    {
        Input input;
        while (1)
        {
            pair<int, int> mouse = input.getMousePos();

            MapTile* tile = tileClicked(mouse.first, mouse.second);

            Unit* unitTile = unitOnTile(tile);

            float dist = distEuclid(unit->curPos->center, tile->center);
            cerr << dist << '\n';
            if (unitTile == NULL && numSteps(dist) <= unit->steps)
            {
                cerr << ":D\n";
                unit->curPos = tile;
                return;
            }

            if (unitTile != NULL && unitTile->player != unit->player)
            {
                unit->attack(*unitTile);
                if (unitTile->hp <= 0)
                {
                    removeUnit(unitTile);
                }
                return;
            }
        }
    }

    int inQueue[2] = {-1, -1}, turnLeft[2] = {-1, -1};

    void showClassMenu(bool turn)
    {
        Graphics graphics;
        graphics.init("Class Menu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, MENU_SCREEN_WIDTH, MENU_SCREEN_HEIGHT, SDL_WINDOW_INPUT_FOCUS);

        string filePath = "assets/background_class_menu.jpg";
        SDL_Texture *texture = graphics.loadTexture(filePath.c_str());
        SDL_RenderCopy(graphics.renderer, texture, NULL, NULL);

        string fontPath = "assets/classMenu_font.ttf";
        int x = 25, y = 50;
        SDL_Rect iconPos[5];
        for (int i = 1; i < NUM_CLASS; i++)
        {
            filePath = "assets/" + CLASS_NAME[i] + "_icon_" + std::to_string(turn) + ".png";
            texture = graphics.loadTexture(filePath.c_str());
            graphics.renderTexture(texture, x, y, ON_MENU_TEXTURE_SIZE, ON_MENU_TEXTURE_SIZE, graphics.renderer);
            iconPos[i].x = x;
            iconPos[i].y = y;
            iconPos[i].h = ON_MENU_TEXTURE_SIZE;
            iconPos[i].w = ON_MENU_TEXTURE_SIZE;

            string tname = CLASS_NAME[i];
            string thp = "HP: " + to_string(CLASS_HP[i]);
            string tdame = "DAMAGE: " + to_string(CLASS_DAME[i]);
            string tstep = "STEPS: " + to_string(CLASS_STEP[i]);
            string tcost = "COSTS: " + to_string(CLASS_COST[i]);

            graphics.showText(fontPath, 15, tname, x, y + ON_MENU_TEXTURE_SIZE + 25, 150, 30);
            graphics.showText(fontPath, 15, thp, x, y + ON_MENU_TEXTURE_SIZE + 55, 150, 30);
            graphics.showText(fontPath, 15, tdame, x, y + ON_MENU_TEXTURE_SIZE + 85, 150, 30);
            graphics.showText(fontPath, 15, tstep, x, y + ON_MENU_TEXTURE_SIZE + 115, 150, 30);
            graphics.showText(fontPath, 15, tcost, x, y + ON_MENU_TEXTURE_SIZE + 145, 150, 30);

            x += 50 + ON_MENU_TEXTURE_SIZE;
        }

        string tqueue = "Unit in queue: " + (inQueue[turn] == -1 ? "None" : CLASS_NAME[inQueue[turn]]) + (inQueue[turn] == -1 ? "" : "(" + to_string(turnLeft[turn]) + " turns left)");
        graphics.showText(fontPath, 20, tqueue, 100, 430, 600, 50);

        graphics.presentScene();

        Input input;
        while (1)
        {
            pair<int, int> mousePos = input.getMousePos();
            //cerr << mousePos.first << ' ' << mousePos.second << '\n';
            for (int i = 1; i < NUM_CLASS; i++)
            {
                if (isInRect(mousePos.first, mousePos.second, iconPos[i].x, iconPos[i].y, iconPos[i].h, iconPos[i].w))
                {
                    cerr << "Clicked on " << CLASS_NAME[i] << '\n';
                    if (inQueue[turn] == -1)
                    {
                        inQueue[turn] = i;
                        turnLeft[turn] = CLASS_COST[i];
                    } else
                    {
                        for (auto& unit : allUnits)
                        {
                            if (unit->curPos == center[turn]->curPos && unit->name == CLASS_NAME[i])
                            {
                                graphics.quit();
                                Move(unit);
                                return;
                            }
                        }
                    }
                    graphics.quit();
                    return;
                }
            }
        }

    }

    void playTurn(bool turn, Graphics& graphics)
    {
        turnLeft[turn]--;

        if (inQueue[turn] != -1 && turnLeft[turn] == 0)
        {
            Unit* newUnit = new Unit(inQueue[turn], center[turn]->curPos, turn, graphics);
            allUnits.push_back(newUnit);
            inQueue[turn] = -1;
        }

        Unit* unit;
        Input input;
        do
        {
            pair<int, int> mousePos = input.getMousePos();
            unit = unitOnTile(tileClicked(mousePos.first, mousePos.second));
        }while (unit == NULL || unit->player != turn);

        if (unit->name == "center")
        {
            showClassMenu(turn);
        } else
        {
            Move(unit);
        }
    }
};

#endif // _LOGIC__H
