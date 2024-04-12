#ifndef _GRAPHICS__H
#define _GRAPHICS__H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <iostream>
#include <cmath>
#include <algorithm>
#include <string>

#include "defs.h"
#include "utils.h"
#include "structs.h"

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

	void logErrorAndExit(const char* msg, const char* error)
    {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", msg, error);
        SDL_Quit();
    }

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

        if (TTF_Init() == -1)
        {
            cout << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError();
        }

        if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
        {
            logErrorAndExit( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
        }

    }

    SDL_Texture *loadTexture(const char *filename)
    {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);

        SDL_Texture *texture = IMG_LoadTexture(renderer, filename);
        if (texture == NULL)
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Load texture %s", IMG_GetError());

        return texture;
    }

    void renderTexture(SDL_Texture *texture, int x, int y, int w, int h, SDL_Renderer* renderer)
    {
        SDL_Rect dest;
        dest.x = x;
        dest.y = y;
        dest.w = w;
        dest.h = h;

        SDL_RenderCopy(renderer, texture, NULL, &dest);
    }

    TTF_Font* loadFont(const char* path, int length)
    {
        TTF_Font* gFont = TTF_OpenFont( path, length );
        if (gFont == nullptr)
        {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                           SDL_LOG_PRIORITY_ERROR,
                           "Load font %s", TTF_GetError());
        }
        return gFont;
    }

    SDL_Texture* renderText(const char* text, TTF_Font* font, SDL_Color textColor)
    {
        SDL_Surface* textSurface = TTF_RenderText_Solid( font, text, textColor );
        if ( textSurface == nullptr )
        {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                           SDL_LOG_PRIORITY_ERROR,
                           "Render text surface %s", TTF_GetError());
            return nullptr;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface( renderer, textSurface );
        if( texture == nullptr )
        {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                           SDL_LOG_PRIORITY_ERROR,
                           "Create texture from text %s", SDL_GetError());
        }
        SDL_FreeSurface( textSurface );
        return texture;
    }

    void showText(string fontPath, int size, string text, int x, int y, int w, int h)
    {
        TTF_Font* font = loadFont(fontPath.c_str(), size);
        SDL_Color color = {255, 255, 0, 0};
        SDL_Texture* Text = renderText(text.c_str(), font, color);
        renderTexture(Text, x, y, w, h, renderer);
    }

    void renderAnim(int x, int y, Sprite* sprite, bool flip)
    {
        const SDL_Rect* clip = sprite->getCurrentClip();
        SDL_Rect renderQuad = {x, y, ON_MAP_TEXTURE_SIZE, ON_MAP_TEXTURE_SIZE};
        if (!flip)
            SDL_RenderCopy(renderer, sprite->texture, clip, &renderQuad);
        else
            SDL_RenderCopyEx(renderer, sprite->texture, clip, &renderQuad, 0, NULL, SDL_FLIP_HORIZONTAL);
    }

    Mix_Music *loadMusic(const char* path)
    {
        Mix_Music *gMusic = Mix_LoadMUS(path);
        if (gMusic == nullptr)
        {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                           SDL_LOG_PRIORITY_ERROR,
                "Could not load music! SDL_mixer Error: %s", Mix_GetError());
        }
        return gMusic;
    }

    void play(Mix_Music *gMusic)
    {
        if (gMusic == nullptr) return;

        if (Mix_PlayingMusic() == 0)
        {
            Mix_PlayMusic( gMusic, -1 );
        }
        else if( Mix_PausedMusic() == 1 )
        {
            Mix_ResumeMusic();
        }
    }

    Mix_Chunk* loadSound(const char* path)
    {
        Mix_Chunk* gChunk = Mix_LoadWAV(path);
        if (gChunk == nullptr)
        {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                           SDL_LOG_PRIORITY_ERROR,
               "Could not load sound! SDL_mixer Error: %s", Mix_GetError());
        }
    }
    void play(Mix_Chunk* gChunk) {
        if (gChunk != nullptr) {
            Mix_PlayChannel( -1, gChunk, 0 );
        }
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
        SDL_RenderPresent(renderer);
    }

    void quit()
    {
        Mix_Quit();
        TTF_Quit();
        IMG_Quit();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        //SDL_Quit();
    }
};

#endif // _GRAPHICS__H
