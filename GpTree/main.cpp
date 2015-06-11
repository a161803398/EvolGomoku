
#include "GpTree.h"
#include <stdlib.h>
#include <time.h>
using namespace std;
int main() {
    srand(time(0));

    for(int i =0; i<10; i++) {
        GeneTree *exTree;
        randTree2C(exTree, 5);
        printTree(exTree);
        cout <<endl;
    }

    ofstream fout("gene.txt");
    if(!fout) {
        cout << "Can not create file!!\n";
        return 1;
    }

    GeneTree *exTree;
    randTree2C(exTree, 10);

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
    printTree(readTree);
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
