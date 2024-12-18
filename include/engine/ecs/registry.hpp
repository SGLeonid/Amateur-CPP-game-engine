#ifndef ENGINE_ECS_REGISTRY_H
#define ENGINE_ECS_REGISTRY_H

#include <vector>
#include <list>

class EcsSystem;

#include <engine/ecs/system.hpp>
#include <engine/ecs/entity.hpp>

class EcsRegistry {
public:
    EcsRegistry();
    ~EcsRegistry();

    int createEntity(const char *tag = "DEFAULT", int parent = -1);
    int addEntity(EcsEntity *entity);
    void deleteEntity(int id);
    
    void setEntityParent(int id, int parent);
    int getEntityParent(int id);
    
    void setEntityActive(int id, bool active);
    bool isEntityActive(int id);
    
    std::list<EcsEntity*> getAllChildren(int id);
    // void deleteAllChildren(int id);

    void addSystem(EcsSystem *system);

    template <typename T, typename ...Args> void addComponent(int id, bool active, Args &&... args);
    template <typename T> bool hasComponent(int id);
    template <typename T> void removeComponent(int id);
    template <typename T> T* getComponent(int id);

    template <typename T>
    static int getComponentTypeId() {
        static int id = generateComponentTypeId();
        return id;
    };

    template <typename T> std::list<T*> getAllComponents();

    void start();
    void update(double deltaTime);

private:
    std::vector<EcsEntity*> entities;
    std::vector<EcsSystem*> systems;

    static int generateComponentTypeId();
};

template <typename T>
T *_EcsEntityS::getComponent() {
    int type = EcsRegistry::getComponentTypeId<T>();
    return (T*)components[type];
}

template <typename T>
bool _EcsEntityS::hasComponent() {
    int type = EcsRegistry::getComponentTypeId<T>();
    return components[type] != nullptr;
}

template <typename T, typename ...Args>
void EcsRegistry::addComponent(int id, bool active, Args &&... args) {
    int type = getComponentTypeId<T>();
    T *component = new T(id, active, std::forward<Args>(args)...);
    entities[id] -> components[type] = (EcsComponent*)component;

    component -> entity = entities[id];
}

template <typename T>
bool EcsRegistry::hasComponent(int id) {
    int type = getComponentTypeId<T>();
    return entities[id] -> components[type] != nullptr;
}

template <typename T>
void EcsRegistry::removeComponent(int id) {
    int type = getComponentTypeId<T>();
    PoolAllocator::free(entities[id] -> components[type]);
    entities[id] -> components[type] = nullptr;
}

template <typename T>
T *EcsRegistry::getComponent(int id) {
    int type = getComponentTypeId<T>();
    return (T*)(entities[id] -> components[type]);
}

template <typename T>
std::list<T*> EcsRegistry::getAllComponents() {
    std::list<T*> components;

    for (int id = 0; id < entities.size(); id++) {
        int type = getComponentTypeId<T>();
        EcsComponent *component = entities[id] -> components[type];

        if (component) {
            components.push_back((T*)component);
        }
    }

    return components;
}

#endif