#ifndef ENGINE_ECS_COMPONENT_H
#define ENGINE_ECS_COMPONENT_H

#define ECS_MAX_COMPONENTS 16

#include <engine/memory/object.hpp>

typedef struct _EcsComponentS EcsComponentS;
typedef PooledObject<EcsComponentS> EcsComponent;

#include <engine/ecs/entity.hpp>

struct _EcsComponentS {
    _EcsComponentS() : id(-1), active(false) {}
    _EcsComponentS(int _id, bool _active) : id(_id), active(_active) {}
    ~_EcsComponentS() {}

private:
    int id;
    bool active;
    EcsEntity *entity;

public:
    void setActive(bool _active) { active = _active; }

    int getId() { return id; }
    bool isActive() { return active; }
    EcsEntity *getEntity() { return entity; }

    friend class EcsRegistry;
};


#endif