#include "raylib.h"

#include "render.cpp"
#include "level_io.cpp"
#include "ecs.cpp"
#include "sparse_set.cpp"
#include "grid.cpp"
#include "input.cpp"
#include "goal.cpp"
#include "tilemap.cpp"
#include "hole.cpp"
#include "undo.cpp"

#include "entity.h"
#include "components.h"
#include "game.h"

#include <iostream>
#include <cstdlib>

// SHORT TERM GOALS:
//  1. FILE I/O:
//      - Put player info in its own separate file (player.ec)
//      - Find ways to load in prefabs for levels, and then use level files
//        to load in those prefabs and add info the prefab wouldn't have.
//          - For example, almost every entity in the game will have a
//            preset renderer, a preset animator, a preset pushable, etc.
//  3. Screen Sizing:
//      - Set a reference resolution as a constant.
//      - Allow the player to change the screen size, and multiply everything
//        being renderered by a factor of (new_size/reference_size).
//  4. SFX / Music:
//      - Background track that loops.
//      - When the player pushes a box.
//      - When a box falls into a hole.
//      - When the player uses an undo.
//      - When the player completes a level.
//      - More to come later.
//  5. Menu:
//      - Pause the game.
//      - Give the option to quit to a new level.
//      - Give the option to reset a level entirely.
//  6. Level Select:
//      - Display all levels currently loaded into the game.
//          - Each should be a separate little box/menu item.
//  7. Title Screen:
//      - Title.
//      - Options for level select and quitting the game.
//  8. Level Editor:
//      - Button to display all tiles.
//          - Including a blank space indicator.
//      - Button to select a tile.
//          - Display it somehow.
//      - Player can then move their mouse over the grid.
//          - If they click their mouse button, set that space on the grid with
//            the tile they previously selected.
//      - Button to save the current grid.
//      - Button to move to a level loader.
//          - Select a level grid to display on screen. 
//      - Button to exit.
//  KNOWN BUGS / ISSUES:
//      Consider adding an input buffer:
//          Record inputs the player made within a window of time
//          right before the player reaches their previous destination.

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

    eIdQ.currentId++;
    TileMap tileMap;
    Tile tile;
    Vector3 tilePosition = {1, 5, 0};
    tilemapInit(gridWidth, gridHeight, gridOffsetX, gridOffsetY, UNIT_WIDTH, UNIT_HEIGHT, &tileMap);
    tile = (Tile) {11, 0, eIdQ.currentId};
    tileMap.map[((int) tilePosition.y) * tileMap.width + ((int) tilePosition.x)] = tile;
    Hole hole = {
        .storedType = NO_BOX,
        .boxDirX = 0,
        .boxDirY = 0,
        .boxId = -1,
        .baseAtlasX = 11,
        .baseAtlasY = 0
    };
    ecsAddPosition(eIdQ.currentId, &tilePosition, &ecs); 
    ecsAddHole(eIdQ.currentId, &hole, &ecs);
    eIdQ.currentId++;

    // z sorting
    SetRandomSeed(GetTime() * 1000);
    ecsZSort(&ecs);
    gridUpdateWithECS(&ecs, &grid);

    UndoHistory undoHistory;
    undoInit(INITIAL_UNDOS, &undoHistory);

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
                inputUpdatePlayerUndo(&ecs, &grid, &tileMap, &gameState, &undoHistory);
                inputUpdatePlayerMove(&ecs, &grid, &tileMap, &gameState, &undoHistory);
                renderUpdateOffsets(&ecs);

                // Draw
                BeginDrawing();
                ClearBackground(RAYWHITE);
                renderDrawTilemap(&atlas, &tileMap);
                renderDrawSprites(&atlas, &ecs, &grid);
                EndDrawing();
                break;

            case AWAIT_MOVE_END:
                renderUpdateAnimations(&ecs);
                renderUpdateOffsets(&ecs);

                gameState = HOLES;
                for (int i = 0; i < ecs.renderSet.size; i++) {
                    if (ecs.renderers[i].spriteOffsetX != 0 || ecs.renderers[i].spriteOffsetY != 0) {
                        gameState = AWAIT_MOVE_END;
                        break;
                    }
                }

                BeginDrawing();
                ClearBackground(RAYWHITE);
                renderDrawTilemap(&atlas, &tileMap);
                renderDrawSprites(&atlas, &ecs, &grid);
                EndDrawing();
                break;

            case HOLES:
                holeUpdate(&ecs, &grid, &tileMap);
                gameState = GOALCHECK;
                break;
 
            case GOALCHECK:
                goalCheckLevelEnd(&ecs, &grid, &gameState);
                break;

            case LEVEL_END:
                renderUpdateAnimations(&ecs);
                renderUpdateOffsets(&ecs);

                BeginDrawing();
                ClearBackground(RAYWHITE);
                renderDrawTilemap(&atlas, &tileMap);
                renderDrawSprites(&atlas, &ecs, &grid);
                EndDrawing();
                break;
        }
    }

    UnloadTexture(atlas);
    undoFree(&undoHistory);
    tilemapFree(&tileMap);
    gridFree(&grid);
    ecsFree(&ecs);

    return 0;
}
