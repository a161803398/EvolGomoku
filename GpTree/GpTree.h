#ifndef GPTREE_H_INCLUDED
#define GPTREE_H_INCLUDED
#include <iostream>
#include <fstream>

class GeneTree {
    public:
    GeneTree *left;
    GeneTree *right;
    char opt;
    int data;

    GeneTree(char opt, int data){
        this->left = 0;
        this->right = 0;
        this->opt = opt;
        this->data = data;
    }

    GeneTree(int data){
        this->left = 0;
        this->right = 0;
        this->opt = 'n';
        this->data = data;
    }

    GeneTree(char opt){
        this->left = 0;
        this->right = 0;
        this->opt = opt;
        this->data = 0;
    }
};
void exportTree(GeneTree *p, std::ostream &out);
void importTree(GeneTree *&p, std::ifstream &fin);
void printTree(GeneTree *p);
#endif // GPTREE_H_INCLUDED
