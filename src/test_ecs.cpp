#include "ecs.cpp"
#include "sparse_set.cpp"
#include "iostream"

int main() {
    const int numEntities = 2000;
    const char levelName[] = "./test_level.ec";
    int entityId = 0;

    ECS ecs; 
    ecsInit(numEntities, &ecs);

    Vector3 position = {0, 0, 10};
    ecsAddPosition(entityId, &position, &ecs);
    position = (Vector3) {5, 4, 8};
    ecsAddPosition(++entityId, &position, &ecs);
    position = (Vector3) {3, 1, 1};
    ecsAddPosition(++entityId, &position, &ecs);
    position = (Vector3) {0, 2, 7};
    ecsAddPosition(++entityId, &position, &ecs);
    position = (Vector3) {2, 6, 3};
    ecsAddPosition(++entityId, &position, &ecs);
    position = (Vector3) {1, 1, 3};
    ecsAddPosition(++entityId, &position, &ecs);
    position = (Vector3) {1, 3, 3};
    ecsAddPosition(++entityId, &position, &ecs);
    position = (Vector3) {6, 7, 7};
    ecsAddPosition(++entityId, &position, &ecs);

    SetRandomSeed(GetTime() * 1000);

//void ecsAddPositionZSorted(int entityId, Vector3 *position, ECS *ecs)
//void ecsSwapPositionIndices(int low, int high, ECS *ecs)
//int ecsPartitionPositionsByZ(int low, int pivot, int high, ECS *ecs)
//void ecsQuicksortPositionsByZ(int low, int high, ECS *ecs)
//void ecsZSort(ECS *ecs)

    /*
    std::cout << "TESTING SWAP:\n";
    std::cout << "ID 0: " << ecs.positionSet.sparse[0] << ' ' << ecs.positionSet.dense[ecs.positionSet.sparse[0]] << ' ' << ecs.positions[ecs.positionSet.sparse[0]].z << '\n';
    std::cout << "ID 7: " << ecs.positionSet.sparse[7] << ' ' << ecs.positionSet.dense[ecs.positionSet.sparse[7]] << ' ' << ecs.positions[ecs.positionSet.sparse[7]].z << '\n';

    ecsSwapPositionIndices(0, 7, &ecs);

    std::cout << "ID 0: " << ecs.positionSet.sparse[0] << ' ' << ecs.positionSet.dense[ecs.positionSet.sparse[0]] << ' ' << ecs.positions[ecs.positionSet.sparse[0]].z << '\n';
    std::cout << "ID 7: " << ecs.positionSet.sparse[7] << ' ' << ecs.positionSet.dense[ecs.positionSet.sparse[7]] << ' ' << ecs.positions[ecs.positionSet.sparse[7]].z << '\n';
    */

    /*
    int mid = (ecs.positionSet.size) / 2;
    int midVal = ecs.positions[mid].z;
    mid = ecsPartitionPositionsByZ(0, mid, 7, &ecs);
    std::cout << "MID: " << mid << '\n';
    std::cout << "MIDVAL: " << midVal << '\n';
    for (int i = 0; i < ecs.positionSet.size; i++) {
        std::cout << ecs.positions[i].z << '\n';
    }

    mid = ecsPartitionPositionsByZ(0, (int) mid/2, mid, &ecs); 
    std::cout << "MID: " << mid << '\n';
    std::cout << "MIDVAL: " << midVal << '\n';
    for (int i = 0; i < ecs.positionSet.size; i++) {
        std::cout << ecs.positions[i].z << '\n';
    }
    */


    std::cout << "TESTING SORT:\n";
    ecsZSort(&ecs);
    for (int i = 0; i < ecs.positionSet.size-1; i++) {
        if (ecs.positions[i].z <= ecs.positions[i+1].z) continue;

        std::cout << "INVALID SORT: " << '\n';
        sparsePrint(&(ecs.positionSet));
        return -1;
    }
    std::cout << "SUCCESS" << '\n';


    std::cout << "TESTING INSERTION WITH SORT:" << '\n';
    position = (Vector3) {0, 0, 11};
    ecsAddPositionZSorted(++entityId, &position, &ecs);
    for (int i = 0; i < ecs.positionSet.size-1; i++) {
        if (ecs.positions[i].z <= ecs.positions[i+1].z) continue;

        std::cout << "INVALID SORT: " << '\n';
        sparsePrint(&(ecs.positionSet));
        return -1;
    }
    std::cout << "SUCCESS" << '\n';

    position = (Vector3) {5, 10, 2};
    ecsAddPositionZSorted(++entityId, &position, &ecs);
    for (int i = 0; i < ecs.positionSet.size-1; i++) {
        if (ecs.positions[i].z <= ecs.positions[i+1].z) continue;

        std::cout << "INVALID SORT: " << '\n';
        sparsePrint(&(ecs.positionSet));
        return -1;
    }
    std::cout << "SUCCESS" << '\n';

    position = (Vector3) {3, 4, 7};
    ecsAddPositionZSorted(++entityId, &position, &ecs);
    for (int i = 0; i < ecs.positionSet.size-1; i++) {
        if (ecs.positions[i].z <= ecs.positions[i+1].z) continue;

        std::cout << "INVALID SORT: " << '\n';
        sparsePrint(&(ecs.positionSet));
        return -1;
    }
    std::cout << "SUCCESS" << '\n';

    std::cout << "TESTS COMPLETE!\n";

    ecsFree(&ecs);

    return 0;
}
