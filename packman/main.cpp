

#define SDL_MAIN_HANDLED //sdl lib miatt

#include "game.h" // a j�t�k
int main() {
    SDL_Init(SDL_INIT_EVERYTHING); //sdl lib miatt

    Game game;
    game.loop();

    return 0;
}



