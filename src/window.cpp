#include <engine/window/window.hpp>
#include <assert.hpp>

Window::Window() = default;

Window::Window(const char *title, int w, int h, uint32_t flags) {
    create(title, w, h, flags);
}

Window::~Window() {
    Debug::log(DEBUG_TRACE, "Cleaning up Window");
    if (window) SDL_DestroyWindow(window);
}

void Window::create(const char *title, int w, int h, uint32_t flags) {
    int status;
    SDL_DisplayMode displayMode;

    if (w == -1 || h == -1) {
        status = SDL_GetCurrentDisplayMode(0, &displayMode);
        if (status != 0) {
            Debug::log(DEBUG_WARN, "Get display mode info failed: " + std::string(SDL_GetError()));
            Debug::log(DEBUG_INFO, "Trying to set default screen mode 1024x768");
            displayMode.w = 1024;
            displayMode.h = 768;
        }

        w = displayMode.w;
        h = displayMode.h;
    }
    
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, flags);
    
    assert(window, "Creating window failed: " + std::string(SDL_GetError()));

    Debug::log(DEBUG_TRACE, "Created window instance");
}

int Window::pollEvent(const WindowEvent &event) {
    return SDL_PollEvent((SDL_Event*)&event);
}