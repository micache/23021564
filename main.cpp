#ifndef _MAIN__H
#define _MAIN__H
#include <SDL.h>
#include "graphics.h"
#include "input.h"
#include <iostream>

int main(int argv, char** args)
{
    Graphics graphic;
    graphic.initSDL();

    Input input;
    while (1)
    {
        graphic.prepareScene();

        input.doInput();

        graphic.presentScene();

        SDL_Delay(16);
    }
    return 0;
}
#endif // _LOGIC__H
