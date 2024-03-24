#ifndef _GRAPHICS__H
#define _GRAPHICS__H

#include <SDL.h>
#include "defs.h"
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

struct Graphics
{
    SDL_Renderer *renderer;
	SDL_Window *window;

	void init()
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

    void drawHexagon(SDL_Renderer* renderer, float centerX, float centerY)
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White color

        double angle = 0;
        for (int i = 0; i < 6; ++i)
        {
            float x1 = centerX + MAP_HEX_SIZE * cos(angle);
            float y1 = centerY + MAP_HEX_SIZE * sin(angle);
            angle += MAP_HEX_ANGLE;

            float x2 = centerX + MAP_HEX_SIZE * cos(angle);
            float y2 = centerY + MAP_HEX_SIZE * sin(angle);

            SDL_RenderDrawLineF(renderer, x1, y1, x2, y2);
        }
    }

    void prepareScene()
    {
        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        //drawHexagon(renderer, 200, 200);


        //Draw hexagon
        bool fl = 0;
        int cnt = 0;
        for (float x = 0; x < SCREEN_WIDTH; x += MAP_HEX_DISTANCE_HORIZ)
        {
            for (float y = (fl ? MAP_HEX_DISTANCE_VERT / 2 : 0); y < SCREEN_HEIGHT; y += MAP_HEX_DISTANCE_VERT)
            {
                drawHexagon(renderer, x, y);
            }
            fl = !fl;
        }

        //
    }

    void presentScene()
    {
        SDL_RenderPresent(renderer);
    }

    void quit()
    {
        //IMG_Quit();

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
};

#endif // _GRAPHICS__H
