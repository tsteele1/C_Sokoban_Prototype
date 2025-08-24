#ifndef SPARSESET_H
#define SPARSESET_H

#include <iostream>
#include <cstdlib>

struct SparseSet {
    int size;
    int maxSize;
    int *sparse;
    int *dense;
};

void sparseInit(int maxSize, SparseSet *set);

void sparseFree(SparseSet *set);

bool sparseContains(int id, SparseSet *set);

void sparseAdd(int id, SparseSet *set);

void sparseRemove(int id, SparseSet *set);

void sparsePrint(SparseSet *set);

#endif
