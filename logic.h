#ifndef _LOGIC__H
#define _LOGIC__H

#include <vector>
#include <ctime>
#include <chrono>
#include <random>
#include <assert.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

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
    bool turn;

    bool showMenu(Graphics& graphics)
    {
        string filePath = "assets/background_menu.png";
        SDL_Texture *texture = graphics.loadTexture(filePath.c_str());
        SDL_RenderCopy(graphics.renderer, texture, NULL, NULL);

        filePath = "assets/1player_button_menu.png";
        texture = graphics.loadTexture(filePath.c_str());
        graphics.renderTexture(texture, 530, 320, 222, 85, graphics.renderer);

        filePath = "assets/2players_button_menu.png";
        texture = graphics.loadTexture(filePath.c_str());
        graphics.renderTexture(texture, 530, 420, 222, 85, graphics.renderer);

        filePath = "assets/quit_button_menu.png";
        texture = graphics.loadTexture(filePath.c_str());
        graphics.renderTexture(texture, 530, 515, 222, 85, graphics.renderer);

        graphics.presentScene();

        Mix_Music* music = graphics.loadMusic("assets/menu_music.mp3");
        graphics.play(music);

        Input input;
        while (1)
        {
            pair<int, int> mousePos = input.getMousePos();
            //1 player
            if (isInRect(mousePos.first, mousePos.second, 530, 320, 85, 222))
            {
                SDL_DestroyTexture(texture);
                Mix_FreeMusic(music);
                return 1;
            }
            //2 players
            if (isInRect(mousePos.first, mousePos.second, 530, 420, 85, 222))
            {
                SDL_DestroyTexture(texture);
                Mix_FreeMusic(music);
                return 0;
            }
            //quit
            if (isInRect(mousePos.first, mousePos.second, 530, 515, 85, 222))
                exit(0);
        }
    }

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

        center[0] = new Unit(0, allMapTile[16], 0, NULL, NULL, NULL);
        center[0]->initTexture(graphics);
        allUnits.push_back(center[0]);

        center[1] = new Unit(0, allMapTile[137], 1, NULL, NULL, NULL);
        center[1]->initTexture(graphics);
        allUnits.push_back(center[1]);
    }

    Sprite unitAnim[4][2][3]; /// 0: walk, 1: atk, 2: hit

    void initAnim(Graphics& graphics)
    {
        for (int t = 0; t < 2; t++)
        {
            //archer
            SDL_Texture* texture = graphics.loadTexture(("assets/archer_walk_sheet_" + std::to_string(t) + ".png").c_str());
            unitAnim[0][t][0].init(texture, ARCHER_WALK_FRAMES, ARCHER_WALK_CLIP);

            texture = graphics.loadTexture(("assets/archer_atk_sheet_" + std::to_string(t) + ".png").c_str());
            unitAnim[0][t][1].init(texture, ARCHER_ATK_FRAMES, ARCHER_ATK_CLIP);

            texture = graphics.loadTexture(("assets/archer_hit_sheet_" + std::to_string(t) + ".png").c_str());
            unitAnim[0][t][2].init(texture, ARCHER_HIT_FRAMES, ARCHER_HIT_CLIP);

            //swordsman
            texture = graphics.loadTexture(("assets/swordsman_sheet_" + std::to_string(t) + ".png").c_str());
            unitAnim[1][t][0].init(texture, SWORDSMAN_WALK_FRAMES, SWORDSMAN_WALK_CLIP);

            texture = graphics.loadTexture(("assets/swordsman_sheet_" + std::to_string(t) + ".png").c_str());
            unitAnim[1][t][1].init(texture, SWORDSMAN_ATK_FRAMES, SWORDSMAN_ATK_CLIP);

            texture = graphics.loadTexture(("assets/swordsman_sheet_" + std::to_string(t) + ".png").c_str());
            unitAnim[1][t][2].init(texture, SWORDSMAN_HIT_FRAMES, SWORDSMAN_HIT_CLIP);

            //scout
            texture = graphics.loadTexture(("assets/scout_walk_sheet_" + std::to_string(t) + ".png").c_str());
            unitAnim[2][t][0].init(texture, SCOUT_WALK_FRAMES, SCOUT_WALK_CLIP);

            texture = graphics.loadTexture(("assets/scout_atk_sheet_" + std::to_string(t) + ".png").c_str());
            unitAnim[2][t][1].init(texture, SCOUT_ATK_FRAMES, SCOUT_ATK_CLIP);

            texture = graphics.loadTexture(("assets/scout_hit_sheet_" + std::to_string(t) + ".png").c_str());
            unitAnim[2][t][2].init(texture, SCOUT_HIT_FRAMES, SCOUT_HIT_CLIP);

            //knight
            texture = graphics.loadTexture(("assets/knight_sheet_" + std::to_string(t) + ".png").c_str());
            unitAnim[3][t][0].init(texture, KNIGHT_WALK_FRAMES, KNIGHT_WALK_CLIP);

            texture = graphics.loadTexture(("assets/knight_sheet_" + std::to_string(t) + ".png").c_str());
            unitAnim[3][t][1].init(texture, KNIGHT_ATK_FRAMES, KNIGHT_ATK_CLIP);

            texture = graphics.loadTexture(("assets/knight_sheet_" + std::to_string(t) + ".png").c_str());
            unitAnim[3][t][2].init(texture, KNIGHT_HIT_FRAMES, KNIGHT_HIT_CLIP);
        }
    }

    void init(Graphics& graphics)
    {
        initMap();
        initUnits(graphics);
        initAnim(graphics);
    }

    void drawMap(SDL_Renderer* renderer)
    {
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

    void drawUnits(Graphics& graphics)
    {
        for (auto& unit : allUnits)
        {
            if (unit->curPos == center[unit->player]->curPos && unit->name != "center")
                continue;
            //draw icon
            float cenX = unit->curPos->center.first, cenY = unit->curPos->center.second;
            int sz = unit->name == "center" ? 64 : ON_MAP_TEXTURE_SIZE;
            graphics.renderTexture(unit->texture, cenX - sz / 2, cenY - sz / 2, sz, sz, graphics.renderer);

            if (!unit->showHp)
                continue;

            //draw hp bar
            SDL_FRect hpBar;
            hpBar.x = cenX - HP_BAR_WIDTH / 2;
            hpBar.y = cenY + sz / 2;
            hpBar.w = HP_BAR_WIDTH;
            hpBar.h = HP_BAR_HEIGHT;

            //Fill all bar with green
            tuple<int, int, int> color = MAP_TILE_COLOR[1];
            SDL_SetRenderDrawColor(graphics.renderer, std::get<0>(color), std::get<1>(color), std::get<2>(color), 0);
            SDL_RenderFillRectF(graphics.renderer, &hpBar);

            //draw the lost hp with red
            SDL_FRect hpLost;
            float percentLost = (1. * CLASS_HP[unit->id] - unit->hp) / CLASS_HP[unit->id];
            hpLost.x = hpBar.x + HP_BAR_WIDTH - percentLost * HP_BAR_WIDTH;
            hpLost.y = hpBar.y;
            hpLost.w = percentLost * HP_BAR_WIDTH;
            hpLost.h = hpBar.h;

            SDL_SetRenderDrawColor(graphics.renderer, 255, 0, 0, 0);
            SDL_RenderFillRectF(graphics.renderer, &hpLost);

            //draw boundary
            SDL_SetRenderDrawColor(graphics.renderer, 0, 0, 0, 0);
            SDL_RenderDrawRectF(graphics.renderer, &hpBar);
        }
    }

    void draw(Graphics& graphics)
    {
        drawMap(graphics.renderer);
        drawUnits(graphics);
    }

    void drawPlayerTurn(Graphics& graphics)
    {
        string filePath = "assets/player_" + std::to_string(turn) + "_turn.png";
        SDL_Texture* texture = graphics.loadTexture(filePath.c_str());
        graphics.prepareScene();
        draw(graphics);
        graphics.renderTexture(texture, SCREEN_WIDTH / 2 - 445 / 2, SCREEN_HEIGHT / 2 - 220 / 2, 445, 219, graphics.renderer);
        graphics.presentScene();

        SDL_Delay(500);

        SDL_DestroyTexture(texture);
    }

    void initTurn(bool _turn, Graphics& graphics)
    {
        turn = _turn;
        for (auto& unit : allUnits)
        {
            if (unit->player == turn)
            {
                unit->steps = CLASS_STEP[unit->id];
            }
        }

        drawPlayerTurn(graphics);
        Mix_Music* music = graphics.loadMusic("assets/background_music.mp3");
        graphics.play(music);
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
                //cerr << "Clicked on " << unit->name << '\n';
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

    void walkAnim(Unit* unit, MapTile* tile, Graphics& graphics)
    {
        const int STEP = 20;
        int x = unit->curPos->center.first - ON_MAP_TEXTURE_SIZE / 2, y = unit->curPos->center.second - ON_MAP_TEXTURE_SIZE / 2;
        float endX = tile->center.first - ON_MAP_TEXTURE_SIZE / 2, endY = tile->center.second - ON_MAP_TEXTURE_SIZE / 2;
        int vx = x < endX ? 1 : -1, vy = y < endY ? 1 : -1;
        int distx = abs(x - endX), disty = abs(y - endY);

        Mix_Chunk *walkSound = graphics.loadSound("assets/walk_sound.mp3");
        graphics.play(walkSound);

        SDL_DestroyTexture(unit->texture);
        //don't show hp bar
        unit->showHp = 0;
        while (distx || disty)
        {
            cerr << distx << ' ' << disty << '\n';
            graphics.prepareScene();
            draw(graphics);
            graphics.renderAnim(x, y, unit->walk, x > endX);
            graphics.presentScene();

            unit->walk->tick();
            if (distx >= STEP)
            {
                x += vx * STEP;
                distx -= STEP;
            }
            if (disty >= STEP)
            {
                y += vy * STEP;
                disty -= STEP;
            }
            if (distx < STEP && disty < STEP)
                break;
        }
        unit->showHp = 1;

        Mix_FreeChunk(walkSound);
        unit->resetAnim();
        unit->initTexture(graphics);
    }

    void atkAnim(Unit* unit, Graphics& graphics, MapTile* tile)
    {
        int x = unit->curPos->center.first - ON_MAP_TEXTURE_SIZE / 2, y = unit->curPos->center.second - ON_MAP_TEXTURE_SIZE / 2;

        Mix_Chunk *atkSound = graphics.loadSound(("assets/" + unit->name + "_atk_sound.mp3").c_str());
        graphics.play(atkSound);

        SDL_DestroyTexture(unit->texture);
        do
        {
            graphics.prepareScene();
            draw(graphics);
            graphics.renderAnim(x, y, unit->atk, unit->curPos->center.first > tile->center.first);
            graphics.presentScene();

            unit->atk->tick();
        }
        while(unit->atk->currentFrame != 0);
        SDL_Delay(100);

        Mix_FreeChunk(atkSound);
        unit->resetAnim();
        unit->initTexture(graphics);
    }

    void hitAnim(Unit* unit, Graphics& graphics, MapTile* tile)
    {
        int x = unit->curPos->center.first - ON_MAP_TEXTURE_SIZE / 2, y = unit->curPos->center.second - ON_MAP_TEXTURE_SIZE / 2;

        Mix_Chunk *hitSound = graphics.loadSound("assets/hit_sound.mp3");
        graphics.play(hitSound);

        if (unit->name == "center")
            return;

        SDL_DestroyTexture(unit->texture);
        do
        {
            graphics.prepareScene();
            draw(graphics);
            graphics.renderAnim(x, y, unit->atk, unit->curPos->center.first > tile->center.first);
            graphics.presentScene();

            unit->hit->tick();
        }
        while(unit->hit->currentFrame != 0);
        SDL_Delay(100);

        Mix_FreeChunk(hitSound);
        unit->resetAnim();
        unit->initTexture(graphics);
    }

    MapTile* getPosClicked()
    {
        Input input;
        pair<int, int> mouse = input.getMousePos();
        MapTile* tile = tileClicked(mouse.first, mouse.second);
        return tile;
    }

    void Move(Unit* unit, MapTile* tile, Graphics& graphics)
    {
        Unit* unitTile = unitOnTile(tile);

        float dist = distEuclid(unit->curPos->center, tile->center);
        int steps = numSteps(dist);
        cerr << steps << '\n';

        //walk
        if (unitTile == NULL && steps <= unit->steps)
        {
            cerr << "WALK\n";
            walkAnim(unit, tile, graphics);
            unit->steps -= steps;
            unit->curPos = tile;
            return;
        }

        //attack other unit
        if (unitTile != NULL && steps + 1 <= unit->steps && unitTile->player != unit->player)
        {
            cerr << "ATK\n";
            atkAnim(unit, graphics, unitTile->curPos);
            hitAnim(unitTile, graphics, unitTile->curPos);
            unitTile->hp -= unit->dame;
            unit->steps -= steps + 1;
            if (unitTile->hp <= 0)
            {
                if (unitTile->name == "center")
                {
                    SDL_SetRenderDrawColor(graphics.renderer, 0, 0, 0, 0);
                    string filePath = "assets/winner_" + std::to_string(turn) + ".png";
                    SDL_Texture* texture = graphics.loadTexture(filePath.c_str());
                    SDL_RenderClear(graphics.renderer);
                    graphics.renderTexture(texture, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, graphics.renderer);
                    graphics.presentScene();
                    SDL_Delay(5000);
                    exit(0);
                }
                removeUnit(unitTile);
            }
            return;
        }

        //don't move (click on itself)
        if (steps == 0)
        {
            return;
        }
    }

    void clearTexture(vector<SDL_Texture*>& vc)
    {
        while (!vc.empty())
        {
            SDL_DestroyTexture(vc.back());
            vc.pop_back();
        }
    }

    int inQueue[2] = {-1, -1}, turnLeft[2] = {-1, -1};

    Unit* showClassMenu()
    {
        vector<SDL_Texture*> used;

        Graphics graphics;
        graphics.init("Class Menu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, MENU_SCREEN_WIDTH, MENU_SCREEN_HEIGHT, SDL_WINDOW_INPUT_FOCUS);

        string filePath = "assets/background_class_menu.jpg";
        SDL_Texture *texture = graphics.loadTexture(filePath.c_str());
        used.push_back(texture);
        SDL_RenderCopy(graphics.renderer, texture, NULL, NULL);

        string fontPath = "assets/classMenu_font.ttf";
        int x = 25, y = 50;
        SDL_Rect iconPos[5];
        for (int i = 1; i < NUM_CLASS; i++)
        {
            filePath = "assets/" + CLASS_NAME[i] + "_icon_" + std::to_string(turn) + ".png";
            texture = graphics.loadTexture(filePath.c_str());
            used.push_back(texture);
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

        string tqueue = "unit is being created: ";
        graphics.showText(fontPath, 20, tqueue, 100, 430, 600, 50);
        if (inQueue[turn] != -1)
        {
            filePath = "assets/" + CLASS_NAME[inQueue[turn]] + "_icon_" + std::to_string(turn) + ".png";
            texture = graphics.loadTexture(filePath.c_str());
            used.push_back(texture);
            graphics.renderTexture(texture, 700, 430, 50, 50, graphics.renderer);
        }

        string texist = "unit is finished: ";
        graphics.showText(fontPath, 15, texist, 80, 490, 300, 40);

        int Xleft = 380;
        for (auto& unit : allUnits)
        {
            if (unit->curPos == center[turn]->curPos && unit->name != "center")
            {
                cerr << "Finished: " << unit->name << "\n";
                //graphics.showText(fontPath, 15, unit->name, Xleft, 490, 32, 32);
                unit->initTexture(graphics);
                //used.push_back(unit->texture);
                graphics.renderTexture(unit->texture, Xleft, 490, 40, 40, graphics.renderer);
                Xleft += 42;
            }
        }

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
                    }
                    else
                    {
                        for (auto& unit : allUnits)
                        {
                            if (unit->curPos == center[turn]->curPos && unit->name == CLASS_NAME[i])
                            {
                                clearTexture(used);
                                graphics.quit();
                                return unit;
                            }
                        }
                    }
                    clearTexture(used);
                    graphics.quit();
                    return NULL;
                }
            }
        }
    }

    void checkNewUnit(Graphics& graphics)
    {
        turnLeft[turn]--;

        if (inQueue[turn] != -1 && turnLeft[turn] == 0)
        {
            Unit* newUnit = new Unit(inQueue[turn], center[turn]->curPos, turn, &unitAnim[inQueue[turn] - 1][turn][0],
                                     &unitAnim[inQueue[turn] - 1][turn][1],
                                     &unitAnim[inQueue[turn] - 1][turn][2]);
            newUnit->initTexture(graphics);
            allUnits.push_back(newUnit);
            inQueue[turn] = -1;

            Mix_Chunk *sound = graphics.loadSound("assets/unit_creation_sound.mp3");
            graphics.play(sound);
            SDL_Delay(1000);
            Mix_FreeChunk(sound);
        }
    }

    void playTurn(Graphics& graphics)
    {
        checkNewUnit(graphics);
        while (1)
        {
            graphics.prepareScene();
            draw(graphics);
            graphics.presentScene();

            Unit* unit;
            Input input;
            do
            {
                pair<int, int> mousePos = input.getMousePos();
                //End turn
                if (mousePos == make_pair(-1, -1))
                {
                    cerr << "End player " << turn << " turn\n";
                    return;
                }

                unit = unitOnTile(tileClicked(mousePos.first, mousePos.second));
            }
            while (unit == NULL || unit->player != turn);

            if (unit->name == "center")
            {
                unit = showClassMenu();
                if (unit != NULL)
                    unit->initTexture(graphics);
            }

            if (unit != NULL)
            {
                Move(unit, getPosClicked(), graphics);
            }
        }
    }

    int cnt = 0;

    void Attp(int i, int dep, ld& alpha, ld& beta, bool player, ld& res, vector<MapTile*>& trace, vector<MapTile*>& best)
    {
        cnt++;
        if (beta <= alpha || cnt > MAX_BRANCH_REACH)
        {
            return;
        }
        if (i == (int)allUnits.size())
        {
            ld val = minimax(dep - 1, alpha, beta, !player);
            if (player)
            {
                if (res < val)
                {
                    best = trace;
                    res = val;
                }
                alpha = max(alpha, val);
            }
            else
            {
                if (res > val)
                {
                    best = trace;
                    res = val;
                }
                beta = min(beta, val);
            }
            return;
        }
        Unit* unit = allUnits[i];
        if (unit->player != player)
        {
            trace.push_back(unit->curPos);
            Attp(i + 1, dep, alpha, beta, player, res, trace, best);
            trace.pop_back();
            return;
        }
        for (const auto& tile : allMapTile)
        {
            if (beta <= alpha || cnt > MAX_BRANCH_REACH)
                return;
            ld dist = distEuclid(unit->curPos->center, tile->center);
            if (numSteps(dist) > unit->steps)
                continue;
            Unit* unitTile = unitOnTile(tile);
            //WALK
            if (unitTile == NULL)
            {
                MapTile* oldtile = unit->curPos;
                unit->curPos = tile;
                trace.push_back(unit->curPos);
                Attp(i + 1, dep, alpha, beta, player, res, trace, best);
                trace.pop_back();
                unit->curPos = oldtile;
            }
            else if (numSteps(dist) + 1 <= unit->steps)
            {
                if (unitTile->player == player)
                {
                    trace.push_back(unit->curPos);
                    Attp(i + 1, dep, alpha, beta, player, res, trace, best);
                    trace.pop_back();
                }
                else
                {
                    int oldhp = unitTile->hp;
                    unitTile->hp -= unit->dame;
                    trace.push_back(unitTile->curPos);
                    Attp(i + 1, dep, alpha, beta, player, res, trace, best);
                    trace.pop_back();
                    unitTile->hp = oldhp;
                }
            }
        }
    }

    vector<MapTile*> bestMove;

    //alpha beta pruning
    ld minimax(int dep, ld alpha, ld beta, bool player)
    {
        if (dep == 0 || gameOver(allUnits))
        {
            return eval(allUnits);
        }
        vector<MapTile*> best;
        ld resv = player ? -INF : INF;
        vector<MapTile*> trace;
        //move unit
        Attp(2, dep, alpha, beta, player, resv, trace, best);

        if (dep == TREE_DEPTH)
            bestMove = best;
        return resv;
    }

    void botPlay(Graphics& graphics)
    {
        checkNewUnit(graphics);

        //create new unit
        if (inQueue[1] == -1)
        {
            int id, minv = INF;
            for (int i = 1; i < 5; i++)
            {
                ld sum = 0;
                int nearest = INF;
                for (auto& v : allUnits)
                {
                    if (!v->player)
                    {
                        int step = numSteps(distEuclid(allUnits[1]->curPos->center, v->curPos->center)) + CLASS_COST[i];
                        if (step < nearest)
                        {
                            nearest = step;
                            sum = (step * step);
                        }
                        if (step == nearest)
                            sum += (step * step);
                    }
                }
                if (minv > sum)
                {
                    minv = sum;
                    id = i;
                }
            }
            inQueue[1] = id;
            turnLeft[1] = CLASS_COST[id];
        }

        cnt = 0;
        random_shuffle(allMapTile.begin(), allMapTile.end());
        cerr << minimax(TREE_DEPTH, -INF, INF, 1) << '\n';

        for (int i = 2; i < (int)allUnits.size(); i++)
        {
            auto unit = allUnits[i];
            if (!unit->player)
                continue;
            Move(unit, bestMove[i - 2], graphics);
        }
        cerr << "End player " << 1 << " turn\n";
    }
};

#endif // _LOGIC__H
