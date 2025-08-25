#include "hole.h"

void holeUpdate(ECS *ecs, Grid *grid, TileMap *tileMap) {
    for (int i = 0; i < ecs->holeSet.size; i++) {
        if (ecs->holes[i].storedType != NO_BOX) continue;

        int entityId = ecs->holeSet.dense[i];
        Vector3 holePosition = ecs->positions[ecs->positionSet.sparse[entityId]];

        int boxId = gridGetAtMap(holePosition.x, holePosition.y, grid);
        if (boxId < 0) continue;

        gridSetFromMap(holePosition.x, holePosition.y, -1, grid);
        holeFillWithBox(holePosition.x, holePosition.y, boxId, tileMap, ecs);
    }
}

void holeChangeSprite(int holeIdx, int holeX, int holeY, TileMap *tileMap, ECS *ecs) {
    const int woodBoxCol = 6;
    const int redBoxCol = 7;
    const int blueBoxCol = 8;
    const int greenBoxCol = 9;
    const int greyBoxCol = 10;
    const int filledHoleRow = 4;

    switch(ecs->holes[holeIdx].storedType) {
        case WOODEN:
            tileMap->map[holeY * tileMap->width + holeX].atlasX = woodBoxCol;
            tileMap->map[holeY * tileMap->width + holeX].atlasY = filledHoleRow;
            break;

        case RED_METAL:
            tileMap->map[holeY * tileMap->width + holeX].atlasX = redBoxCol;
            tileMap->map[holeY * tileMap->width + holeX].atlasY = filledHoleRow;
            break;

        case GREEN_METAL:
            tileMap->map[holeY * tileMap->width + holeX].atlasX = greenBoxCol;
            tileMap->map[holeY * tileMap->width + holeX].atlasY = filledHoleRow;
            break;

        case BLUE_METAL:
            tileMap->map[holeY * tileMap->width + holeX].atlasX = blueBoxCol;
            tileMap->map[holeY * tileMap->width + holeX].atlasY = filledHoleRow;
            break;

        case GREY_METAL:
            tileMap->map[holeY * tileMap->width + holeX].atlasX = greyBoxCol;
            tileMap->map[holeY * tileMap->width + holeX].atlasY = filledHoleRow;
            break;

        case NO_BOX:
            break;
    }
}

void holeSetUpDxDy(int holeId, int dx, int dy, ECS *ecs) {
    int holeIdx = ecs->holeSet.sparse[holeId];
    ecs->holes[holeIdx].boxDirX = -dx;
    ecs->holes[holeIdx].boxDirY = -dy;
}

void holeFillWithBox(int holeX, int holeY, int boxId, TileMap *tileMap, ECS *ecs) {
    if (holeX < 0 || holeX >= tileMap->width || holeY < 0 || holeY >= tileMap->height) return;

    int holeId = tileMap->map[holeY * tileMap->width + holeX].entityId;
    if (ecs->holeSet.sparse[holeId] < 0 || ecs->boxSet.sparse[boxId] < 0) return;

    Box box = ecs->boxes[ecs->boxSet.sparse[boxId]];
    ecsRemovePosition(boxId, ecs);

    int holeIdx = ecs->holeSet.sparse[holeId];
    ecs->holes[holeIdx].storedType = box.type;
    ecs->holes[holeIdx].boxId = boxId;

    holeChangeSprite(holeIdx, holeX, holeY, tileMap, ecs);
}
