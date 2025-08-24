#ifndef INPUT_H
#define INPUT_H
#include "ecs.h"
#include "raylib.h"
#include "grid.h"
#include "game.h"

void inputUpdatePlayerMove(ECS *ecs, Grid *grid, GameState *state);

#endif
