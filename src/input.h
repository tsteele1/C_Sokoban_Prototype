#ifndef INPUT_H
#define INPUT_H
#include "ecs.h"
#include "raylib.h"
#include "grid.h"
#include "tilemap.h"
#include "undo.h"
#include "game.h"

void inputUpdatePlayerMove(ECS *ecs, Grid *grid, TileMap *tileMap, GameState *state);
void inputUpdatePlayerUndo(ECS *ecs, Grid *grid, TileMap *tileMap, GameState *state, UndoHistory *undoHistory);

#endif
