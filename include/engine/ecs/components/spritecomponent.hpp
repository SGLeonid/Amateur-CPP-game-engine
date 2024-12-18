#ifndef ENGINE_SPRITE_COMPONENT_H
#define ENGINE_SPRITE_COMPONENT_H

#include <engine/ecs/component.hpp>
#include <engine/memory/object.hpp>
#include <engine/graphics/texture.hpp>

typedef struct SpriteComponentS : public EcsComponent {
    SpriteComponentS() : EcsComponent() {};
    SpriteComponentS(int _id, bool _active, const char *_path = nullptr, Vector2f _pivot = Vector2f(0.0f, 0.0f)) : EcsComponent(_id, _active), texture(), pivot(_pivot), path(_path) {};
    ~SpriteComponentS() = default;

    Texture texture;
    Recti rect;
    Vector2f pivot;
    const char *path;
} SpriteComponentS;

typedef PooledObject<SpriteComponentS> SpriteComponent;

#endif