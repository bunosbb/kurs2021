#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include "Project.h"

#include "Image.h"
class Image;

class Unit {
public:
    Unit();
    Unit(int new_x, int new_y);

    ~Unit();

    void AddImage(Image* img);
    void AddImage(Image* img, std::string);
    Image* GetImage();
    Image* GetImage(std::string);
    Image* GetImage(int alpha);
    Image* GetImage(int alpha, std::string);
    Image* GetImageStatic();
    void Move(int x_ch, int y_ch);

    int GetX();
    int GetY();

private:
    std::vector<Image*> imgs;
    std::unordered_map<std::string, std::vector<Image*>> str_imgs;
    int x;
    int y;
};


class Graphics {
public:
    Graphics(int new_w, int new_h);

    Image* NewImage(const char* file);
    //Image* NewImage(char* file, int r, int g, int b);
    bool DrawImage(Image* img, int x, int y);
    bool DrawImage(Image* img, int x, int y, int start_x, int start_y, int end_x, int end_y);

    int GetW();
    int GetH();

    void Update();

    void Clear();
private:
    SDL_Window* wind;
    int w;
    int h;
};
#endif