#ifndef _GRAPHICS__H
#define _GRAPHICS__H

#include <SDL.h>
#include <SDL_image.h>

#include <iostream>
#include <cmath>
#include <algorithm>
#include <string>

#include "defs.h"
#include "utils.h"

void colorTriangle(SDL_Renderer *renderer, float x1, float y1, float x2, float y2, float x3, float y3)
{
    float minX = std::min({x1, x2, x3});
    float maxX = std::max({x1, x2, x3});
    float minY = std::min({y1, y2, y3});
    float maxY = std::max({y1, y2, y3});

    // Fill the triangle by filling rectangles within the bounding box
    for (float y = minY; y <= maxY; y += 0.5)
    {
        for (float x = minX; x <= maxX; x += 0.5)
        {
            // Check if the pofloat (x, y) is inside the triangle
            if (isInTriangle(x, y, x1, y1, x2, y2, x3, y3))
            {
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
    }
}

struct Graphics
{
    SDL_Renderer *renderer;
	SDL_Window *window;

	void init(std::string name, int x, int y, int w, int h, int flag)
    {
        int rendererFlags;

        rendererFlags = SDL_RENDERER_ACCELERATED;

        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            std::cout << "Couldn't initialize SDL: " << SDL_GetError();
            exit(1);
        }

        window = SDL_CreateWindow(name.c_str(), x, y, w, h, flag);

        if (!window)
        {
            std::cout << "Failed to open window: " << SDL_GetError();
            exit(1);
        }

        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

        renderer = SDL_CreateRenderer(window, -1, rendererFlags);

        if (!renderer)
        {
            std::cout << "Failed to create renderer: " << SDL_GetError();
            exit(1);
        }

        if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))
            std::cout << "SDL_image error:" << IMG_GetError();
    }

    SDL_Texture *loadTexture(const char *filename)
    {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);

        SDL_Texture *texture = IMG_LoadTexture(renderer, filename);
        if (texture == NULL)
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Load texture %s", IMG_GetError());

        return texture;
    }

    void renderTexture(SDL_Texture *texture, int x, int y, SDL_Renderer* renderer)
    {
        SDL_Rect dest;
        dest.x = x;
        dest.y = y;
        dest.w = TEXTURE_SIZE;
        dest.h = TEXTURE_SIZE;

        SDL_RenderCopy(renderer, texture, NULL, &dest);
    }


    void prepareScene()
    {
        // Clear the screen with grass color
        auto color = MAP_TILE_COLOR[1];
        SDL_SetRenderDrawColor(renderer, std::get<0>(color), std::get<1>(color), std::get<2>(color), 0);
        SDL_RenderClear(renderer);
    }

    void presentScene()
    {
        SDL_RenderPresent( renderer);
    }

    void quit()
    {
        IMG_Quit();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
};

#endif // _GRAPHICS__H
