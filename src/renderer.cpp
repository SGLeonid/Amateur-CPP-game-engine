#include <engine/graphics/renderer.hpp>
#include <assert.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

Renderer::Renderer() = default;

Renderer::Renderer(const Window &window, int scale) {
    create(window, scale);
}

Renderer::~Renderer() {
    Debug::log(DEBUG_TRACE, "Cleaning up Renderer");
    
    if (backBuffer) SDL_DestroyTexture(backBuffer);
    if (renderer) SDL_DestroyRenderer(renderer);
}

void Renderer::create(const Window &window, int scale) {
    int w, h;
    
    renderer = SDL_CreateRenderer(
        window.window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    assert(renderer, "Creating renderer failed: " + std::string(SDL_GetError()));

    Debug::log(DEBUG_TRACE, "Created renderer instance");

    SDL_GetWindowSize(window.window, &w, &h);

    backBuffer = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        w / scale,
        h / scale
    );

    assert(backBuffer, "Creating back buffer failed: " + std::string(SDL_GetError()));

    Debug::log(DEBUG_TRACE, "Created back buffer instance");
}

void Renderer::setDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void Renderer::clear() {
    SDL_RenderClear(renderer);
}

void Renderer::present() {
    SDL_SetRenderTarget(renderer, nullptr);
    SDL_RenderCopy(renderer, backBuffer, nullptr, nullptr);
    SDL_RenderPresent(renderer);
    SDL_SetRenderTarget(renderer, backBuffer);
}

void Renderer::loadTexture(Texture &texture, const char *fileName) {
    SDL_Surface *source = IMG_Load(fileName);
    if (source == nullptr) {
        Debug::log(DEBUG_ERROR, "Cannot load image file: " + std::string(IMG_GetError()));
        texture.texture = nullptr;
        return;
    }
    
    texture.texture = SDL_CreateTextureFromSurface(renderer, source);
    if (texture.texture == nullptr) {
        Debug::log(DEBUG_ERROR, "Cannot create texture from image: " + std::string(SDL_GetError()));
        return;
    }

    SDL_FreeSurface(source);

    Debug::log(DEBUG_TRACE, "Created texture from file " + std::string(fileName));
}

Vector2i Renderer::queryTextureSize(const Texture &texture) {
    Vector2i size;
    SDL_QueryTexture(texture.texture, nullptr, nullptr, &size.x, &size.y);
    return size;
}

void Renderer::drawTexture(const Texture &texture) {
    SDL_RenderCopy(renderer, texture.texture, nullptr, nullptr);
}

void Renderer::drawTexture(const Texture &texture, const Recti &dst) {
    SDL_RenderCopy(renderer, texture.texture, nullptr, (SDL_Rect*)&dst);
}

void Renderer::drawTexture(const Texture &texture, const Recti &src, const Recti &dst) {
    SDL_RenderCopy(renderer, texture.texture, (SDL_Rect*)&src, (SDL_Rect*)&dst);
}

void Renderer::destroyTexture(const Texture &texture) {
    SDL_DestroyTexture(texture.texture);
}