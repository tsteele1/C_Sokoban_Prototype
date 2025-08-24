#include "render.h"
#include "components.h"
#include <iostream>

void renderDrawSprites(Texture *atlas, ECS *ecs) {
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
            40 + (position.x * renderer.spriteWidth) + renderer.spriteOffsetX, 
            40 + (position.y * renderer.spriteHeight) + renderer.spriteOffsetY,
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

void renderUpdateOffsets(ECS *ecs) {
    const float MAX_MOVE_TIME = 0.75;

    for (int i = 0; i < ecs->renderSet.size; i++) {

        if (ecs->renderers[i].spriteOffsetX != 0) {
            ecs->renderers[i].moveAnimTime = ecs->renderers[i].moveAnimTime + GetFrameTime() > MAX_MOVE_TIME ? MAX_MOVE_TIME : ecs->renderers[i].moveAnimTime + GetFrameTime();

            ecs->renderers[i].spriteOffsetX = ecs->renderers[i].spriteOffsetX * (MAX_MOVE_TIME - ecs->renderers[i].moveAnimTime);

            float tempX = ecs->renderers[i].spriteOffsetX;
            tempX = tempX < 0 ? -tempX : tempX;
            if (tempX < 0.5) ecs->renderers[i].spriteOffsetX = 0;
        }
        else if (ecs->renderers[i].spriteOffsetY != 0) {
            ecs->renderers[i].moveAnimTime = ecs->renderers[i].moveAnimTime + GetFrameTime() > MAX_MOVE_TIME ? MAX_MOVE_TIME : ecs->renderers[i].moveAnimTime + GetFrameTime();

            ecs->renderers[i].spriteOffsetY = ecs->renderers[i].spriteOffsetY * (MAX_MOVE_TIME - ecs->renderers[i].moveAnimTime);

            float tempY = ecs->renderers[i].spriteOffsetY;
            tempY = tempY < 0 ? -tempY : tempY;
            if (tempY < 0.5) ecs->renderers[i].spriteOffsetY = 0;
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
