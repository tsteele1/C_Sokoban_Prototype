#include "grid.h"
#include "hole.h"
#include <cstdlib>
#include <iostream>

void gridInit(int width, int height, int worldOffsetX, int worldOffsetY, int unitWidth, int unitHeight, int maxPushDepth, Grid *grid) {
    grid->width = width;
    grid->height = height;
    grid->worldOffsetX = worldOffsetX;
    grid->worldOffsetY = worldOffsetY;
    grid->unitWidth = unitWidth;
    grid->unitHeight = unitHeight;
    grid->map = (int *) malloc(sizeof(int) * width * height);
    grid->pushDepth = maxPushDepth;

    for (int i = 0; i < width * height; i++) {
        grid->map[i] = -1;
    }

}

void gridFree(Grid *grid) {
    free(grid->map);
    grid->width = 0;
    grid->height = 0;
    grid->worldOffsetX = 0;
    grid->worldOffsetY = 0;
    grid->unitWidth = 0;
    grid->unitHeight = 0;
}

void gridUpdateWithECS(ECS *ecs, Grid *grid) {
    for (int i = 0; i < ecs->positionSet.size; i++) {
        int x = ecs->positions[i].x;
        int y = ecs->positions[i].y;
        if (gridGetAtMap(x, y, grid) < 0) {
            continue;
        }

        gridSetFromMap(x, y, ecs->positionSet.dense[i], grid);
    }
}

int gridGetAtWorld(int x, int y, Grid *grid) {
    x = (x - grid->worldOffsetX) / grid->unitWidth;
    y = (y - grid->worldOffsetY) / grid->unitHeight;

    if (!gridInBoundsMap(x, y, grid->width, grid->height)) return -1;

    return grid->map[y * grid->width + x];
}

int gridGetAtMap(int x, int y, Grid *grid) {
    if (!gridInBoundsMap(x, y, grid->width, grid->height)) return -1;

    return grid->map[y * grid->width + x];
}

bool gridInBoundsMap(int x, int y, int width, int height) {
    return y >= 0 && x >= 0 && y < height && x < width;
}

void gridSetFromWorld(int x, int y, int id, Grid *grid) {
    x = (x - grid->worldOffsetX) / grid->unitWidth;
    y = (y - grid->worldOffsetY) / grid->unitHeight;

    if (!gridInBoundsMap(x, y, grid->width, grid->height)) return;

    grid->map[y * grid->width + x] = id;
}

void gridSetFromMap(int x, int y, int id, Grid *grid) {
    if (!gridInBoundsMap(x, y, grid->width, grid->height)) return;

    grid->map[y * grid->width + x] = id;
}

bool gridPushableMap(int x, int y, int dx, int dy, int pushDepth, ECS *ecs, TileMap *tileMap, Grid *grid) {
    // empty space
    bool walkable = gridWalkableMap(x + dx, y + dy, dx, dy, ecs, tileMap, grid);
    if (walkable) return true;

    // wall / OOB
    bool boxAtDest = gridBoxAtMap(x + dx, y + dy, ecs, grid);
    if (!boxAtDest) return false;

    if (grid->pushDepth > 0 && pushDepth >= grid->pushDepth) return false;

    // box is there but can be pushed
    if (gridPushableMap(x + dx, y + dy, dx, dy, pushDepth + 1, ecs, tileMap, grid)) {
        return true;
    }

    // box is there but can't be pushed
    return false;
}


bool gridPushableWorld(int x, int y, int dx, int dy, int pushDepth, ECS *ecs, TileMap *tileMap, Grid *grid) {
    x = (x - grid->worldOffsetX) / grid->unitWidth;
    y = (y - grid->worldOffsetY) / grid->unitHeight;
    dx = dx / grid->unitWidth;
    dy = dy / grid->unitHeight;

    // empty space
    bool walkable = gridWalkableMap(x + dx, y + dy, dx, dy, ecs, tileMap, grid);
    if (walkable) return true;

    // wall / OOB
    bool boxAtDest = gridBoxAtMap(x + dx, y + dy, ecs, grid);
    if (!boxAtDest) {
        return false;
    }

    if (grid->pushDepth > 0 && pushDepth >= grid->pushDepth) return false;

    // box is there but can be pushed
    if (gridPushableMap(x + dx, y + dy, dx, dy, pushDepth + 1, ecs, tileMap, grid)) {
        return true;
    }

    // box is there but can't be pushed
    return false;
}

bool gridWalkableWorld(int x, int y, int dx, int dy, ECS *ecs, TileMap *tileMap, Grid *grid) {
    x = (x - grid->worldOffsetX) / grid->unitWidth;
    y = (y - grid->worldOffsetY) / grid->unitHeight;

    if (!gridInBoundsMap(x, y, grid->width, grid->height)) return false;

    int entityId = grid->map[y * grid->width + x];
    int tileMapEntityId = tilemapEntityAtMap(x, y, tileMap);
    if (entityId < 0 && tileMapEntityId < 0) return true;
    else if (entityId >= 0 && tileMapEntityId < 0) return false;

    int holeIdx = ecs->holeSet.sparse[tileMapEntityId];
    int movingEntityId = grid->map[(y - dy) * grid->width + (x - dx)];
    int movingBoxIdx = ecs->boxSet.sparse[movingEntityId];
    if (holeIdx < 0 || ecs->holes[holeIdx].storedType != NO_BOX || movingBoxIdx >= 0) return true;

    return false;
}

bool gridWalkableMap(int x, int y, int dx, int dy, ECS *ecs, TileMap *tileMap, Grid *grid) {
    if (!gridInBoundsMap(x, y, grid->width, grid->height)) return false;

    int entityId = grid->map[y * grid->width + x];
    int tileMapEntityId = tilemapEntityAtMap(x, y, tileMap);
    if (entityId < 0 && tileMapEntityId < 0) return true;
    else if (entityId >= 0 && tileMapEntityId < 0) return false;

    int holeIdx = ecs->holeSet.sparse[tileMapEntityId];
    int movingEntityId = grid->map[(y - dy) * grid->width + (x - dx)];
    int movingBoxIdx = ecs->boxSet.sparse[movingEntityId];
    if (holeIdx < 0) return true;
    else if (ecs->holes[holeIdx].storedType != NO_BOX) return true;
    else if (movingBoxIdx >= 0) return true;

    return false;
}

bool gridBoxAtMap(int x, int y, ECS *ecs, Grid *grid) {
    if (!gridInBoundsMap(x, y, grid->width, grid->height)) return false;
    else if (grid->map[y * grid->width + x] == -1) return false;

    return ecs->pushSet.sparse[grid->map[y * grid->width + x]] != -1;
}

void gridPushBoxesWorld(int x, int y, int dx, int dy, ECS *ecs, TileMap *tileMap, Grid *grid) {
    x = (x - grid->worldOffsetX) / grid->unitWidth;
    y = (y - grid->worldOffsetY) / grid->unitHeight;

    int i = 0;
    int ox = x;
    int oy = y;
    int dxMap = dx / grid->unitWidth;
    int dyMap = dy / grid->unitHeight;
    int entityId = grid->map[y * grid->width + x];
    int tempId = entityId;

    while (tempId >= 0 && i <= grid->pushDepth) { 
        entityId = tempId;
        int positionId = ecs->positionSet.sparse[entityId];
        int renderId = ecs->renderSet.sparse[entityId];

        ecs->positions[positionId].x += dx;
        ecs->positions[positionId].y += dy;
        ecs->renderers[renderId].spriteOffsetX = -dx * grid->unitWidth;
        ecs->renderers[renderId].savedOffsetX = -dx * grid->unitWidth;
        ecs->renderers[renderId].spriteOffsetY = -dy * grid->unitHeight;
        ecs->renderers[renderId].savedOffsetY = -dy * grid->unitHeight;
        ecs->renderers[renderId].moveAnimTime = 0;
        ecs->renderers[renderId].moveAnimType = WALK;
        ecs->renderers[renderId].maxMoveAnimTime = WALK_TIME;

        x += dxMap;
        y += dyMap;
        tempId = grid->map[y * grid->width + x];
        grid->map[y * grid->width + x] = entityId;
        i++;
    }

    grid->map[oy * grid->width + ox] = -1;
}


void gridPushBoxesMap(int x, int y, int dx, int dy, ECS *ecs, TileMap *tileMap, Grid *grid) {
    int i = 0;
    int ox = x;
    int oy = y;
    int entityId = grid->map[y * grid->width + x];
    int tempId = entityId;

    while (tempId >= 0 && (i <= grid->pushDepth || grid->pushDepth < 0)) { 
        entityId = tempId;
        int positionId = ecs->positionSet.sparse[entityId];
        int renderId = ecs->renderSet.sparse[entityId];

        ecs->positions[positionId].x += dx;
        ecs->positions[positionId].y += dy;
        ecs->renderers[renderId].spriteOffsetX = -dx * grid->unitWidth;
        ecs->renderers[renderId].savedOffsetX = -dx * grid->unitWidth;
        ecs->renderers[renderId].spriteOffsetY = -dy * grid->unitHeight;
        ecs->renderers[renderId].savedOffsetY = -dy * grid->unitHeight;
        ecs->renderers[renderId].moveAnimTime = 0;
        ecs->renderers[renderId].moveAnimType = WALK;
        ecs->renderers[renderId].maxMoveAnimTime = WALK_TIME;

        x += dx;
        y += dy;

        int tileIdAtDest = tilemapEntityAtMap(x, y, tileMap);
        if (tileIdAtDest < 0 || ecs->holeSet.sparse[tileIdAtDest] < 0 || ecs->holes[ecs->holeSet.sparse[tileIdAtDest]].storedType != NO_BOX) {

            tempId = grid->map[y * grid->width + x];
            grid->map[y * grid->width + x] = entityId;
        } else if (tileIdAtDest >= 0 && ecs->holeSet.sparse[tileIdAtDest] >= 0) {
            holeFillWithBox(x, y, dx, dy, entityId, tileMap, ecs);
        }
        i++;
    }

    grid->map[oy * grid->width + ox] = -1;
}
