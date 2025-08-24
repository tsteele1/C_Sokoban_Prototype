#include "ecs.h"
#include <stdlib.h>

void ecsInit(int numEntities, ECS *ecs) {
    sparseInit(numEntities, &(ecs->positionSet));
    sparseInit(numEntities, &(ecs->renderSet));
    sparseInit(numEntities, &(ecs->pushSet));
    sparseInit(numEntities, &(ecs->animSet));
    sparseInit(numEntities, &(ecs->goalSet));
    sparseInit(numEntities, &(ecs->boxSet));

    ecs->positions = (Vector3 *) malloc(sizeof(Vector3) * numEntities);
    ecs->renderers = (Renderer *) malloc(sizeof(Renderer) * numEntities);
    ecs->pushables = (Pushable *) malloc(sizeof(Renderer) * numEntities);
    ecs->animators = (Animator *) malloc(sizeof(Animator) * numEntities);
    ecs->goals = (Goal *) malloc(sizeof(Goal) * numEntities);
    ecs->boxes = (Box *) malloc(sizeof(Box) * numEntities);
}

void ecsFree(ECS *ecs) {
    free(ecs->positions);
    free(ecs->renderers);
    free(ecs->pushables);
    free(ecs->animators);
    free(ecs->goals);
    free(ecs->boxes);

    sparseFree(&(ecs->positionSet));
    sparseFree(&(ecs->renderSet));
    sparseFree(&(ecs->pushSet));
    sparseFree(&(ecs->animSet));
    sparseFree(&(ecs->goalSet));
    sparseFree(&(ecs->boxSet));
}

void ecsAddPosition(int entityId, Vector3 *position, ECS *ecs) {
    if (ecs->positionSet.size + 1 > ecs->positionSet.maxSize) return;

    ecs->positions[ecs->positionSet.size] = *position;
    sparseAdd(entityId, &(ecs->positionSet));
}

void ecsAddPositionZSorted(int entityId, Vector3 *position, ECS *ecs) {
    if (ecs->positionSet.size + 1 > ecs->positionSet.maxSize) return;

    ecs->positions[ecs->positionSet.size] = *position;
    sparseAdd(entityId, &(ecs->positionSet));

    for (int i = ecs->positionSet.size - 1; i > 0; i--) {
        Vector3 tempPosition = ecs->positions[i-1];
        int tempId = ecs->positionSet.dense[i-1];

        if (tempPosition.z <= (*position).z) break;

        // swap 
        ecs->positions[i-1] = *position;
        ecs->positionSet.dense[i-1] = entityId;
        ecs->positionSet.sparse[entityId] = i - 1;

        ecs->positions[i] = tempPosition;
        ecs->positionSet.dense[i] = tempId;
        ecs->positionSet.sparse[tempId] = i;
    }
}

void ecsSwapPositionIndices(int low, int high, ECS *ecs) {
    Vector3 tempVec = ecs->positions[high];
    int tempId = ecs->positionSet.dense[high];
    int entityId = ecs->positionSet.dense[low];

    ecs->positions[high] = ecs->positions[low];
    ecs->positionSet.dense[high] = entityId;
    ecs->positionSet.sparse[entityId] = high;

    ecs->positions[low] = tempVec;
    ecs->positionSet.dense[low] = tempId;
    ecs->positionSet.sparse[tempId] = low;
}

int ecsPartitionPositionsByZ(int low, int pivot, int high, ECS *ecs) {
    int pivotZ = ecs->positions[pivot].z;
    int i = low - 1;
    int j = high + 1;
    ecsSwapPositionIndices(pivot, high, ecs);

    while (1) {
        do {
            i++;
        } while (ecs->positions[i].z < pivotZ);

        do {
            j--;
        } while (ecs->positions[j].z > pivotZ);

        if (i >= j) return j;

        ecsSwapPositionIndices(i, j, ecs);
    }
}

void ecsQuicksortPositionsByZ(int low, int high, ECS *ecs) {
    // why sort if nothing to sort?
    if (ecs->positionSet.size == 0) return;
    else if (low < 0 || high >= ecs->positionSet.size) return;

    if (high - low + 1 == 1 || high < low) return;
    else if (high - low + 1 == 2) {
        if (ecs->positions[low].z <= ecs->positions[high].z) return;
        ecsSwapPositionIndices(low, high, ecs); 
        return;
    }

    int pivot = GetRandomValue(low, high);
    pivot = ecsPartitionPositionsByZ(low, pivot, high, ecs);

    ecsQuicksortPositionsByZ(low, pivot, ecs);
    ecsQuicksortPositionsByZ(pivot + 1, high, ecs);
}

void ecsZSort(ECS *ecs) {
    int low = 0;
    int high = ecs->positionSet.size;

    ecsQuicksortPositionsByZ(low, high-1, ecs);
}

void ecsRemovePosition(int entityId, ECS *ecs) {
    if (ecs->positionSet.size == 0) return;

    // remove changes idx position of last element, so store it
    int idxToAdjust = ecs->positionSet.dense[(ecs->positionSet.size) - 1];

    sparseRemove(entityId, &(ecs->positionSet));

    // get the new idx of the element we need to change in positions
    int newIdx = ecs->positionSet.sparse[idxToAdjust];

    ecs->positions[newIdx] = ecs->positions[ecs->positionSet.size];

    // Since the size of the positionSet was changed, we don't have
    // to worry about there being an extra position at the end of
    // the array since we'll never iterate over it.
}

void ecsAddRender(int entityId, Renderer *renderer, ECS *ecs) {
    if (ecs->renderSet.size + 1 > ecs->renderSet.maxSize) return;

    ecs->renderers[ecs->renderSet.size] = *renderer;
    sparseAdd(entityId, &(ecs->renderSet));
}

void ecsRemoveRender(int entityId, ECS *ecs) {
    if (ecs->renderSet.size == 0) return;

    // remove changes idx position of last element, so store it
    int idxToAdjust = ecs->renderSet.dense[(ecs->renderSet.size) - 1];

    sparseRemove(entityId, &(ecs->renderSet));

    // get the new idx of the element we need to change in positions
    int newIdx = ecs->renderSet.sparse[idxToAdjust];

    ecs->renderers[newIdx] = ecs->renderers[ecs->renderSet.size];
}

void ecsAddPushable(int entityId, Pushable *pushable, ECS *ecs) {
    if (ecs->pushSet.size + 1 > ecs->pushSet.maxSize) return;

    ecs->pushables[ecs->pushSet.size] = *pushable;
    sparseAdd(entityId, &(ecs->pushSet));
}


void ecsRemovePushable(int entityId, ECS *ecs) {
    if (ecs->pushSet.size == 0) return;

    // remove changes idx position of last element, so store it
    int idxToAdjust = ecs->pushSet.dense[(ecs->pushSet.size) - 1];

    sparseRemove(entityId, &(ecs->pushSet));

    // get the new idx of the element we need to change in positions
    int newIdx = ecs->pushSet.sparse[idxToAdjust];

    ecs->pushables[newIdx] = ecs->pushables[ecs->pushSet.size];
}

void ecsAddAnimator(int entityId, Animator *animator, ECS *ecs) {
    if (ecs->animSet.size + 1 > ecs->animSet.maxSize) return;

    ecs->animators[ecs->animSet.size] = *animator;
    sparseAdd(entityId, &(ecs->animSet));
}

void ecsRemoveAnimator(int entityId, ECS *ecs) {
    if (ecs->animSet.size == 0) return;

    int idxToAdjust = ecs->animSet.dense[(ecs->animSet.size)-1];

    sparseRemove(entityId, &(ecs->animSet));

    int newIdx = ecs->animSet.sparse[idxToAdjust];

    ecs->animators[newIdx] = ecs->animators[ecs->animSet.size];
}

void ecsAddGoal(int entityId, Goal *goal, ECS *ecs) {
    if (ecs->goalSet.size + 1 > ecs->goalSet.maxSize) return;

    ecs->goals[ecs->goalSet.size] = *goal;
    sparseAdd(entityId, &(ecs->goalSet));
}

void ecsRemoveGoal(int entityId, ECS *ecs) {
    if (ecs->goalSet.size == 0) return;

    int idxToAdjust = ecs->goalSet.dense[(ecs->goalSet.size)-1];
    
    sparseRemove(entityId, &(ecs->goalSet));

    int newIdx = ecs->goalSet.sparse[idxToAdjust];

    ecs->goals[newIdx] = ecs->goals[ecs->goalSet.size];
}

void ecsAddBox(int entityId, Box *box, ECS *ecs) {
    if (ecs->boxSet.size + 1 > ecs->boxSet.maxSize) return;

    ecs->boxes[ecs->boxSet.size] = *box;
    sparseAdd(entityId, &(ecs->boxSet));
}

void ecsRemoveBox(int entityId, ECS *ecs) {
    if (ecs->boxSet.size == 0) return;

    int idxToAdjust = ecs->boxSet.dense[(ecs->boxSet.size)-1];

    sparseRemove(entityId, &(ecs->boxSet));

    int newIdx = ecs->boxSet.sparse[idxToAdjust];

    ecs->boxes[newIdx] = ecs->boxes[ecs->boxSet.size];
}
