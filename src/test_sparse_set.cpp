#include <iostream>
#include "sparse_set.h"

void testAdd(int val, SparseSet* set) {
    if (sparseContains(val, set)) {
        std::cout << "Caught Re-Add" << '\n';
        sparseAdd(val, set);
        sparsePrint(set);
        std::cout << '\n';
        return;
    }

    std::cout << "Adding " << val << ":" << '\n';
    sparseAdd(val, set);
    if (!sparseContains(val, set)) std::cout << "Failed to Add" << '\n';
    else std::cout << "Successful Add" << '\n';
    sparsePrint(set);
    std::cout << '\n';
}

void testRemove(int val, SparseSet* set) {
    if (!sparseContains(val, set)) {
        std::cout << "Caught False Delete" << '\n';
        sparseRemove(val, set);
        sparsePrint(set);
        std::cout << '\n';
        return;
    }

    std::cout << "Removing " << val << ":" << '\n';
    sparseRemove(val, set);
    if (sparseContains(val, set)) std::cout << "Failed to Remove" << '\n';
    else std::cout << "Successful Remove" << '\n';
    sparsePrint(set);
    std::cout << '\n';
}

int main() {
    SparseSet set;
    sparseInit(2000, &set);

    std::cout << "Initial Print (nothing): " << '\n';
    sparsePrint(&set);
    std::cout << '\n';

    testAdd(1, &set);
    testAdd(0, &set);
    testAdd(7, &set);
    testAdd(7, &set);
    testAdd(set.maxSize + 1, &set);
    testAdd(9, &set);
    testAdd(3, &set);

    testRemove(0, &set);
    testRemove(1, &set);
    testRemove(0, &set);
    testRemove(7, &set);
    testRemove(9, &set);
    testRemove(3, &set);
    testRemove(set.maxSize + 1, &set);

    sparseFree(&set);

    return 0;
}
