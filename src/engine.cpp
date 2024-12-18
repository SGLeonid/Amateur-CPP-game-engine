#include <engine/engine.hpp>
#include <debug.hpp>

#include <engine/ecs/components/transformcomponent.hpp>
#include <engine/ecs/components/spritecomponent.hpp>
#include <engine/ecs/systems/rendersystem.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define DEFAULT_SCREEN_WIDTH 640
#define DEFAULT_SCREEN_HEIGHT 512
#define DEFAULT_PIXEL_SCALE 2
#define DEFAULT_FPS 60
#define DEFAULT_FRAME_TIME (1000 / DEFAULT_FPS)

Engine::Engine() = default;

Engine::~Engine() {
    SDL_Quit();
}

// DEMO PART - PLAYER CLASS (EXAMPLE OF CLASSES THAT DERIVED FROM ECSENTITY
// AND HAVE THEIR OWN BEHAVIOR SO WE CAN IMPLEMENT ANY GAME LOGICS)
// 

class Player : public EcsEntity, public Logger {
public:
    Player() : EcsEntity(true, "PLAYER"), Logger("PLAYER") {}
    ~Player() {}

    void start() {
        log(DEBUG_INFO, "Player entity started activity! :)");

        transform = getComponent<TransformComponent>();
    }

    void update(double deltaTime) {
        transform -> position += Engine::getInstance().getKeyboardAxis() * 64 * deltaTime;
    }

private:
    TransformComponent *transform;
};

typedef PooledObject<Player> PlayerEntity;

// END IF PLAYER CLASS

int Engine::run(int argc, const char *argv[]) {
    (void)argc;
    (void)argv;

    Debug::init(DEBUG_USE_CONSOLE);
    Debug::log(DEBUG_INFO, "Cobra engine pre-alpha v0.20");

    if (running) Debug::log(DEBUG_ERROR, "Attempt to call run method repeatedly");

    int status;
    status = SDL_Init(SDL_INIT_EVERYTHING);
    
    assert(status == 0, "SDL Initialization failed: " + std::string(SDL_GetError()));

    int flags = IMG_INIT_PNG;
    status = IMG_Init(flags);
    
    assert((status & flags) == flags, "SDL IMG initialization failed" + std::string(IMG_GetError()));

    window.create("Engine (Textures)", -1, -1);
    renderer.create(window, DEFAULT_PIXEL_SCALE);
    renderer.setDrawColor(191, 191, 191, 255);

    SDL_ShowCursor(SDL_DISABLE);

    uint64_t startTime = SDL_GetPerformanceCounter();
    uint64_t timerFreq = SDL_GetPerformanceFrequency();
    deltaTime = 0.0f;

    const unsigned char *keys;

    // ECS REGISTRY TEST

    EcsRegistry registry;
    registry.addSystem(new RenderSystem);

    int id = registry.addEntity(new PlayerEntity());
    registry.addComponent<TransformComponent>(id, true, Vector2f(0.0f, 0.0f));
    registry.addComponent<SpriteComponent>(id, true, "assets/player.png", Vector2f(0.0f, 0.0f));

    int childid = registry.createEntity("IMPOSTOR", id);
    registry.addComponent<TransformComponent>(childid, true, Vector2f(32.0f, 32.0f));
    registry.addComponent<SpriteComponent>(childid, true, "assets/player.png", Vector2f(0.0f, 0.0f));

    registry.start();

    // END OF ECS REGISTRY TEST

    running = true;
    while(running) {
        keyboardAxis = Vector2f(0.0f, 0.0f);
        mouseAxis = Vector2f(0.0f, 0.0f);

        keys = SDL_GetKeyboardState(nullptr);

        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                shutdown();
            }

            if (event.type == SDL_KEYDOWN) {
                if (keys[SDL_SCANCODE_ESCAPE]) { shutdown(); }
            }

            if (event.type == SDL_MOUSEMOTION) {
                mouseAxis.x = event.motion.x / DEFAULT_PIXEL_SCALE;
                mouseAxis.y = event.motion.y / DEFAULT_PIXEL_SCALE;
            }
        }

        if (keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP]) { keyboardAxis.y -= 1.0f; }
        if (keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_DOWN]) { keyboardAxis.y += 1.0f; }
        if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT]) { keyboardAxis.x -= 1.0f; }
        if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT]) { keyboardAxis.x += 1.0f; }

        renderer.clear();

        registry.update(deltaTime);

        renderer.present();

        uint64_t endTime = SDL_GetPerformanceCounter(); 
        uint64_t elapsedTime = endTime - startTime;
        deltaTime = (double)elapsedTime / (double)timerFreq;
        startTime = endTime;
    }

    return 0;
}

void Engine::shutdown() {
    running = false;
}

double Engine::getDeltaTime() {
    return deltaTime;
}

Window *Engine::getWindow() {
    return &window;
}

Renderer *Engine::getRenderer() {
    return &renderer;
}

Vector2f Engine::getKeyboardAxis() {
    return keyboardAxis;
}

Vector2f Engine::getMouseAxis() {
    return mouseAxis;
}

Engine &Engine::getInstance() {
    static Engine engine;
    return engine;
}