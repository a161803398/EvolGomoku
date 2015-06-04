#include "GpTree.h"
using std::cout;
void exportTree(GeneTree *p, std::ostream &out) {
    if (!p) {
        out << "# ";
    } else {
        out << (p->opt) << (p->data) << " ";
        exportTree(p->left, out);
        exportTree(p->right, out);
    }
}

void importTree(GeneTree *&p, std::ifstream &fin) {
    char opt;
    int data;
    if(fin.eof()) return; //if EOF
    fin >> opt;

    if (opt != '#') { //if there is a node(tree)
        fin >> data;
        p = new GeneTree(opt, data);
        importTree(p->left, fin);
        importTree(p->right, fin);
    }
}

void printTree(GeneTree *p) {
    if (p) {
        cout << "(";
        printTree(p->left);
        cout << " ";
        //output current node
        cout << (p->opt);
        cout << (p->data);
        cout << " ";
        printTree(p->right);
        cout << ")";
    }
}
