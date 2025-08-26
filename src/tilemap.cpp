#include "tilemap.h"
#include <stdlib.h>

void tilemapInit(int width, int height, int worldOffsetX, int worldOffsetY, int unitWidth, int unitHeight, TileMap *tileMap) {
    tileMap->width = width;
    tileMap->height = height;
    tileMap->worldOffsetX = worldOffsetX;
    tileMap->worldOffsetY = worldOffsetY;
    tileMap->unitWidth = unitWidth;
    tileMap->unitHeight = unitHeight;
    tileMap->map = (Tile *) malloc(sizeof(Tile) * width * height);

    for (int i = 0; i < width * height; i++) {
        tileMap->map[i] = (Tile) {0, 0, -1};
    }
}

void tilemapFree(TileMap *tileMap) {
    free(tileMap->map);
    tileMap->width = 0;
    tileMap->height = 0;
    tileMap->worldOffsetX = 0;
    tileMap->worldOffsetY = 0;
    tileMap->unitWidth = 0;
    tileMap->unitHeight = 0;
}

int tilemapEntityAtMap(int x, int y, TileMap *tileMap) {
    return tileMap->map[y * tileMap->width + x].entityId;
}
