#ifndef _UTIL_H_
#define _UTIL_H_

#include "Project.h"

class POINT {
public:
    POINT();
    POINT(int n_x, int n_y);
    operator std::pair<int32_t, int32_t>();

    int32_t x;
    int32_t y;
};

#endif
