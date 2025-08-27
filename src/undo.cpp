#include "undo.h"
#include "hole.h"
#include <stdlib.h>
#define INITIAL_UNDOS 1000
#define HISTORY_PADDING 100
#define MIN_TIME 1

void undoInit(int maxSize, UndoHistory *undoHistory) {
    undoHistory->history = (MoveData *) malloc(sizeof(MoveData) * maxSize);
    undoHistory->size = 0;
    undoHistory->maxSize = maxSize;
    undoHistory->currentTime = MIN_TIME;
}

void undoReInit(int maxSize, UndoHistory *undoHistory) {
    MoveData *newHistory = (MoveData *) malloc(sizeof(MoveData) * maxSize);

    for (int i = 0; i < undoHistory->size; i++) {
        newHistory[i] = undoHistory->history[i];
    }

    undoHistory->maxSize = maxSize;
    free(undoHistory->history);
    undoHistory->history = newHistory;
}

void undoFree(UndoHistory *undoHistory) {
    free(undoHistory->history);
    undoHistory->size = 0;
    undoHistory->maxSize = 0;
    undoHistory->currentTime = MIN_TIME;
}

void undoToTime(int time, ECS *ecs, Grid *grid, TileMap *tileMap, UndoHistory *undoHistory) {
    if (time < MIN_TIME) return;

    MoveData move = undoHistory->history[undoHistory->size-1];
    Vector3 newPosition = (Vector3) {0, 0, 0};

    while (move.timeStamp != time-1 && undoHistory->size > 0) {
        if (ecs->positionSet.sparse[move.entityId] < 0) {
            // Add it's position back
            newPosition.x = (float) move.newX;
            newPosition.y = (float) move.newY;
            newPosition.z = (float) move.z;
            ecsAddPositionZSorted(move.entityId, &newPosition, ecs);

            // If it came from a hole, empty the hole
            holeEmptyHoleAtMap(move.newX, move.newY, tileMap, ecs);
        }

        // variable setup
        int positionIdx = ecs->positionSet.sparse[move.entityId];
        int renderIdx = ecs->renderSet.sparse[move.entityId];
        int x = ecs->positions[positionIdx].x;
        int y = ecs->positions[positionIdx].y;
        int offsetX = move.dx * grid->unitWidth;
        int offsetY = move.dy * grid->unitHeight;

        // undo the move
        ecs->positions[positionIdx].x -= move.dx;
        ecs->positions[positionIdx].y -= move.dy;

        if (grid->map[y * grid->width + x] == move.entityId) {
            grid->map[y * grid->width + x] = -1;
        }
        grid->map[(y - move.dy) * grid->width + (x - move.dx)] = move.entityId;
        ecs->renderers[renderIdx].spriteOffsetX = offsetX;
        ecs->renderers[renderIdx].spriteOffsetY = offsetY;
        ecs->renderers[renderIdx].savedOffsetX = offsetX;
        ecs->renderers[renderIdx].savedOffsetY = offsetY;
        ecs->renderers[renderIdx].moveAnimTime = 0;
        ecs->renderers[renderIdx].maxMoveAnimTime = WALK_TIME;
        ecs->renderers[renderIdx].moveAnimType = WALK;

        // upkeep
        undoHistory->size = undoHistory->size - 1 <= 0 ? 0 : undoHistory->size - 1;
        if (undoHistory->size == 0) continue;
        move = undoHistory->history[undoHistory->size-1];
    }
 
    undoHistory->currentTime = time;
}

void undoAddMove(int entityId, int newX, int newY, int dx, int dy, int z, UndoHistory *undoHistory) {
    MoveData newMove = {
        .entityId = entityId,
        .newX = newX,
        .newY = newY,
        .dx = dx,
        .dy = dy,
        .z = z,
        .timeStamp = undoHistory->currentTime
    };

    if (undoHistory->size + 1 > undoHistory->maxSize) undoReInit(undoHistory->maxSize + HISTORY_PADDING, undoHistory);

    undoHistory->history[undoHistory->size] = newMove;
    undoHistory->size++;
}
