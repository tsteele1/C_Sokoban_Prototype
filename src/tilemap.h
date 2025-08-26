#ifndef TILEMAP_H
#define TILEMAP_H

struct Tile {
    float atlasX;
    float atlasY;
    int entityId;
};

struct TileMap {
    Tile *map;
    int width;
    int height;
    int worldOffsetX;
    int worldOffsetY;
    int unitWidth;
    int unitHeight;
};

void tilemapInit(int width, int height, int worldOffsetX, int worldOffsetY, int unitWidth, int unitHeight, TileMap *map); 
void tilemapFree(TileMap *tileMap); 
int tilemapEntityAtMap(int x, int y, TileMap *tileMap);

#endif
