#ifndef _UTILS__H
#define _UTILS__H

#include <string>

#include "defs.h"

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

string convertToString(int id)
{
    string tname = "NAME: " + CLASS_NAME[id];
    string thp = "HP: " + to_string(CLASS_HP[id]);
    string tdame = "DAMAGE: " + to_string(CLASS_DAME[id]);
    string tstep = "STEPS PER TURN: " + to_string(CLASS_STEP[id]);
    return tname + "\n" + thp + "\n" + tdame + "\n" + tstep;
}

#endif
