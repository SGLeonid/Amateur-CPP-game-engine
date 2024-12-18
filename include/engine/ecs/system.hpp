#ifndef ENGINE_ECS_SYSTEM_H
#define ENGINE_ECS_SYSTEM_H

class EcsRegistry;

#include <engine/ecs/registry.hpp>

class EcsSystem {
public:
    EcsSystem();
    ~EcsSystem();

    void setRegistryPtr(EcsRegistry *registry);

    virtual void start() = 0;
    virtual void update(double deltaTime) = 0;
    virtual void destroy() = 0;

protected:
    EcsRegistry *registry;
};

#endif