#ifndef INPUT_H
#define INPUT_H
#include "ecs.h"
#include "raylib.h"
#include "grid.h"
#include "tilemap.h"
#include "game.h"

void inputUpdatePlayerMove(ECS *ecs, Grid *grid, TileMap *tileMap, GameState *state);

#endif
