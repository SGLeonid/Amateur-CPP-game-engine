#ifndef ENGINE_IMAGE_H
#define ENGINE_IMAGE_H

#include <SDL2/SDL.h>

#include <engine/graphics/rect.hpp>

class Texture {
public:
    Texture() = default;
    ~Texture() = default;

private:
    SDL_Texture *texture;

    friend class Renderer;
};

#endif