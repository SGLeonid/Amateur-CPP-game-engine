#include <engine/ecs/system.hpp>

EcsSystem::EcsSystem() {}

EcsSystem::~EcsSystem() {}

void EcsSystem::setRegistryPtr(EcsRegistry *_registry) {
    registry = _registry;
}