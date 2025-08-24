#include "raylib.h"

#include "render.cpp"
#include "level_io.cpp"
#include "ecs.cpp"
#include "sparse_set.cpp"
#include "grid.cpp"
#include "input.cpp"
#include "goal.cpp"

#include "entity.h"
#include "components.h"
#include "game.h"

#include <iostream>
#include <cstdlib>

// SHORT TERM GOALS:
//  0. Grid Alignment:
//      - Change render so it doesn't use magic numbers
//  1. FILE I/O:
//      - Put player info in its own separate file (player.ec)
//      - Find ways to load in prefabs for levels, and then use level files
//        to load in those prefabs
//  2. Holes:
//      - How do we implement them?
//  3. Undo:
//  4. Screen Sizing:
//  5. SFX / Music:
//  6. Menu:
//  7. Level Select:
//  8. Title Screen:
//  9. Level Editor:
//  KNOWN BUGS:

int main() {
    // const info
    int screenWidth = 800;
    int screenHeight = 450;
    const int numEntities = 2000;
    const char levelName[] = "./test_level.ec";
    GameState gameState = PLAYER;

    // raylib setup
    InitWindow(screenWidth, screenHeight, "Sokoban_Prototype");
    SetTargetFPS(60);

    // texture info
    Texture atlas = LoadTexture("../art/sokoban_tilesheet.png");
    const int SPRWIDTH = atlas.width / 13;
    const int SPRHEIGHT = atlas.height / 8;
    const int UNIT_WIDTH = 64;
    const int UNIT_HEIGHT = 64;

    // ecs setup based on level
    ECS ecs; 
    ecsInit(numEntities, &ecs);
    EntityIdQueue eIdQ = {-1, numEntities};

    //TODO put these into a .ec file
    Animator animator;
    animator.currentNumAnims = 4;
    animator.animIdx = 0;
    animator.frameIdx = 0;
    animator.animTime = 0;
    for (int i = 0; i < animator.currentNumAnims; i++) {
        animator.framesPerAnim[i] = 4;
        animator.fpsForAnim[i] = 5;
    }
    // front
    animator.animations[0][0] = (Frame) {0, 4};
    animator.animations[0][1] = (Frame) {1, 4};
    animator.animations[0][2] = (Frame) {0, 4};
    animator.animations[0][3] = (Frame) {2, 4};
    // back
    animator.animations[1][0] = (Frame) {3, 4};
    animator.animations[1][1] = (Frame) {4, 4};
    animator.animations[1][2] = (Frame) {3, 4};
    animator.animations[1][3] = (Frame) {5, 4};
    // left
    animator.animations[2][0] = (Frame) {3, 6};
    animator.animations[2][1] = (Frame) {4, 6};
    animator.animations[2][2] = (Frame) {3, 6};
    animator.animations[2][3] = (Frame) {5, 6};
    // right
    animator.animations[3][0] = (Frame) {0, 6};
    animator.animations[3][1] = (Frame) {1, 6};
    animator.animations[3][2] = (Frame) {0, 6};
    animator.animations[3][3] = (Frame) {2, 6};
    ecsAddAnimator(0, &animator, &ecs);

    Grid grid;
    // FIXME can we load these variables with the grid from the .lvl?
    const int gridOffsetX = 40;
    const int gridOffsetY = 40;
    const int maxPushDepth = 1;
    int gridWidth = (screenWidth - gridOffsetX) / UNIT_WIDTH + 1;
    int gridHeight = (screenHeight - gridOffsetY) / UNIT_HEIGHT + 1;
    gridInit(gridWidth, gridHeight, gridOffsetX, gridOffsetY, UNIT_WIDTH, UNIT_HEIGHT, maxPushDepth, &grid);

    levelLoad(levelName, &ecs, &eIdQ, &grid);

    // z sorting
    SetRandomSeed(GetTime() * 1000);
    ecsZSort(&ecs);
    gridUpdateWithECS(&ecs, &grid);

    // adding to the renderers
    for (int i = 0; i < ecs.renderSet.size; i++) {
        ecs.renderers[i].spriteWidth = SPRWIDTH;
        ecs.renderers[i].spriteHeight = SPRHEIGHT;
        ecs.renderers[i].spriteOffsetX = 0;
        ecs.renderers[i].spriteOffsetY = 0;
        ecs.renderers[i].moveAnimTime = 0;
        ecs.renderers[i].moveAnimType = WALK;
    }

    // Game Loop
    while (!WindowShouldClose()) {
        switch(gameState) {
            case PLAYER:
                // Update
                renderUpdateAnimations(&ecs);
                inputUpdatePlayerMove(&ecs, &grid, &gameState);
                renderUpdateOffsets(&ecs);

                // Draw
                BeginDrawing();
                ClearBackground(RAYWHITE);
                renderDrawSprites(&atlas, &ecs);
                EndDrawing();
                break;
            case GOALCHECK:
                goalCheckLevelEnd(&ecs, &grid, &gameState);
                break;
            case LEVEL_END:
                renderUpdateAnimations(&ecs);
                renderUpdateOffsets(&ecs);

                BeginDrawing();
                ClearBackground(RAYWHITE);
                renderDrawSprites(&atlas, &ecs);
                EndDrawing();
                break;
        }
    }

    UnloadTexture(atlas);
    gridFree(&grid);
    ecsFree(&ecs);

    return 0;
}
