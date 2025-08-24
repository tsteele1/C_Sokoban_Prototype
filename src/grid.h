#ifndef GRID_H
#define GRID_H
#include "ecs.h"

struct Grid {
    int *map;
    int width;
    int height;
    int worldOffsetX;
    int worldOffsetY;
    int unitWidth;
    int unitHeight;
    int pushDepth;
};

void gridInit(int width, int height, int worldOffsetX, int worldOffsetY, int unitWidth, int unitHeight, int maxPushDepth, Grid *grid); 
void gridFree(Grid *grid); 
void gridUpdateWithECS(ECS *ecs, Grid *grid);
int gridGetAtWorld(int x, int y, Grid *grid); 
int gridGetAtMap(int x, int y, Grid *grid);
bool gridInBoundsMap(int x, int y, int width, int height); 
void gridSetFromWorld(int x, int y, int id, Grid *grid); 
void gridSetFromMap(int x, int y, int id, Grid *grid);
bool gridPushableMap(int x, int y, int dx, int dy, int pushDepth, ECS *ecs, Grid *grid); 
bool gridPushableWorld(int x, int y, int dx, int dy, ECS *ecs, Grid *grid); 
bool gridWalkableWorld(int x, int y, ECS *ecs, Grid *grid);
bool gridWalkableMap(int x, int y, ECS *ecs, Grid *grid);
bool gridBoxAtMap(int x, int y, ECS *ecs, Grid *grid);
void gridPushBoxesWorld(int x, int y, ECS *ecs, Grid *grid);
void gridPushBoxesMap(int x, int y, ECS *ecs, Grid *grid);

#endif
