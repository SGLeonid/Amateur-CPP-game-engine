#ifndef ENGINE_RENDER_SYSTEM_H
#define ENGINE_RENDER_SYSTEM_H

#include <engine/ecs/system.hpp>

class RenderSystem : public EcsSystem {
public:
    RenderSystem();
    ~RenderSystem();

    void start() override;
    void update(double deltaTime) override ;
    void destroy() override;

private:
    Renderer *renderer;
};

#endif