#ifndef BASE_VECTOR2_H
#define BASE_VECTOR2_H

#include <iostream>
#include <cmath>

template <typename T>
class Vector2T {
    public:
    T x, y;

    Vector2T() : x(0), y(0) {}
    Vector2T(T a) : x(a), y(a) {}
    Vector2T(T _x, T _y) : x(_x), y(_y) {}
    Vector2T(const Vector2T &v) : x(v.x), y(v.y) {}

    template<typename U>
    Vector2T(const Vector2T<U> &v) : x((T)v.x), y((T)v.y) {}

    ~Vector2T() {}

    Vector2T &operator=(const Vector2T &a) {
        x = a.x;
        y = a.y;
        return *this;
    }

    Vector2T operator-() { return Vector2T(-x, -y); }

    T &operator[](size_t i) { return *((T*)this + i); }
    T operator[](size_t i) const { return *((T*)this + i); }

    Vector2T operator+(const Vector2T &a) const {
        return Vector2T(x + a.x, y + a.y);
    }

    Vector2T operator-(const Vector2T &a) const {
        return Vector2T(x - a.x, y - a.y);
    }

    Vector2T operator*(T a) const {
        return Vector2T(x * a, y * a);
    }

    Vector2T operator/(T a) const {
        return Vector2T(x / a, y / a);
    }

    Vector2T &operator+=(const Vector2T &a) {
        x += a.x;
        y += a.y;
        return *this;
    }

    Vector2T &operator-=(const Vector2T &a) {
        x -= a.x;
        y -= a.y;
        return *this;
    }

    Vector2T &operator*=(T a) {
        x *= a;
        y *= a;
        return *this;
    }

    Vector2T &operator/=(T a) {
        x /= a;
        y /= a;
        return *this;
    }

    bool operator==(const Vector2T &a) {
        return (x == a.x) && (y == a.y);
    }

    bool operator!=(const Vector2T &a) {
        return !(*this == a);
    }

    T length() { return sqrt(x * x + y * y); }
    T length2() { return (x * x + y * y); }

    void normalize() {
        T l = length();
        x /= l;
        y /= l;
    }

    Vector2T normalized() {
        Vector2T v(*this);
        v.normalize();
        return v;
    }
};

template <typename T>
T dot(const Vector2T<T> &a, const Vector2T<T> &b) {
    return (a.x * b.x + a.y * b.y);
}

template <typename T>
T cross(const Vector2T<T> &a, const Vector2T<T> &b) {
    return (a.x * b.y - a.y * b.x);
}

template <typename T>
std::ostream& operator<<(std::ostream &stream, const Vector2T<T> a) {
    stream << "Vector2 {" << a.x << ", " << a.y << "}";
    return stream;
};

typedef Vector2T<int> Vector2i;
typedef Vector2T<float> Vector2f;
typedef Vector2T<double> Vector2d;

#endif