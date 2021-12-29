#include "util.h"

POINT::POINT() {
    x = 0;
    y = 0;
}

POINT::POINT(int n_x, int n_y){
    x = n_x;
    y = n_y;
}

POINT::operator std::pair<int32_t, int32_t> () {
    return std::make_pair(x, y);
}