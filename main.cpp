#include<iostream>
#include <SDL.h>
#include "graphics.h"
#include "logic.h"

using namespace std;

int main(int argc, char* argv[]) {
    //srand(time(0));

    Graphics graphics;
    graphics.init("aoe parody", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_INPUT_FOCUS);

    Game game;
    game.init(graphics);

    bool t = game.showMenu(graphics);
    bool turn = 0;
    while (1)
    {
        game.initTurn(turn, graphics);

        if (turn && t)
            game.botPlay(graphics);
        else
            game.playTurn(graphics);

        turn = !turn;
    }

    graphics.quit();
    return 0;
}
