#ifndef UNDO_H
#define UNDO_H
#include "grid.h"
#include "ecs.h"
#include "tilemap.h"
#include "undo_history.h"

void undoInit(int maxSize, UndoHistory *undoHistory);
void undoFree(UndoHistory *undoHistory);

void undoToTime(int time, ECS *ecs, Grid *grid, TileMap *tileMap, UndoHistory *undoHistory);
void undoAddMove(int entityId, int newX, int NewY, int dx, int dy, int z, UndoHistory *undoHistory);

#endif
