#ifndef VEC2D_H
#define VEC2D_H

#include <cmath>

struct Vec2D {
    double x, y;

    vec2d(double x = 0.0, double y = 0.0) : x(x), y(y) {}

    vec2D operator+(const Vec2D& v) const { return Vec2D(x + v.x, y + v.y); }
    vec2D operator-(const Vec2D& v) const { return Vec2D(x - v.x, y - v.y); }
    vec2D operator*(double s) const { return Vec2D(x * s, y * s); }
    vec2D operator/(double s) const { return Vec2D(x / s, y / s); }

    double dot(const Vec2D& v) const { return x * v.x + y * v.y; }
    double magnitude() const { return std::sqrt(x*x + y*y); }
    Vec2D normalize() const {
        double mag = magnitude();
        return mag == 0 ? Vec2d(0, 0) : Vec2D(x / mag, y / mag);
    }
};

#endif
