#ifndef VEC2D_H
#define VEC2D_H

#include <cmath>

struct vec2d {
    double x, y;

    vec2d(double x = 0.0, double y = 0.0) : x(x), y(y) {}

    vec2d operator+(const vec2d& v) const { return vec2d(x + v.x, y + v.y); }
    vec2d operator-(const vec2d& v) const { return vec2d(x - v.x, y - v.y); }
    vec2d operator*(double s) const { return vec2d(x * s, y * s); }
    vec2d operator/(double s) const { return vec2d(x / s, y / s); }

    double dot(const vec2d& v) const { return x * v.x + y * v.y; }
    double magnitude() const { return std::sqrt(x*x + y*y); }
    vec2d normalize() const {
        double mag = magnitude();
        return mag == 0 ? vec2d(0, 0) : vec2d(x / mag, y / mag);
    }
};

#endif
