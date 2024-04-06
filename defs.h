#ifndef _DEFS__H
#define _DEFS__H

#include<tuple>
#include <string>

#define WINDOW_TITLE "AOE parody"
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define MENU_SCREEN_WIDTH 800
#define MENU_SCREEN_HEIGHT 600

#define MAP_HEX_SIZE 50
const int MAP_HEX_DISTANCE_HORIZ = MAP_HEX_SIZE * 3 / 2;
const int MAP_HEX_DISTANCE_VERT = MAP_HEX_SIZE * sqrt(3);
const float MAP_HEX_ANGLE = M_PI / 3;
#define MAP_HEX_NUMBER 153

const std::tuple<int, int, int> MAP_TILE_COLOR[3] = {{55, 105, 33}, {81, 197, 30}, {16, 87, 204}}; /// 0: heavy grass, 1: grass, 2: water
#define MAP_COLOR_HGRASS 0
#define MAP_COLOR_GRASS 1
#define MAP_COLOR_WATER 2

#define NUM_CLASS 5
const std::string CLASS_NAME[NUM_CLASS] = {"center", "archer", "swordsman", "scout", "knight"};
const int CLASS_HP[NUM_CLASS] = {200, 40, 65, 45, 70};
const int CLASS_DAME[NUM_CLASS] = {10, 7, 4, 2, 6};
const int CLASS_COST[NUM_CLASS] = {0, 3, 2, 3, 6};
const int CLASS_STEP[NUM_CLASS] = {0, 2, 1, 3, 2};

#define ON_MAP_TEXTURE_SIZE 54
#define ON_MENU_TEXTURE_SIZE 150

const float DIST_BETWEEN_TILE[4] = {0, 86.4523, 172.905, 259.357};

#define HP_BAR_WIDTH 40
#define HP_BAR_HEIGHT 6

const int INF = 2e9 + 5;

const int ARCHER_ATK_CLIP[][4] = {{0, 0, 38, 38},
                                  {38, 0, 38, 38},
                                  {76, 0, 38, 38},
                                  {0, 38, 38, 38},
                                  {38, 38, 38, 38}};
const int ARCHER_ATK_FRAMES = sizeof(ARCHER_ATK_CLIP)/sizeof(int)/4;

const int ARCHER_HIT_CLIP[][4] = {{0, 0, 38, 38},
                                  {38, 0, 38, 38},
                                  {76, 0, 38, 38},
                                  {0, 38, 38, 38}};
const int ARCHER_HIT_FRAMES = sizeof(ARCHER_HIT_CLIP)/sizeof(int)/4;

const int ARCHER_WALK_CLIP[][4] = {{0, 0, 38, 38},
                                  {38, 0, 38, 38},
                                  {76, 0, 38, 38},
                                  {0, 38, 38, 38},
                                  {38, 38, 38, 38},
                                  {76, 38, 38, 38},
                                  {0, 76, 38, 38}};
const int ARCHER_WALK_FRAMES = sizeof(ARCHER_WALK_CLIP)/sizeof(int)/4;

const int SWORDSMAN_WALK_CLIP[][4] = {{0, 64, 64, 64},
                                  {64, 64, 64, 64},
                                  {128, 64, 64, 64},
                                  {192, 64, 64, 64},
                                  {256, 64, 64, 64},
                                  {320, 64, 64, 64},
                                  {384, 64, 64, 64},
                                  {448, 64, 64, 64}};
const int SWORDSMAN_WALK_FRAMES = sizeof(SWORDSMAN_WALK_CLIP)/sizeof(int)/4;

const int SWORDSMAN_ATK_CLIP[][4] = {{0, 320, 64, 64},
                                     {64, 320, 64, 64},
                                     {128, 320, 64, 64}};
const int SWORDSMAN_ATK_FRAMES = sizeof(SWORDSMAN_ATK_CLIP)/sizeof(int)/4;

const int SWORDSMAN_HIT_CLIP[][4] = {{0, 512, 64, 64},
                                     {64, 512, 64, 64}};
const int SWORDSMAN_HIT_FRAMES = sizeof(SWORDSMAN_HIT_CLIP)/sizeof(int)/4;

const int SCOUT_WALK_CLIP[][4] = {{0, 0, 231, 190},
                                  {231, 0, 231, 190},
                                  {231*2, 0, 231, 190},
                                  {231*3, 0, 231, 190},
                                  {231*4, 0, 231, 190},
                                  {231*5, 0, 231, 190},
                                  {231*6, 0, 231, 190},
                                  {231*7, 0, 231, 190}};
const int SCOUT_WALK_FRAMES = sizeof(SCOUT_WALK_CLIP)/sizeof(int)/4;

const int SCOUT_ATK_CLIP[][4] = {{0, 0, 231, 190},
                                  {231, 0, 231, 190},
                                  {231*2, 0, 231, 190},
                                  {231*3, 0, 231, 190},
                                  {231*4, 0, 231, 190},
                                  {231*5, 0, 231, 190},
                                  {231*6, 0, 231, 190},
                                  {231*7, 0, 231, 190}};
const int SCOUT_ATK_FRAMES = sizeof(SCOUT_ATK_CLIP)/sizeof(int)/4;

const int SCOUT_HIT_CLIP[][4] = {{0, 0, 231, 190},
                                  {231, 0, 231, 190},
                                  {231*2, 0, 231, 190},
                                  {231*3, 0, 231, 190}};
const int SCOUT_HIT_FRAMES = sizeof(SCOUT_HIT_CLIP)/sizeof(int)/4;

const int KNIGHT_WALK_CLIP[][4] = {{0, 64, 64, 64},
                                  {64, 64, 64, 64},
                                  {128, 64, 64, 64},
                                  {192, 64, 64, 64},
                                  {256, 64, 64, 64},
                                  {320, 64, 64, 64},
                                  {384, 64, 64, 64},
                                  {448, 64, 64, 64}};
const int KNIGHT_WALK_FRAMES = sizeof(KNIGHT_WALK_CLIP)/sizeof(int)/4;

const int KNIGHT_ATK_CLIP[][4] = {{0, 320, 64, 64},
                                     {64, 320, 64, 64},
                                     {128, 320, 64, 64}};
const int KNIGHT_ATK_FRAMES = sizeof(KNIGHT_ATK_CLIP)/sizeof(int)/4;

const int KNIGHT_HIT_CLIP[][4] = {{0, 512, 64, 64},
                                     {64, 512, 64, 64}};
const int KNIGHT_HIT_FRAMES = sizeof(KNIGHT_HIT_CLIP)/sizeof(int)/4;

#endif
