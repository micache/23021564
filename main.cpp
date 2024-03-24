#include<iostream>
#include <SDL.h>
#include "graphics.h"
#include "logic.h"

using namespace std;



int main(int argc, char* argv[]) {
    //srand(time(0));

    Graphics graphics;
    graphics.init();

    Game game;
    game.initMap();

    graphics.prepareScene();
    game.draw(graphics);
    graphics.presentScene();

    SDL_Delay(1000000);

    graphics.quit();
    return 0;
}
