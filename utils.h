#ifndef _UTILS__H
#define _UTILS__H

#include <string>
#include <assert.h>

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
    return ceil(dist / DIST_BETWEEN_TILE);
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

//u2 di truoc
bool defeat(const Unit* u1, const Unit* u2)
{
    int turn1 = (u1->hp + u2->dame - 1) / u2->dame;
    int turn2 = (u2->hp + u1->dame - 1) / u1->dame;
    return turn1 <= turn2;
}

ld eval(const vector<Unit*>& units)
{
    if (units[0]->hp <= 0)
        return INF;
    if (units[1]->hp <= 0)
        return -INF;
    ld res = 0;
    int sumhplost = 0, sumhp = 0;
    for (auto& u : units)
    {
        if (u->name == "center")
            continue;
        if (u->player)
        {
            assert(numSteps(distEuclid(u->curPos->center, units[0]->curPos->center)) > 0);
            res += (ld)1 / numSteps(distEuclid(u->curPos->center, units[0]->curPos->center));
        } else
        {
            sumhplost += CLASS_HP[u->id] - u->hp;
            sumhp += CLASS_HP[u->id];
        }
    }
    int hpcenterlost = max(1, CLASS_HP[0] - units[0]->hp);
    return res - 1 / (hpcenterlost * hpcenterlost) + (sumhp == 0 ? 0 : (ld)sumhplost / sumhp);
}

#endif
