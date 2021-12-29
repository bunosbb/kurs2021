#include "GameObject.hpp"

GameObject::GameObject(POINT _boost, POINT _g_boost,
               POINT _pos, Image* _image,
               size_t _w, size_t _h) :
               image(_image),
               w(_w), h(_h) {
    boost = new POINT(_boost);
    g_boost = new POINT(_g_boost);
    pos = new POINT(_pos);
    center = new POINT (pos->x + w / 2, pos->y + h / 2);
}

GameObject::~GameObject() {
    std::cout << "delete gameobj" << std::endl;
    delete boost;
    delete g_boost;
    delete pos;
    //delete image;
    delete center;
}

POINT GameObject::GetBoostedPos() const {
    POINT boosted_pos;
    boosted_pos.x = pos->x + boost->x + g_boost->x;
    boosted_pos.y = pos->y + boost->y + g_boost->y;

    return boosted_pos;
}

void GameObject::Boost() {
    ++count_of_tics;
    if (count_of_tics >= 30 || std::abs(boost->x + boost->y) == 0) {
        boost->x += g_boost->x;
        boost->y += g_boost->y;
        count_of_tics = 0;
    }
    pos->x += boost->x;
    pos->y += boost->y;
    //std::cout << "boost: " << boost->x << ", " << boost->y << std::endl; 
    return;
}

void GameObject::Boost(const ReflectDirection& dir) {
    ++count_of_tics;
    if (count_of_tics >= 30 || (dir != kUp && std::abs(boost->x + boost->y) == 0)) {
        boost->x += g_boost->x;
        boost->y += g_boost->y;
        count_of_tics = 0;
    }
    pos->x += boost->x;
    pos->y += boost->y;
    std::cout << "boost: " << boost->x << ", " << boost->y << std::endl; 
    return;
}

void GameObject::ReflectBoost(const ReflectDirection& dir) {
    switch (dir) {
    case kRight:
    case kLeft:
        std::cout << "right left" << std::endl;
        boost->x = -boost->x;
        break;
    case kUp:
    case kDown:
        std::cout << "up down" << std::endl;
        boost->y = -boost->y;
        break;
    case kTopRight:
    case kBotLeft:
        std::cout << "top left. bot right" << std::endl;
        std::swap(boost->x, boost->y);
        break;
    case kBotRight:
    case kTopLeft:
        std::cout << "top right. bot left" << std::endl;
        boost->x = -boost->x;
        boost->y = -boost->y;
        std::swap(boost->x, boost->y);
        break;
    }
    return;
}

POINT GameObject::GetPos() const {
    return *pos;
}

int GameObject::GetH() const {
    return h;
}

void GameObject::SetBoost(const POINT& add_boost) {
    boost->x = add_boost.x;
    boost->y = add_boost.y;
    return;
}

void GameObject::AddBoost(const GameObject* obj) {
    boost->x += obj->boost->x;
    boost->y += obj->boost->y;
}

void GameObject::Move(const POINT& diff) {
    pos->x += diff.x;
    pos->y += diff.y;
    return;
}

std::optional<ReflectDirection> GameObject::IsIntersec(const GameObject* obj) const {
    center->x = pos->x + w / 2;
    center->y = pos->y + h / 2;
    if (obj->pos->x <= center->x && center->x <= obj->pos->x + obj->w) {
        if (obj->pos->y + obj->h >= pos->y && pos->y > obj->pos->y) {
            return kUp;
        }
        if (obj->pos->y <= pos->y + h && pos->y + h < obj->pos->y + obj->h) {
            return kDown;
        }
    }
    if (obj->pos->y <= center->y && center->y <= obj->pos->y + obj->h) {
        if (obj->pos->x < pos->x && pos->x <= obj->pos->x + obj->w) {
            return kLeft;
        }
        if (obj->pos->x <= pos->x + w && pos->x + w < obj->pos->x + obj->w) {
            return kRight;
        }
    }
    //corners
    if (obj->pos->x <= pos->x && pos->x <= obj->pos->x + obj->w &&
        obj->pos->y <= pos->y && pos->y <= obj->pos->y + obj->h) {
        return kBotRight;
    }
    if (obj->pos->x <= pos->x + w && pos->x + w <= obj->pos->x + obj->w &&
         obj->pos->y <= pos->y && pos->y <= obj->pos->y + obj->h)  {
        return kBotLeft;
    }
    if (obj->pos->x <= pos->x && pos->x <= obj->pos->x + obj->w &&
         obj->pos->y <= pos->y + h && pos->y + h <= obj->pos->y + obj->h) {
        return kTopRight;
    }
    if (obj->pos->x <= pos->x + w && pos->x + w <= obj->pos->x + obj->w &&
         obj->pos->y <= pos->y + h && pos->y + h <= obj->pos->y + obj->h) {
        return kTopLeft;
    }
    return std::nullopt;
}

Image* GameObject::GetImage() const {
    return image;
}