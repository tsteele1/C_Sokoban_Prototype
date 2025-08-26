#ifndef ECS_H
#define ECS_H

#include "components.h"
#include "sparse_set.h"
#include "raylib.h"

// ECS (Sokoban Version) features:
// Position
// Rendering
// Pushable
// Animated
// Goal
// Box
struct ECS {
    SparseSet positionSet;
    SparseSet renderSet;
    SparseSet pushSet;
    SparseSet animSet;
    SparseSet goalSet;
    SparseSet boxSet;
    SparseSet holeSet;

    Vector3 *positions;
    Renderer *renderers;
    Pushable *pushables;
    Animator *animators;
    Goal *goals;
    Box *boxes;
    Hole *holes;
};

void ecsInit(int numEntities, ECS *ecs);
void ecsFree(ECS *ecs);

void ecsAddPosition(int entityId, Vector3 *position, ECS *ecs);
void ecsAddPositionSortedZ(int entityId, Vector3 *position, ECS *ecs);
void ecsZSort(ECS *ecs);
void ecsRemovePosition(int entityId, ECS *ecs);

void ecsAddRender(int entityId, Renderer *renderer, ECS *ecs);
void ecsRemoveRender(int entityId, ECS *ecs);

void ecsAddPushable(int entityId, Pushable *pushable, ECS *ecs);
void ecsRemovePushable(int entityId, ECS *ecs);

void ecsAddAnimator(int entityId, Animator *animator, ECS *ecs);
void ecsRemoveAnimator(int entityId, Animator *animator, ECS *ecs);

void ecsAddGoal(int entityId, Goal *goal, ECS *ecs);
void ecsRemoveGoal(int entityId, Goal *goal, ECS *ecs);

void ecsAddBox(int entityId, Box *box, ECS *ecs);
void ecsRemoveBox(int entityId, ECS *ecs);

void ecsAddHole(int entityId, Hole *hole, ECS *ecs);
void ecsRemoveHole(int entityId, ECS *ecs);

#endif
