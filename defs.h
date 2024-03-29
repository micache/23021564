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
#define MAP_HEX_DISTANCE_HORIZ MAP_HEX_SIZE * 3 / 2
#define MAP_HEX_DISTANCE_VERT MAP_HEX_SIZE * sqrt(3)
#define MAP_HEX_ANGLE M_PI / 3
#define MAP_HEX_NUMBER 153

const std::tuple<int, int, int> MAP_TILE_COLOR[3] = {{55, 105, 33}, {81, 197, 30}, {16, 87, 204}}; /// 0: heavy grass, 1: grass, 2: water
#define MAP_COLOR_HGRASS 0
#define MAP_COLOR_GRASS 1
#define MAP_COLOR_WATER 2

#define NUM_CLASS 5
const std::string CLASS_NAME[NUM_CLASS] = {"center", "archer", "swordsman", "scout", "knight"};
const int CLASS_HP[NUM_CLASS] = {200, 40, 65, 45, 70};
const int CLASS_DAME[NUM_CLASS] = {10, 7, 4, 2, 6};
const int CLASS_COST[NUM_CLASS] = {0, 3, 1, 2, 5};
const int CLASS_STEP[NUM_CLASS] = {0, 2, 1, 3, 2};

#define ON_MAP_TEXTURE_SIZE 64
#define ON_MENU_TEXTURE_SIZE 150

#endif
