#ifndef ENGINE_ECS_ENTITY_H
#define ENGINE_ECS_ENTITY_H

#include <string>

#include <engine/memory/object.hpp>

typedef struct _EcsEntityS EcsEntityS;
typedef PooledObject<EcsEntityS> EcsEntity;

#include <engine/ecs/component.hpp>

struct _EcsEntityS {
    _EcsEntityS() : id(-1), active(false), tag("DEFAULT"), parent(nullptr), children() {}
    _EcsEntityS(bool _active = true, std::string _tag = "DEFAULT", EcsEntity *_parent = nullptr) : id(-1), active(_active), tag(_tag), parent(_parent), children() {}
    ~_EcsEntityS() {}; 

private:
    int id;
    bool active;
    std::string tag;
    
    EcsEntity *parent;
    std::list<EcsEntity*> children;

    EcsComponent *components[ECS_MAX_COMPONENTS];

public:
    virtual void start() {}
    virtual void update(double deltaTime) {}

    template <typename T> T *getComponent();
    template <typename T> bool hasComponent();

    void setActive(bool _active) { active = _active; }
    void setTag(std::string _tag) { tag = _tag; }

    void setParent(EcsEntity *entity) {
        if (!entity && parent) {
            parent -> children.remove((EcsEntity*)this);
            parent = nullptr;
            return;
        }

        parent = entity;
        entity -> children.push_back((EcsEntity*)this);
    }

    void addChild(EcsEntity *entity) {
        if (entity -> parent) {
            entity -> setParent(nullptr);
        }

        entity -> setParent((EcsEntity*)this);
    }

    int getId() { return id; }
    bool isActive() { return active; }
    std::string getTag() { return tag; }
    EcsEntity *getParent() { return parent; }
    std::list<EcsEntity*> getChildren() { return children; }

    friend class EcsRegistry;
};

#endif