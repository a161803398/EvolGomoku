#ifndef EVOL_H_INCLUDED
#define EVOL_H_INCLUDED

#include "GpTree.h"
#include <algorithm>

const int RunLimit = 6;
const int Population = RunLimit * 4;
const int InitialDepth = 3;
const int GenerationLimit = 100;

struct TreeScore {
    GeneTree *tree;
    int score;
    bool operator < (const TreeScore &treeScore) const {
        return score < treeScore.score;
    }
    bool operator > (const TreeScore &treeScore) const {
        return score > treeScore.score;
    }
    bool operator >= (const TreeScore &treeScore) const {
        return score >= treeScore.score;
    }
    bool operator <= (const TreeScore &treeScore) const {
        return score <= treeScore.score;
    }
    bool operator == (const TreeScore &treeScore) const {
        return score == treeScore.score;
    }
};
void nPlusN(TreeScore *population, TreeScore *offspring);
void random10(TreeScore *population, TreeScore *offspring);

#endif // EVOL_H_INCLUDED
