#include "Image.h"

Image::~Image() {
    delete surf;
}

int Image::GetH() {
    return surf->h;
}

int Image::GetW() {
    return surf->w;
}

void Image::SetAlpha(int alpha) {
    SDL_SetSurfaceAlphaMod(surf, alpha);
}