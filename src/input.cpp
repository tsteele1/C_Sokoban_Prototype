#include "input.h"
#include "render.h"
#include <iostream>

void inputUpdatePlayerUndo(ECS *ecs, Grid *grid, TileMap *tileMap, GameState *state, UndoHistory *undoHistory) {
    if (!IsKeyPressed(KEY_Z)) return;

    undoToTime((undoHistory->currentTime - 1), ecs, grid, tileMap, undoHistory);
    *state = AWAIT_MOVE_END;
}

void inputUpdatePlayerMove(ECS *ecs, Grid *grid, TileMap *tileMap, GameState *state, UndoHistory *undoHistory) {
    if (*state == AWAIT_MOVE_END) return;

    const int DX = 1;
    const int DY = 1;
    const int playerDepth = 0;
    const float bumpModifier = 1.5;

    bool keyPressed = false;
    int positionIdx = ecs->positionSet.sparse[0];
    int renderIdx = ecs->renderSet.sparse[0];
    int playerX = ecs->positions[positionIdx].x;
    int playerY = ecs->positions[positionIdx].y;
    if (IsKeyPressed(KEY_RIGHT)) {
        if (gridPushableMap(playerX, playerY, DX, 0, playerDepth, ecs, tileMap, grid)) {
            gridPushBoxesMap(playerX, playerY, DX, 0, ecs, tileMap, grid, undoHistory);
            keyPressed = true;
        }
        else {
            ecs->renderers[renderIdx].spriteOffsetX = DX * grid->unitWidth * bumpModifier;
            ecs->renderers[renderIdx].savedOffsetX = DX * grid->unitWidth * bumpModifier;
            ecs->renderers[renderIdx].moveAnimTime = 0;
            ecs->renderers[renderIdx].maxMoveAnimTime = BUMP_TIME;
            ecs->renderers[renderIdx].moveAnimType = BUMP;
        }
        renderUpdateAnimIdx(0, DX, 0, ecs);
    }
    else if (IsKeyPressed(KEY_LEFT)) {
        if (gridPushableMap(playerX, playerY, -DX, 0, playerDepth, ecs, tileMap, grid)) {
            gridPushBoxesMap(playerX, playerY, -DX, 0, ecs, tileMap, grid, undoHistory);
            keyPressed = true;
        }
        else {
            ecs->renderers[renderIdx].spriteOffsetX = -DX * grid->unitWidth * bumpModifier;
            ecs->renderers[renderIdx].savedOffsetX = -DX * grid->unitWidth * bumpModifier;
            ecs->renderers[renderIdx].moveAnimTime = 0;
            ecs->renderers[renderIdx].maxMoveAnimTime = BUMP_TIME;
            ecs->renderers[renderIdx].moveAnimType = BUMP;
        }
        renderUpdateAnimIdx(0, -DX, 0, ecs);
    } 
    else if (IsKeyPressed(KEY_UP)) {
        if (gridPushableMap(playerX, playerY, 0, -DY, playerDepth, ecs, tileMap, grid)) {
            gridPushBoxesMap(playerX, playerY, 0, -DY, ecs, tileMap, grid, undoHistory);
            keyPressed = true;
        }
        else {
            ecs->renderers[renderIdx].spriteOffsetY = -DY * grid->unitHeight * bumpModifier;
            ecs->renderers[renderIdx].savedOffsetY = -DY * grid->unitWidth * bumpModifier;
            ecs->renderers[renderIdx].moveAnimTime = 0;
            ecs->renderers[renderIdx].maxMoveAnimTime = BUMP_TIME;
            ecs->renderers[renderIdx].moveAnimType = BUMP;
        }
        renderUpdateAnimIdx(0, 0, -DY, ecs);
        ecs->animators[ecs->animSet.sparse[0]].animIdx = (int) UP;
    }
    else if (IsKeyPressed(KEY_DOWN)) {
        if (gridPushableMap(playerX, playerY, 0, DY, playerDepth, ecs, tileMap, grid)) { 
            gridPushBoxesMap(playerX, playerY, 0, DY, ecs, tileMap, grid, undoHistory);
            keyPressed = true;
        }
        else {
            ecs->renderers[renderIdx].spriteOffsetY = DY * grid->unitHeight * bumpModifier;
            ecs->renderers[renderIdx].savedOffsetY = DY * grid->unitHeight * bumpModifier;
            ecs->renderers[renderIdx].moveAnimTime = 0;
            ecs->renderers[renderIdx].maxMoveAnimTime = BUMP_TIME;
            ecs->renderers[renderIdx].moveAnimType = BUMP;
        }
        renderUpdateAnimIdx(0, 0, DY, ecs);
    }

    if (keyPressed) {
        *state = AWAIT_MOVE_END;
    }
}
