#ifndef LIO_H
#define LIO_H
#include "entity.h"
#include "ecs.h"
#include "components.h"
#include "grid.h"

int levelLineLength(int fileIndex, int fileSize, unsigned char* fileData);
Vector3 levelReadVector3(unsigned char *fileData);
Renderer levelReadRenderer(unsigned char *fileData); 
Pushable levelReadPushable(unsigned char *fileData); 
bool levelInit(ECS *ecs, EntityIdQueue *eIdQ, Grid *grid, int fileIndex, int fileSize, unsigned char *fileData); 
bool levelLoad(const char *fileName, ECS *ecs, EntityIdQueue *eIdQ, Grid *grid);

#endif
