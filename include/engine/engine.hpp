#ifndef ENGINE_H
#define ENGINE_H

#include <engine/window/window.hpp>
#include <engine/window/windowevent.hpp>
#include <engine/graphics/renderer.hpp>

class Engine {
public:
    Engine(const Engine &engine) = delete;
    void operator=(const Engine &engine) = delete;

    int run(int argc, const char *argv[]);
    void shutdown();

    Window *getWindow();
    Renderer *getRenderer();
    double getDeltaTime();

    Vector2f getKeyboardAxis();
    Vector2f getMouseAxis();

    static Engine &getInstance();

private:
    Engine();
    ~Engine();

    Window window;
    Renderer renderer;
    WindowEvent event;

    Vector2f keyboardAxis;
    Vector2f mouseAxis;

    double deltaTime;
    bool running;
};

#endif