#ifndef HOLE_H
#define HOLE_H
#include "ecs.h"
#include "tilemap.h"
#include "components.h"

void holeFillWithBox(int holeX, int holeY, int dx, int dy, int boxId, TileMap *tileMap, ECS *ecs);

#endif
