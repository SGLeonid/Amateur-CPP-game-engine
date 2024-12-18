#include <engine/ecs/registry.hpp>
#include <engine/ecs/system.hpp>

#include <engine/engine.hpp>
#include <assert.hpp>

EcsRegistry::EcsRegistry() : entities(), systems() {
    Debug::log(DEBUG_TRACE, "Created ECS registry");
}

EcsRegistry::~EcsRegistry() {
    Debug::log(DEBUG_TRACE, "Cleaning up EcsRegistry");

    for (auto &system : systems) {
        if (system) {
            system -> destroy();
        }
    }

    for (auto &entity : entities) {
        if (entity) {
            deleteEntity(entity -> id);
        }
    }
}

int EcsRegistry::createEntity(const char *tag, int parent) {
    int id;
    EcsEntity *entity = new EcsEntity(true, tag, nullptr);
    id = addEntity(entity);
    setEntityParent(id, parent);

    return id;
}

int EcsRegistry::addEntity(EcsEntity *entity) {
    static int newId = 0;

    entity -> id = newId;

    for (auto &component : entity -> components) {
        component = nullptr;
    }

    entities.push_back(entity);

    Debug::log(DEBUG_TRACE, "Created entitiy with ID: " + std::to_string(newId));

    return newId++;
}

void EcsRegistry::deleteEntity(int id) {
    static int level = 0;

    for (auto &component : entities[id] -> components) {
        if (component) {
            delete component;
        }
    }

    if (!(entities[id] -> children.empty())) {
        
        level++;
        for (auto &child : entities[id] -> children) {
            deleteEntity(child -> id);
        }
        level--;
        entities[id] -> children.clear();
    }

    EcsEntity *parent = entities[id] -> parent;
    if (level == 0 && parent) {
        parent -> children.remove(entities[id]);
    }

    delete entities[id];
    entities[id] = nullptr;

    Debug::log(DEBUG_TRACE, "Deleted entity: " + std::to_string(id));
}

void EcsRegistry::setEntityParent(int id, int parent) {
    entities[id] -> parent = entities[parent];
    entities[parent] -> children.push_back(entities[id]);
}

int EcsRegistry::getEntityParent(int id) {
    return entities[id] -> parent -> id;
}

void EcsRegistry::setEntityActive(int id, bool active) {
    entities[id] -> active = active;
}

bool EcsRegistry::isEntityActive(int id) {
    return entities[id] -> active;
}

std::list<EcsEntity*> EcsRegistry::getAllChildren(int id) {
    return entities[id] -> children;
}

int EcsRegistry::generateComponentTypeId() {
    static int id = 0;
    return id++;
}

void EcsRegistry::addSystem(EcsSystem *system) {
    system -> setRegistryPtr(this);
    systems.push_back(system);

    Debug::log(DEBUG_TRACE, "Registered system at " + std::to_string((uint64_t)system));
}

void EcsRegistry::start() {
    for (auto &system : systems) {
        if (system) {
            system -> start();
        }
    }

    for (auto &entity : entities) {
        if (entity) {
            entity -> start();
        }
    }
}

void EcsRegistry::update(double deltaTime) {
    for (auto &entity : entities) {
        if (entity) {
            entity -> update(deltaTime);
        }
    }

    for (auto &system : systems) {
        if (system) {
            system -> update(deltaTime);
        }
    }
}