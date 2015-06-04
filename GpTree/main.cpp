
#include "GpTree.h"
using namespace std;
int main() {
    ofstream fout("gene.txt");
    if(!fout) {
        cout << "Can not create file!!\n";
        return 1;
    }

    GeneTree *exTree;
    exTree = new GeneTree('+');
    exTree->left = new GeneTree('*');
    exTree->left -> left = new GeneTree(83);
    exTree->left -> right = new GeneTree(12);
    exTree->right = new GeneTree(16);

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
