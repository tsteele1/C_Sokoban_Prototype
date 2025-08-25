#include "hole.h"

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

void holeFillWithBox(int holeX, int holeY, int dx, int dy, int boxId, TileMap *tileMap, ECS *ecs) {
    if (holeX < 0 || holeX >= tileMap->width || holeY < 0 || holeY >= tileMap->height) return;

    int holeId = tileMap->map[holeY * tileMap->width + holeX].entityId;
    if (ecs->holeSet.sparse[holeId] < 0 || ecs->boxSet.sparse[boxId] < 0) return;

    Box box = ecs->boxes[ecs->boxSet.sparse[boxId]];
    ecsRemovePosition(boxId, ecs);

    int holeIdx = ecs->holeSet.sparse[holeId];
    ecs->holes[holeIdx].storedType = box.type;
    ecs->holes[holeIdx].boxId = boxId;
    ecs->holes[holeIdx].boxDirX = -dx;
    ecs->holes[holeIdx].boxDirY = -dy;

    holeChangeSprite(holeIdx, holeX, holeY, tileMap, ecs);
}
