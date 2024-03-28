#ifndef _GRAPHICS__H
#define _GRAPHICS__H

#include <SDL.h>
#include "defs.h"
#include<iostream>

using namespace std;

struct Graphics
{
    SDL_Renderer *renderer;
	SDL_Window *window;

	void initSDL()
    {
        int rendererFlags, windowFlags;

        rendererFlags = SDL_RENDERER_ACCELERATED;

        windowFlags = 0;

        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            cout << "Couldn't initialize SDL: " << SDL_GetError();
            exit(1);
        }

        window = SDL_CreateWindow("aoe 01", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);

        if (!window)
        {
            cout << "Failed to open window: " << SDL_GetError();
            exit(1);
        }

        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

        renderer = SDL_CreateRenderer(window, -1, rendererFlags);

        if (!renderer)
        {
            cout << "Failed to create renderer: " << SDL_GetError();
            exit(1);
        }
    }

    void prepareScene()
    {
        SDL_SetRenderDrawColor(renderer, 96, 128, 255, 255);
        SDL_RenderClear(renderer);
    }

    void presentScene()
    {
        SDL_RenderPresent( renderer);
    }
};

#endif // _GRAPHICS__H
