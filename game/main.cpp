#include <iostream>
#include "Project.h"
class Logger;

int main(int argc, char** args) {
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_DisplayMode screen;
    SDL_GetCurrentDisplayMode(0, &screen);
    Object* config = new Object;
    std::cout << "Try ot read config" << std::endl;
    config->Read("../src/config.json");
    std::cout << "Try ot create Game" << std::endl;
    Game game(config);
    std::cout << "Try ot execute" << std::endl;

    //game.Execute(screen.w, screen.h, 0);
    game.Execute(640, 480, 0);
    //Mix_Quit();
    SDL_Quit();
    delete config;
    return 0;
}