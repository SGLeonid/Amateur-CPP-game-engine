#ifndef ENGINE_TILEMAP_COMPONENT_HPP
#define ENGINE_TILEMAP_COMPONENT_HPP

#include <ecs.hpp>
#include <engine/graphics/texture.hpp>

class TilemapComponentS : public EcsComponent {
public:
    TilemapComponentS() : EcsComponent() {}
    TilemapComponentS(int _id, bool _active, const char *atlas, int _tileSize, int _level, std::vector<int> _tilemap, int _width, int _height) : EcsComponent(_id, _active) {}
    ~TilemapComponentS() {}

    int tileSize;
    int width;
    int height;
    int level;

    const char *atlasName;
    Texture atlasTexture;

    std::vector<int> tilemap;
    std::vector<Texture*> tiles;
};

#endif