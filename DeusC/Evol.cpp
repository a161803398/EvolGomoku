#include "Evol.h"

void nPlusN(TreeScore *population, TreeScore *offspring) {
    std::sort(population, population + Population, std::greater<TreeScore>());
    std::sort(offspring, offspring + Population, std::greater<TreeScore>());

    //merge sort

    int parentId = 0, childId = 0;
    while(parentId < Population && childId < Population){
        if(population[parentId] <= offspring[childId]){ // "<=" for more swap
            std::swap(population[parentId], offspring[childId]);
            childId ++; //already use one child
        }
        parentId ++;
    }
}


