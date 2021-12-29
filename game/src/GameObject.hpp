#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include "Project.h"
#include <optional>

#include "util.h"
class POINT;
class Image;

enum ReflectDirection{
    kRight,
    kLeft,
    kUp,
    kDown,
    kTopRight,
    kTopLeft,
    kBotRight,
    kBotLeft
};

class GameObject {
public:
    GameObject(POINT _boost, POINT _g_boost,
               POINT _pos, Image* _image,
               size_t _w, size_t _h);

    ~GameObject();

    POINT GetBoostedPos() const;

    void Boost();

    void Boost(const ReflectDirection& dir);

    void ReflectBoost(const ReflectDirection& dir);

    POINT GetPos() const;

    int GetH() const;

    void SetBoost(const POINT& add_boost);

    void AddBoost(const GameObject* obj);

    void Move(const POINT& diff);

    Image* GetImage() const;

    std::optional<ReflectDirection> IsIntersec(const GameObject* obj) const;


private:
    POINT* boost;
    POINT* g_boost;
    POINT* pos;
    Image* image;
    POINT* center;
    size_t w;
    size_t h;
    int count_of_tics;
};

#endif