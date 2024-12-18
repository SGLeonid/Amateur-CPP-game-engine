#ifndef ENGINE_RENDERER_H
#define ENGINE_RENDERER_H

#include <engine/window/window.hpp>
#include <engine/graphics/texture.hpp>
#include <engine/graphics/rect.hpp>
#include <engine/math/vector2.hpp>

class Renderer {
public:
    Renderer();
    Renderer(const Window &window, int scale = 1);
    ~Renderer();

    void create(const Window &window, int scale = 1);

    void setDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    void clear();
    void present();

    template <typename T>
    void drawLine(const Vector2T<T> &a, const Vector2T<T> &b) {
        SDL_RenderDrawLine(renderer, a.x, a.y, b.x, b.y);
    }

    template <typename T>
    void drawRect(const RectT<T> &rect) {
        Recti recti = Recti(rect);
        SDL_RenderDrawRect(renderer, (SDL_Rect*)&recti);
    }

    template <typename T>
    void drawFilledRect(const RectT<T> &rect) {
        Recti recti = Recti(rect);
        SDL_RenderFillRect(renderer, (SDL_Rect*)&recti);
    }

    void loadTexture(Texture &texture, const char *fileName);
    Vector2i queryTextureSize(const Texture &texture);
    void drawTexture(const Texture &texture);
    void drawTexture(const Texture &texture, const Recti &dst);
    void drawTexture(const Texture &texture, const Recti &src, const Recti &dst);
    void destroyTexture(const Texture &texture);

private:
    SDL_Renderer *renderer;
    SDL_Texture *backBuffer;
};

#endif