#ifndef _INPUT__H
#define _INPUT__H

#include <SDL.h>

struct Input
{
    pair<int, int> getMousePos()
    {
        while (1)
        {
            SDL_Event event;
            SDL_PollEvent(&event);

            switch (event.type)
            {
                case SDL_QUIT:
                    exit(0);
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    return {x, y};
            }
        }
    }
};
#endif //_INPUT__H
