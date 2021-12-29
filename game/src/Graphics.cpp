#include "Graphics.h"

Graphics::Graphics(int new_w, int new_h) {
    w = new_w;
    h = new_h;
    SDL_Init(SDL_INIT_EVERYTHING);
    wind = SDL_CreateWindow("lab", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
    //wind = SDL_CreateWindow("lab", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_FULLSCREEN);
}

Image* Graphics::NewImage(const char* file) {
    Image* img = new Image();
    img->surf = SDL_LoadBMP(file);
    return img;
}

bool Graphics::DrawImage(Image* img, int x, int y) {
    if (wind == NULL || img->surf == NULL)
        return false;

    SDL_Rect Area;
    Area.x = x;
    Area.y = y;
    SDL_BlitSurface(img->surf, NULL, SDL_GetWindowSurface(wind), &Area);
    return true;
}

bool Graphics::DrawImage(Image* img, int x, int y, int start_x, int start_y, int end_x, int end_y) {
    if (wind == NULL || img->surf == NULL)
        return false;

    SDL_Rect Area;
    Area.x = x;
    Area.y = y;
    SDL_Rect frag;
    frag.x = start_x;
    frag.y = start_y;
    frag.w = end_x - start_x;
    frag.h = end_y - start_y;
    SDL_BlitSurface(img->surf, &frag, SDL_GetWindowSurface(wind), &Area);
    return true;
}

int Graphics::GetW() {
    return w;
}

int Graphics::GetH() {
    return h;
}

void Graphics::Update() {
    SDL_UpdateWindowSurface(wind);
}

void Graphics::Clear() {
    SDL_Surface* surf = SDL_GetWindowSurface(wind);
    SDL_FillRect(surf, NULL, SDL_MapRGB(surf->format, 0, 0, 0));
}


Unit::Unit() {
    x = 0;
    y = 0;
}

Unit::Unit(int new_x, int new_y) {
    x = new_x;
    y = new_y;
}

Unit::~Unit() {
    std::cout << "delete unit" << std::endl;
    for (auto image : imgs) {
        delete image;
    }

        std::cout <<"delete map img" << std::endl;
    for (auto [_, vector] : str_imgs) {
        for (auto image : vector) {
            delete image;
        }
    }
        std::cout <<"deleted map img" << std::endl;
}

void Unit::AddImage(Image* img) {
    imgs.push_back(img);
}

void Unit::AddImage(Image* img, std::string name) {
    str_imgs[name].push_back(img);
}

Image* Unit::GetImage() {
    size_t tm = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    size_t img_cnt = imgs.size();
    size_t ind = 0;
    tm = (tm / 100) % (2 * img_cnt);
    if (tm > img_cnt - 1) tm = (2 * img_cnt - 1) - tm;
    
    imgs[tm]->SetAlpha(255);
    return imgs[tm];
}

Image* Unit::GetImage(std::string name) {
    if (str_imgs[name].size() == 0) {
        std::cout << "no images for name: " << name << '/n';
        return NULL;
    }
    if (str_imgs[name][0] == NULL) {
        std::cout << "no images for name: " << name << '/n';
        return NULL;
    }
    size_t tm = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    size_t img_cnt = str_imgs[name].size();
    size_t ind = 0;
    tm = (tm / 100) % (2 * img_cnt);
    if (tm > img_cnt - 1) tm = (2 * img_cnt - 1) - tm;

    str_imgs[name][tm]->SetAlpha(255);
    return str_imgs[name][tm];
}

Image* Unit::GetImage(int alpha) {
    size_t tm = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    size_t img_cnt = imgs.size();
    size_t ind = 0;
    tm = (tm / 100) % (2 * img_cnt);
    if (tm > img_cnt - 1) tm = (2 * img_cnt - 1) - tm;

    imgs[tm]->SetAlpha(alpha);
    return imgs[tm];
}

Image* Unit::GetImage(int alpha, std::string name) {
    if (str_imgs[name].size() == 0) {
        std::cout << "no images for name: " << name << '/n';
    }
    if (str_imgs[name][0] == NULL) {
        std::cout << "no images for name: " << name << '/n';
    }
    size_t tm = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    size_t img_cnt = str_imgs[name].size();
    size_t ind = 0;
    tm = (tm / 100) % (2 * img_cnt);
    if (tm > img_cnt - 1) tm = (2 * img_cnt - 1) - tm;

    str_imgs[name][tm]->SetAlpha(alpha);
    return str_imgs[name][tm];
}

Image* Unit::GetImageStatic() {
    return imgs[0];
}

void Unit::Move(int x_ch, int y_ch) {
    x += x_ch;
    y += y_ch;
}

int Unit::GetX() {
    return x;
}
int Unit::GetY() {
    return y;
}