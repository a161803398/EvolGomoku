#ifndef GPTREE_H_INCLUDED
#define GPTREE_H_INCLUDED
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
enum : char{
    SL = 'A', SBL, OL, OBL,
    AZ, AEZ, EZ, NZ, EL
};

const int MaxScore = 1000;
const int TermLength = 4;
const int FunLength = 5;
const int LogicLength = 5;

const char TermSet[TermLength] = {SL, SBL, OL, OBL};
const char FunSet[FunLength] = {'+', '-', '*', '/', '%'};
const char LogicSet[LogicLength] = {AZ, AEZ, EZ, NZ, EL};

class GeneTree {
public:
    GeneTree *left;
    GeneTree *right;
    char opt;
    int data;

    GeneTree(char opt, int data) {
        this->left = 0;
        this->right = 0;
        this->opt = opt;
        this->data = data;
    }

    GeneTree(int data) {
        this->left = 0;
        this->right = 0;
        this->opt = 'n';
        this->data = data;
    }

    GeneTree(char opt) {
        this->left = 0;
        this->right = 0;
        this->opt = opt;
        this->data = 0;
    }
};
char getRandOpt();

void delTree(GeneTree *&p);
void cpTree(GeneTree *&tar, GeneTree *src);
void mutateTree(GeneTree *&p);
void exportTree(GeneTree *p, std::ostream &out);
void importTree(GeneTree *&p, std::ifstream &fin);
void printTree(GeneTree *p);
void randTerm(GeneTree *&p, int maxDeep, bool forceFull = false);
void randBranch(GeneTree *&p, int maxDeep, bool forceFull = false);
void crossOver(GeneTree *&parent1, GeneTree *&parent2);
int getNodeCount(GeneTree *p);

GeneTree*& getRandNode(GeneTree *&p, int nodeCount);
GeneTree*& getRandNode(GeneTree *&p);
#endif // GPTREE_H_INCLUDED
