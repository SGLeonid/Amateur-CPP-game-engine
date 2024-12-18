#include <engine/engine.hpp>
#include <engine/ecs/systems/rendersystem.hpp>
#include <engine/ecs/components/transformcomponent.hpp>
#include <engine/ecs/components/spritecomponent.hpp>

#include <list>

RenderSystem::RenderSystem() {}

RenderSystem::~RenderSystem() {
    Debug::log(DEBUG_TRACE, "Cleaning up RenderSystem");
}

void RenderSystem::start() {
    Debug::log(DEBUG_TRACE, "Starting RenderSystem");

    renderer = Engine::getInstance().getRenderer();

    for (auto &sprite : registry -> getAllComponents<SpriteComponent>()) {
        renderer -> loadTexture(sprite -> texture, sprite -> path);
        sprite -> rect.size = renderer -> queryTextureSize(sprite -> texture);
    }
}

void RenderSystem::update(double deltaTime) {
    for (auto &sprite : registry -> getAllComponents<SpriteComponent>()) {
        Vector2f absolutePosition(0.0f, 0.0f);

        for (
            EcsEntity *entity = sprite -> getEntity();
            entity != nullptr;
            entity = entity -> getParent()
        ) {
            if (entity -> hasComponent<TransformComponent>()) {
                absolutePosition += entity -> getComponent<TransformComponent>() -> position;
            }
        };

        sprite -> rect.position = absolutePosition + sprite -> pivot;

        renderer -> drawTexture(sprite -> texture, sprite -> rect);
    }
}

void RenderSystem::destroy() {
    for (auto &sprite : registry -> getAllComponents<SpriteComponent>()) {
        renderer -> destroyTexture(sprite -> texture);
    }
}