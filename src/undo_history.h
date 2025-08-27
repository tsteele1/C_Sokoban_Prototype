#ifndef UNDO_HISTORY_H
#define UNDO_HISTORY_H

struct MoveData {
    int entityId;
    int newX;
    int newY;
    int dx;
    int dy;
    int z;
    unsigned int timeStamp;
};

struct UndoHistory {
    MoveData *history;
    unsigned int size;
    unsigned int maxSize;
    unsigned int currentTime;
};

#endif
