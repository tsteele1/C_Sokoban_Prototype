#include "raylib.h"
#include "level_io.h"
#include "grid.h"
#include <stdio.h>
#include <stdlib.h>

int levelLineLength(int fileIndex, int fileSize, unsigned char* fileData) {
    char currentChar = fileData[fileIndex];
    int lineLength = 0;

    while (currentChar != '\n' && fileIndex + lineLength < fileSize) {
        lineLength++;
        currentChar = fileData[fileIndex + lineLength];
    }

    return lineLength + 1;
}

Vector3 levelReadVector3(unsigned char *fileData) {
    Vector3 vec3;

    sscanf((char *) fileData, ".V3 %f %f %f", &vec3.x, &vec3.y, &vec3.z);

    //printf("%f %f %f\n", vec3.x, vec3.y, vec3.z);
    return vec3;
}

Renderer levelReadRenderer(unsigned char *fileData) {
    Renderer render;

    sscanf((char *) fileData, ".R %f %f", &render.atlasX, &render.atlasY);

    //printf("%f %f\n", render.atlasX, render.atlasY);
    return render;
}

Pushable levelReadPushable(unsigned char *fileData) {
    Pushable pushable {
        .isMoving = false
    };

    return pushable;
}

Goal levelReadGoal(unsigned char *fileData) {
    Goal goal;

    sscanf((char *) fileData, ".O %d", &goal.requiredBox);

    return goal;
}

Box levelReadBox(unsigned char *fileData) {
    Box box;

    sscanf((char *) fileData, ".B %d", &box.type);

    return box;
}

bool levelInit(ECS *ecs, EntityIdQueue *eIdQ, Grid *grid, int fileIndex, int fileSize, unsigned char *fileData) {
    bool reading = true;
    char cmd = ' ';

    char vecType;
    Vector3 position;
    Renderer render;
    Pushable pushable;
    Goal goal;
    Box box;

    while (reading && fileIndex < fileSize) {
        char firstChar = fileData[fileIndex];
        if (firstChar == '/' || firstChar == ' ' || firstChar == '\n' || firstChar == '\r') {
            fileIndex += levelLineLength(fileIndex, fileSize, fileData);
            continue;
        }
        else if (firstChar != '.') {
            printf("levelInit ERROR: Reading '%c', but expected '.'", firstChar);
            return false;
            continue;
        }
        else if (fileIndex + 1 >= fileSize) {
            printf("levelInit ERROR: Improper Line Formatting At EOF");
            return false;
            continue;
        }

        cmd = fileData[fileIndex + 1];

        switch(cmd) {
            case 'V':
                if (fileData[fileIndex + 2] == '3') {
                    position = levelReadVector3(&(fileData[fileIndex]));
                    ecsAddPosition(eIdQ->currentId, &position, ecs);
                    break;
                }
                else {
                    printf("levelInit ERROR: Unrecognized Vector Type %c", vecType);
                    return false;
                    break;
                }
                break;
            case 'G':
                if (ecs->positionSet.sparse[eIdQ->currentId] == -1) {
                    printf("levelInit ERROR: Attempt To Add To Grid Without Position");
                    return false;
                    break;
                }

                position = ecs->positions[ecs->positionSet.sparse[eIdQ->currentId]];
                gridSetFromMap(position.x, position.y, eIdQ->currentId, grid);
                break;

            case 'R':
                render = levelReadRenderer(&(fileData[fileIndex]));
                ecsAddRender(eIdQ->currentId, &render, ecs);
                break;

            case 'P':
                pushable = levelReadPushable(&(fileData[fileIndex]));
                ecsAddPushable(eIdQ->currentId, &pushable, ecs);
                break;

            case 'E':
                eIdQ->currentId++; 
 
                if (eIdQ->currentId >= eIdQ->maxEntities) {
                    printf("levelInit ERROR: Exceeded Max Entity Count");
                    return false;
                }

                break;

            case 'O':
                goal = levelReadGoal(&(fileData[fileIndex]));
                ecsAddGoal(eIdQ->currentId, &goal, ecs);
                break;

            case 'B':
                box = levelReadBox(&(fileData[fileIndex]));
                ecsAddBox(eIdQ->currentId, &box, ecs);
                break;

            default:
                printf("levelInit ERROR: unknown file command %c", cmd);
                return false;
                break;

        }

        fileIndex += levelLineLength(fileIndex, fileSize, fileData);
    }

    return true;
}

bool levelLoad(const char *fileName, ECS *ecs, EntityIdQueue *eIdQ, Grid *grid) {
    if (!FileExists(fileName)) {
        printf("loadLevel ERROR: File %s Not Found", fileName);
        return false;
    }

    int fileSize = GetFileLength(fileName);
    int fileIndex = 0;
    unsigned char *fileData = LoadFileData(fileName, &fileSize);

    bool loaded = levelInit(ecs, eIdQ, grid, fileIndex, fileSize, fileData);

    UnloadFileData(fileData);
    return loaded;
}
