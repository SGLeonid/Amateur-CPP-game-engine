#ifndef BASE_MATH_H
#define BASE_MATH_H

#include <cmath>

template<typename T>
inline T lerp(const T &a, const T &b, float t) {
    return a + ((b - a) * t);
}

template<typename T>
inline T clamp(const T &a, const T &min, const T &max) {
    return a > min ? (a < max ? a : max) : min;
}

#include <engine/math/vector2.hpp>
// #include <vector3.hpp>
// #include <vector4.hpp>

template <typename T>
inline T slerp(T &a, T &b, float t) {
    float d = dot(a.normalized(), b.normalized());
    if ((d > 0.999999) || (d < -0.999999)) {
        if (t <= 0.5) {
            return a;
        }
        return b;
    }

    float angle = acos(d);
    return ((a * sin((1 - t) * angle) + b * sin(t * angle)) / sin(angle));
}

#endif