#ifndef _IMAGE_H_
#define _IMAGE_H_

#include "Project.h"

#include "util.h"
class POINT;

class Image {
public:
    ~Image();

    friend class Graphics;

    int GetW();
    int GetH();
    void SetAlpha(int alpha);

private:
    SDL_Surface* surf;
};

#endif