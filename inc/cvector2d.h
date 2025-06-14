#ifndef CVECTOR2D_H
#define CVECTOR2D_H

#include "vec2d.h"
#include <cmath>

struct CVector2D : public Vec2D {
    CVector2D(double x = 0.0, double y = 0.0) : Vec2D(x, y) {}

    double angle() const { return std::atan2(y, x); }

    void rotate(double radians) {
        double cos_r = std::cos(radians);
        double sin_r = std::sin(radians);
        double new_x = x * cos_r - y * sin_r;
        double new_y = x * sin_r + y * cos_r;
        x = new_x;
        y = new_y;
    }
};

#endif
