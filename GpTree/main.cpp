
#include "GpTree.h"
#include <stdlib.h>
#include <time.h>
using namespace std;

int turn = 0;
int state[2][2][5] ={{{0}}};

int (*selfState)[5] = state[!turn];
int (*oppState)[5] = state[turn];


int evalTree(GeneTree *p) {
    if(!p) return 0;

    int opd1 = evalTree(p->left);
    int opd2 = evalTree(p->right);

    switch(p->opt) {
    case '+':
        return opd1 + opd2;
    case '-':
        return opd1 - opd2;
    case '*':
        return opd1 * opd2;
    case '/':
        if(opd2 == 0) return opd1;
        return opd1 / opd2;
    case '%':
        if(opd2 == 0) return opd1;
        return opd1 % opd2;
    case AZ:
        return opd1 > 0 ? opd2 : 0;
    case AEZ:
        return opd1 >= 0 ? opd2 : 0;
    case EZ:
        return opd1 == 0 ? opd2 : 0;
    case NZ:
        return opd1 != 0 ? opd2 : 0;
    case EL:
        return opd1 != 0 ? opd1 : opd2;
    case 'n':
        return p->data;
    case SL:
        return selfState[0][p->data];
    case SBL:
        return selfState[1][p->data];
    case OL:
        return oppState[0][p->data];
    case OBL:
        return oppState[1][p->data];
    default: //that should not happen
        cout << "Error, unknown opt !!!" << std::endl;
        return 0;
    }
}


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
/*
    ofstream fout("gene.txt");
    if(!fout) {
        cout << "Can not create file!!\n";
        return 1;
    }

    GeneTree *exTree;
    randBranch(exTree, 5);
*/

    /*  exTree = new GeneTree('+');
        exTree->left = new GeneTree('*');
        exTree->left -> left = new GeneTree(83);
        exTree->left -> right = new GeneTree(12);
        exTree->right = new GeneTree(16);
    */
    /*
    exportTree(exTree, fout);
    fout.close();
*/
    GeneTree *readTree;

    ifstream fin("gene.txt");
    importTree(readTree, fin);
    cout << getNodeCount(readTree) << endl;
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
