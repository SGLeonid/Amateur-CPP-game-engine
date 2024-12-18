#ifndef ENGINE_RECT_H
#define ENGINE_RECT_H

#include <SDL2/SDL.h>

#include <engine/math/vector2.hpp>

template <typename T>
class RectT {
    public:
    union {
        struct {
            T x, y, w, h;
        };

        struct {
            Vector2T<T> position;
            Vector2T<T> size;
        };
    };

    RectT() : x(0), y(0), w(0), h(0) {}
    RectT(T _w, T _h) : x(0), y(0), w(_w), h(_h) {}
    RectT(T _x, T _y, T _w, T _h) : x(_x), y(_y), w(_w), h(_h) {}
    RectT(const Vector2T<T> &_position, const Vector2T<T> &_size) : position(_position), size(_size) {}
    RectT(const RectT &r) : x(r.x), y(r.y), w(r.w), h(r.h) {}
    RectT(const SDL_Rect &r) : x(r.x), y(r.y), w(r.w), h(r.h) {}

    template<typename U>
    RectT(const RectT<U> &r) : x((T)r.x), y((T)r.y), w((T)r.w), h((T)r.h) {}
    
    ~RectT() {}

    const RectT &operator=(const RectT &a) {
        x = a.x;
        y = a.y;
        w = a.w;
        h = a.h;
        return *this;
    }

    bool operator==(const RectT &a) {
        return (x == a.x && y == a.y && w == a.w && h == a.h);
    }

    bool operator!=(const RectT &a) {
        return (x != a.x || y != a.y || w != a.w || h != a.h);
    }

    bool collide(const Vector2T<T> &v) {
        return (v.x >= x) && (v.x <= (x + w)) && (v.y >= y) && (v.y <= (y + h));
    }

    bool collide(const RectT &r) {
        return (x <= r.x + r.w) && (x + w >= r.x) && (y <= r.y + r.h) && (y + h >= r.y);
    }

    // Rect difference(const Rect &r);
    // Rect intersect(const Rect &r);
    // Rect combine(const Rect &r);

};

template <typename T>
std::ostream& operator<<(std::ostream &stream, const RectT<T> r) {
    stream << "Rect {" << r.x << ", " << r.y << ", " << r.w << ", " << r.h << "}";
    return stream;
};

typedef RectT<int> Recti;
typedef RectT<float> Rectf;
typedef RectT<double> Rectd;

#endif