#include <iostream>
#include "Project.h"
class Logger;

#pragma warning(disable: 4996)


int main(int argc, char** args) {


    SDL_Init(SDL_INIT_EVERYTHING);
    /*if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
        std::cout << Mix_GetError() << " - on OpenAudio\n";
    }
    Mix_AllocateChannels(10);*/

    SDL_DisplayMode screen;
    SDL_GetCurrentDisplayMode(0, &screen);
    Object config;
    std::cout << "Try ot read config\n" << std::endl;
    config.Read("../src/config.json");
    std::cout << "Try ot create Game\n" << std::endl;
    Game game(config);
    std::cout << "Try ot execute\n" << std::endl;

    //game.Execute(screen.w, screen.h, 0);
    game.Execute(640, 480, 0);
    //Mix_Quit();
    SDL_Quit();
    return 0;
}