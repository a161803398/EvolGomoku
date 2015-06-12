
#include "GpTree.h"
#include <stdlib.h>
#include <time.h>
using namespace std;
int main() {
    srand(time(0));
/*
    //for(int i =0; i<10; i++) {
    GeneTree *parent1, *parent2;
    randBranch(parent1, 5);
    randBranch(parent2, 5);

    printTree(parent1);
    cout << endl << "score = " << evalTree(parent1)<< endl;
*/

/*
    printTree(parent2);
    cout <<endl;
*/

/*
    mutateTree(parent1);

    printTree(parent1);
        cout << endl << "score = " << evalTree(parent1)<< endl;
*/

/*
    GeneTree *child1, *child2;
    cpTree(child1, parent1);
    cpTree(child2, parent2);

    crossOver(child1, child2);

    printTree(child1);
    cout <<endl;

    printTree(child2);
    cout <<endl;
*/

    //}

    ofstream fout("gene.txt");
    if(!fout) {
        cout << "Can not create file!!\n";
        return 1;
    }

    GeneTree *exTree;
    randBranch(exTree, 5);


    /*  exTree = new GeneTree('+');
        exTree->left = new GeneTree('*');
        exTree->left -> left = new GeneTree(83);
        exTree->left -> right = new GeneTree(12);
        exTree->right = new GeneTree(16);
    */
    exportTree(exTree, fout);
    fout.close();

    GeneTree *readTree;

    ifstream fin("gene.txt");
    importTree(readTree, fin);
    //printTree(readTree);
    //exportTree(readTree, cout);

    /*
     char ch;

    fin.unsetf(ios::skipws);
    while(!fin.eof()) {
     fin >> ch;
     if(!fin.eof()) cout << ch;
    }
    */
    fin.close();

    return 0;
}
