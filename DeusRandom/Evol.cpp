#include "Evol.h"

void nPlusN(TreeScore *population, TreeScore *offspring) {
    std::sort(population, population + Population, std::greater<TreeScore>());
    std::sort(offspring, offspring + Population, std::greater<TreeScore>());

    //merge sort

    int parentId = 0, childId = 0;
    while(parentId < Population && childId < Population) {
        if(population[parentId] <= offspring[childId]) { // "<=" for more swap
            std::swap(population[parentId], offspring[childId]);
            childId ++; //already use one child
        }
        parentId ++;
    }
}


void random10(TreeScore *population, TreeScore *offspring) {
    for(int i = 0; i < Population; i++) {
        std::swap(population[i], offspring[i]);
    }
    for(int i = 0; i < Population / 10; i++) {
        int randKeep = rand() % Population;
        std::swap(population[randKeep], offspring[randKeep]);
    }
}

