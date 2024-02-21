#include <SDL.h>
#include <iostream>

int main(int argv, char** args) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create a window
    SDL_Window* window = SDL_CreateWindow("Simple SDL Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Create a renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Set up the player
    SDL_Rect playerRect = {400, 300, 50, 50};
    int playerSpeed = 5;

    // Main game loop
    bool quit = false;
    SDL_Event event;

    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        const Uint8* keys = SDL_GetKeyboardState(nullptr);
        if (keys[SDL_SCANCODE_LEFT] && playerRect.x > 0) {
            playerRect.x -= playerSpeed;
        }
        if (keys[SDL_SCANCODE_RIGHT] && playerRect.x < 800 - playerRect.w) {
            playerRect.x += playerSpeed;
        }
        if (keys[SDL_SCANCODE_UP] && playerRect.y > 0) {
            playerRect.y -= playerSpeed;
        }
        if (keys[SDL_SCANCODE_DOWN] && playerRect.y < 600 - playerRect.h) {
            playerRect.y += playerSpeed;
        }

        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Draw the player
        SDL_SetRenderDrawColor(renderer, 0, 128, 255, 255);
        SDL_RenderFillRect(renderer, &playerRect);

        // Update the screen
        SDL_RenderPresent(renderer);

        // Cap the frame rate
        SDL_Delay(16);
    }

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
