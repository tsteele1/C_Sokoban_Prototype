#include "render.h"
#include "components.h"
#include "grid.h"
#include "tilemap.h"
#include <iostream>

void renderDrawSprites(Texture *atlas, ECS *ecs, Grid *grid) {
    for (int i = 0; i < ecs->positionSet.size; i++) {        
        int entityId = ecs->positionSet.dense[i];

        if (!sparseContains(entityId, &(ecs->renderSet))) continue;

        Renderer renderer = ecs->renderers[ecs->renderSet.sparse[entityId]];
        Vector3 position = ecs->positions[i];

        if (ecs->animSet.sparse[entityId] != -1) {
            Animator animator = ecs->animators[ecs->animSet.sparse[entityId]];
            Frame frame = animator.animations[animator.animIdx][animator.frameIdx];
            renderer.atlasX = frame.atlasX;
            renderer.atlasY = frame.atlasY;
        }

        Rectangle sourceRect = (Rectangle) {
            renderer.atlasX * renderer.spriteWidth,
            renderer.atlasY * renderer.spriteHeight,
            renderer.spriteWidth,
            renderer.spriteHeight
        };
        Rectangle destRect = (Rectangle) {
            grid->worldOffsetX + (position.x * renderer.spriteWidth) + renderer.spriteOffsetX, 
            grid->worldOffsetY + (position.y * renderer.spriteHeight) + renderer.spriteOffsetY,
            renderer.spriteWidth,
            renderer.spriteHeight
        };
        Vector2 center = (Vector2) {
            (renderer.spriteWidth/2) - 1, 
            (renderer.spriteHeight/2) - 1
        };
        float rotation = 0;
        Color tint = WHITE;

        DrawTexturePro(*atlas, sourceRect, destRect, center, rotation, tint);
    }
}

void renderDrawTilemap(Texture *atlas, TileMap *tileMap) {
    float unitWidth = (float) tileMap->unitWidth;
    float unitHeight = (float) tileMap->unitHeight;

    for (int y = 0; y < tileMap->height; y++) {
        for (int x = 0; x < tileMap->width; x++) {
            Tile tile = tileMap->map[y * tileMap->width + x];

            Rectangle sourceRect = (Rectangle) {
                tile.atlasX * unitWidth,
                tile.atlasY * unitHeight,
                unitWidth,
                unitHeight
            };
            Rectangle destRect = (Rectangle) {
                tileMap->worldOffsetX + (x * unitWidth), 
                tileMap->worldOffsetY + (y * unitHeight),
                unitWidth,
                unitHeight
            };
            Vector2 center = (Vector2) {
                (unitWidth/2) - 1, 
                (unitHeight/2) - 1
            };
            float rotation = 0;
            Color tint = WHITE;

            DrawTexturePro(*atlas, sourceRect, destRect, center, rotation, tint);
        }
    }
}

void renderUpdateOffsets(ECS *ecs) {
    for (int i = 0; i < ecs->renderSet.size; i++) {
        float MAX_MOVE_TIME = ecs->renderers[i].maxMoveAnimTime;

        if (ecs->renderers[i].spriteOffsetX != 0) {
            ecs->renderers[i].moveAnimTime = ecs->renderers[i].moveAnimTime + GetFrameTime() > MAX_MOVE_TIME ? MAX_MOVE_TIME : ecs->renderers[i].moveAnimTime + GetFrameTime();

            MoveAnimType moveAnim = ecs->renderers[i].moveAnimType;
            float moveAnimTime = ecs->renderers[i].moveAnimTime;
            bool bumping = moveAnim == BUMP && moveAnimTime < MAX_MOVE_TIME/2;
            float animProgress = bumping ? moveAnimTime/MAX_MOVE_TIME : (MAX_MOVE_TIME - moveAnimTime)/MAX_MOVE_TIME;
            ecs->renderers[i].spriteOffsetX = ecs->renderers[i].savedOffsetX * animProgress;

            if (MAX_MOVE_TIME - moveAnimTime < 0.01) ecs->renderers[i].spriteOffsetX = 0;
        }
        else if (ecs->renderers[i].spriteOffsetY != 0) {
            ecs->renderers[i].moveAnimTime = ecs->renderers[i].moveAnimTime + GetFrameTime() > MAX_MOVE_TIME ? MAX_MOVE_TIME : ecs->renderers[i].moveAnimTime + GetFrameTime();

            MoveAnimType moveAnim = ecs->renderers[i].moveAnimType;
            float moveAnimTime = ecs->renderers[i].moveAnimTime;
            bool bumping = moveAnim == BUMP && moveAnimTime < MAX_MOVE_TIME/2;
            float animProgress = bumping ? moveAnimTime/MAX_MOVE_TIME : (MAX_MOVE_TIME - moveAnimTime)/MAX_MOVE_TIME;
            ecs->renderers[i].spriteOffsetY = ecs->renderers[i].savedOffsetY * animProgress;

            if (MAX_MOVE_TIME - moveAnimTime < 0.01) ecs->renderers[i].spriteOffsetY = 0;
        }
    }
}

void renderUpdateAnimations(ECS *ecs) {
    for (int i = 0; i < ecs->animSet.size; i++) {
        float animTime = (ecs->animators[i].animTime + GetFrameTime());
        int animIdx = ecs->animators[i].animIdx;
        int framesPerAnim = ecs->animators[i].framesPerAnim[animIdx];
        animTime = animTime > ecs->animators[i].fpsForAnim[animIdx]/framesPerAnim ? 0 : animTime;
        ecs->animators[i].animTime = animTime;
        ecs->animators[i].frameIdx = (int) (animTime * framesPerAnim);
    }
}

void renderUpdateAnimIdx(int entityId, int dx, int dy, ECS *ecs) {
    int animatorIdx = ecs->animSet.sparse[entityId];

    if (animatorIdx < 0) return;

    if (dx < 0) {
        ecs->animators[animatorIdx].animIdx = (int) LEFT;
    }
    else if (dx > 0) {
        ecs->animators[animatorIdx].animIdx = (int) RIGHT;
    }
    else if (dy < 0) {
        ecs->animators[animatorIdx].animIdx = (int) UP;
    }
    else if (dy > 0) {
        ecs->animators[animatorIdx].animIdx = (int) DOWN;
    }
}

void renderUpdateAnimIdxUndo(int entityId, int dx, int dy, ECS *ecs) {
    int animatorIdx = ecs->animSet.sparse[entityId];
    if (animatorIdx < 0) return;

    if (dx < 0) {
        ecs->animators[animatorIdx].animIdx = (int) RIGHT;    
    }
    else if (dx > 0) {
        ecs->animators[animatorIdx].animIdx = (int) LEFT;
    }
    else if (dy < 0) {
        ecs->animators[animatorIdx].animIdx = (int) DOWN;
    }
    else if (dy > 0) {
        ecs->animators[animatorIdx].animIdx = (int) UP;
    }
}
