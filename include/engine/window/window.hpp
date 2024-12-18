#ifndef ENGINE_WINDOW_H
#define ENGINE_WINDOW_H

#include <engine/window/windowevent.hpp>

class Window {
public:
    Window();
    Window(const char *title, int w = -1, int h = -1, uint32_t flags = 0);
    ~Window();

    void create(const char *title, int w = -1, int h = -1, uint32_t flags = 0);
    int pollEvent(const WindowEvent &event);

    friend class Renderer;

private:
    SDL_Window *window;
};

#endif