#ifndef ENGINE_TRANSFORM_COMPONENT_H
#define ENGINE_TRANSFORM_COMPONENT_H

#include <engine/ecs/component.hpp>
#include <engine/memory/object.hpp>
#include <engine/math/vector2.hpp>

typedef struct TransformComponentS : public EcsComponent {
    TransformComponentS() : EcsComponent(), position(0, 0) {}
    TransformComponentS(int _id, bool _active, Vector2f _position = Vector2f(0.0f, 0.0f)) : EcsComponent(_id, _active), position(_position) {}
    ~TransformComponentS() = default;

    Vector2i position;
} TransformComponentS;

typedef PooledObject<TransformComponentS> TransformComponent;

#endif