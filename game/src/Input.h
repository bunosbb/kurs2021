#ifndef _INPUT_H_
#define _INPUT_H_

#include "Project.h"

#include "util.h"

#include <cstddef>

class Input {
public:
    void Update();
    bool IsMouseButtonDown(int key);
    bool IsMouseButtonUp(int key);
    POINT GetButtonDownCoords();

    bool IsKeyDown(int key);
    bool IsKeyUp(int key);
    int GetPressedKey();

    void Clear();

    bool IsExit();
private:
    SDL_Event event;
};

#endif