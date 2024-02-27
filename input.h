#ifndef _INPUT__H
#define _INPUT__H

#include <SDL.h>

struct Input
{
    void doInput(void)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    exit(0);
                    break;

                default:
                    break;
            }
        }
    }
};
#endif //_INPUT__H
