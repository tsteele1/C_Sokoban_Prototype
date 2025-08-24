#include "goal.h"

void goalCheckLevelEnd(ECS *ecs, Grid *grid, GameState *state) {
    bool doneLevel = true;

    for (int i = 0; i < ecs->goalSet.size; i++) {
        int entityId = ecs->goalSet.dense[i]; 
        Vector3 position = ecs->positions[ecs->positionSet.sparse[entityId]];
        Goal goal = ecs->goals[i];

        int boxId = gridGetAtMap(position.x, position.y, grid);
        if (boxId < 0 || ecs->boxSet.sparse[boxId] < 0) {
            doneLevel = false;
            break;
        }

        Box box = ecs->boxes[ecs->boxSet.sparse[boxId]];
        if (box.type != goal.requiredBox) {
            doneLevel = false;
            break;
        }
    }

    *state = doneLevel ? LEVEL_END : PLAYER;
}
