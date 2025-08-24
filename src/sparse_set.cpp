#include "sparse_set.h"

void sparseInit(int maxSize, SparseSet *set) {
    set->size = 0;
    set->maxSize = maxSize;
    set->dense = (int *) malloc(maxSize * sizeof(int));
    set->sparse = (int *) malloc(maxSize * sizeof(int));

    for (int i = 0; i < maxSize; i++) {
        set->sparse[i] = -1;
        set->dense[i] = -1;
    }
}

void sparseFree(SparseSet *set) {
    free(set->dense);
    free(set->sparse);
    set->maxSize = 0;
}

bool sparseContains(int id, SparseSet *set) {
    if (id >= set->maxSize || (set->sparse[id] < 0)) return false;

    if (set->dense[(set->sparse[id])] == id) return true;

    return false;
}

void sparseAdd(int id, SparseSet *set) {
    if (id >= set->maxSize) return;
    if (sparseContains(id, set)) return;
    if ((set->size) + 1 > set->maxSize) return;

    set->sparse[id] = set->size;
    set->dense[set->size] = id;
    set->size++;
}

void sparseRemove(int id, SparseSet *set) {
    if (id >= set->maxSize) return;
    if (!sparseContains(id, set)) return;
    if (set->size == 0) return;

    // store the last dense element (index in sparse array)
    int tempDToS = set->dense[set->size - 1];

    // change the dense element associated with id to the stored dense element
    set->dense[set->sparse[id]] = tempDToS;

    // note the change in position of the last dense element 
    set->sparse[tempDToS] = set->sparse[id];

    // clear id from the sparse array
    set->sparse[id] = -1;

    // clear the id from the dense array
    set->dense[set->size - 1] = -1;
    
    // note we have one less element now
    set->size--;
}

void sparsePrint(SparseSet *set) {
    std::cout << "Sparse Set {" << "\n";
    std::cout << "Current Size: " << set->size << '\n';
    std::cout << "Max Size: " << set->maxSize << '\n';
    for (int i = 0; i < set->maxSize; i++) {
        if (set->sparse[i] != -1) {
            std::cout << "S: " << i << " D: " << set->sparse[i] << '\n';
        }
    }
    std::cout << "}" << '\n';
}
