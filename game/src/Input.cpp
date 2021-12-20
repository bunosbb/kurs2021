#include "Input.h"

void Input::Update() {
    SDL_PollEvent(&event);
    //while (!SDL_PollEvent(&event));
}

bool Input::IsMouseButtonDown(int key) {
    return (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == key);
}

bool Input::IsMouseButtonUp(int key) {
    return (event.type == SDL_MOUSEBUTTONUP && event.button.button == key);
}

POINT Input::GetButtonDownCoords() {
    POINT point;

    point.x = event.button.x;
    point.y = event.button.y;

    return point;
}

bool Input::IsKeyDown(int key) {
    /*if (event.type == SDL_KEYDOWN)
        std::cout << event.key.keysym.sym << std::endl;*/
    return (event.type == SDL_KEYDOWN && event.key.keysym.sym == key);
}

bool Input::IsKeyUp(int key) {
    return (event.type == SDL_KEYUP && event.key.keysym.sym == key);
}

int Input::GetPressedKey() {
    return event.key.keysym.sym;
}

void ::Input::Clear() {
    //event.drop;
}

bool Input::IsExit() {
    return (event.type == SDL_QUIT);
}