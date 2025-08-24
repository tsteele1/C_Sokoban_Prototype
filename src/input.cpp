#include "input.h"

void inputUpdatePlayerMove(ECS *ecs, Grid *grid, GameState *state) {
    const int DX = 1;
    const int DY = 1;
    const int playerDepth = 0;

    bool keyPressed = false;
    int playerX = ecs->positions[ecs->positionSet.sparse[0]].x;
    int playerY = ecs->positions[ecs->positionSet.sparse[0]].y;
    if (IsKeyPressed(KEY_RIGHT) && gridPushableMap(playerX, playerY, DX, 0, playerDepth, ecs, grid)) {
        ecs->animators[ecs->animSet.sparse[0]].animIdx = 3;
        gridPushBoxesMap(playerX, playerY, DX, 0, ecs, grid);
        keyPressed = true;
    }
    else if (IsKeyPressed(KEY_LEFT) && gridPushableMap(playerX, playerY, -DX, 0, playerDepth, ecs, grid)) {
        ecs->animators[ecs->animSet.sparse[0]].animIdx = 2;
        gridPushBoxesMap(playerX, playerY, -DX, 0, ecs, grid);
        keyPressed = true;
    } 
    else if (IsKeyPressed(KEY_UP) && gridPushableMap(playerX, playerY, 0, -DY, playerDepth, ecs, grid)) {
        ecs->animators[ecs->animSet.sparse[0]].animIdx = 1;
        gridPushBoxesMap(playerX, playerY, 0, -DY, ecs, grid);
        keyPressed = true;
    }
    else if (IsKeyPressed(KEY_DOWN) && gridPushableMap(playerX, playerY, 0, DY, playerDepth, ecs, grid)) {
        ecs->animators[ecs->animSet.sparse[0]].animIdx = 0;
        gridPushBoxesMap(playerX, playerY, 0, DY, ecs, grid);
        keyPressed = true;
    }

    if (keyPressed) {
        *state = GOALCHECK;
    }
}
