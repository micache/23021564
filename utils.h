#ifndef _UTILS__H
#define _UTILS__H

#include <string>

#include "defs.h"
#include "structs.h"

using namespace std;

bool isInTriangle(float x, float y, float x1, float y1, float x2, float y2, float x3, float y3)
{
    float b1 = (x2 - x1) * (y - y1) - (x - x1) * (y2 - y1);
    float b2 = (x3 - x2) * (y - y2) - (x - x2) * (y3 - y2);
    float b3 = (x1 - x3) * (y - y3) - (x - x3) * (y1 - y3);

    if ((b1 >= 0 && b2 >= 0 && b3 >= 0) || (b1 <= 0 && b2 <= 0 && b3 <= 0))
    {
        return 1;
    }
    return 0;
}

bool isInRect(int x, int y, int u, int v, int h, int w)
{
    return (u <= x && x <= u + w - 1 && v <= y && y <= v + h - 1);
}

float distEuclid(pair<float, float> x1, pair<float, float> x2)
{
    return sqrt((x2.first - x1.first) * (x2.first - x1.first) + (x2.second - x1.second) * (x2.second - x1.second));
}

int numSteps(float dist)
{
    for (int i = 0; i < 4; i++)
    {
        if (dist <= DIST_BETWEEN_TILE[i])
            return i;
    }
    return INF;
}

string convertToString(int id)
{
    string tname = "NAME: " + CLASS_NAME[id];
    string thp = "HP: " + to_string(CLASS_HP[id]);
    string tdame = "DAMAGE: " + to_string(CLASS_DAME[id]);
    string tstep = "STEPS PER TURN: " + to_string(CLASS_STEP[id]);
    return tname + "\n" + thp + "\n" + tdame + "\n" + tstep;
}

//for the minimax algorithm
bool gameOver(const vector<Unit*>& units)
{
    return (units[0]->hp <= 0 || units[1]->hp <= 0);
}

ld eval(const vector<Unit*>& units)
{
    if (units[0]->hp <= 0)
        return INF;
    if (units[1]->hp <= 0)
        return -INF;
    for (auto& u : units)
    {
        if (u->player)
        {

        }
    }
}

#endif
