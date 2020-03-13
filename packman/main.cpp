

#define SDL_MAIN_HANDLED
#include "game.h"
int main() {
    SDL_Init(SDL_INIT_EVERYTHING);
    Game game;
    game.loop();
    return 0;
}



