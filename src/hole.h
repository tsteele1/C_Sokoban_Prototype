#ifndef HOLE_H
#define HOLE_H
#include "ecs.h"
#include "tilemap.h"
#include "components.h"

void holeUpdate(ECS *ecs, Grid *grid, TileMap *tileMap);
void holeFillWithBox(int holeX, int holeY, int boxId, TileMap *tileMap, ECS *ecs);
void holeSetUpDxDy(int holeId, int dx, int dy, ECS *ecs);

#endif
