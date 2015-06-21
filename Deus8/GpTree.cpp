#include "GpTree.h"
using std::cout;
using std::string;

void cpTree(GeneTree *&tar, GeneTree *src) {
    if(!src) {//if not node
        tar = 0;
        return;
    }

    tar = new GeneTree(src->opt, src->data);
    cpTree(tar->left, src->left);
    cpTree(tar->right, src->right);
}

void delTree(GeneTree *&p) {
    if(!p) return; //if not node
    delTree(p->left);
    delTree(p->right);
    delete(p);
    p = 0;
}

void exportTree(GeneTree *p, std::ostream &out) {
    if (!p) return;
    out << (p->opt) << (p->data) << " ";

    exportTree(p->left, out);
    exportTree(p->right, out);
}

void importTree(GeneTree *&p, std::ifstream &fin) {
    char opt;
    int data;
    if(fin.eof()) return; //if EOF
    fin >> opt;
    fin >> data;

    p = new GeneTree(opt, data);

    if(opt != 'n' && (opt < SL || opt > OBL) ){ //if not leaf node
        importTree(p->left, fin);
        importTree(p->right, fin);
    }
}

void randTerm(GeneTree *&p, int maxDeep, bool forceFull) {
    if(maxDeep > 0 && (forceFull || (rand() % 2))) { //if not force full, 50% branch
        randBranch(p, maxDeep, forceFull);
    } else {
        char opt = TermSet[rand() % TermLength]; //rand select some value from TermSet(no number)
        if(rand() % 2) opt = 'n'; //50% number
        if(opt == 'n') { //normal number
            p = new GeneTree(opt, rand() % MaxScore - MaxScore / 2);
        } else {
            p = new GeneTree(opt, rand() % 4 + 1);
        }
    }
}

char getRandOpt() {
    //80% Arithmetic opt
    if(rand() % 10 < 9) return FunSet[rand() % FunLength]; //rand select some function from FunSet
    //20% logic opt
    return LogicSet[rand() % LogicLength];
}

void randBranch(GeneTree *&p, int maxDeep, bool forceFull) {
    p = new GeneTree(getRandOpt());
    randTerm(p->left, maxDeep - 1, forceFull);
    randTerm(p->right, maxDeep - 1, forceFull);
}

int getNodeCount(GeneTree *p) {
    int nodeCount = 1;
    if(p->left) nodeCount += getNodeCount(p->left);
    if(p->right) nodeCount += getNodeCount(p->right);
    return nodeCount;
}


GeneTree** getRandNode(GeneTree **p, int &nodeCount) {
    // (1/placeCount) possibility to select this node
    if(rand() % nodeCount == 0) return p;
    nodeCount --; //this node already drawn

    GeneTree *&thisNode = *p;
    GeneTree **tmp = 0;

    if(thisNode->left) {
        tmp = getRandNode(&(thisNode->left), nodeCount);
        if(tmp) return tmp;
    }

    if(thisNode->right) {
        tmp = getRandNode(&(thisNode->right), nodeCount);
        if(tmp) return tmp;
    }

    return 0;//all fail
}

GeneTree*& getRandNode(GeneTree *&p, int nodeCount) {
    return *getRandNode(&p, nodeCount);
}

GeneTree*& getRandNode(GeneTree *&p) {
    return getRandNode(p, getNodeCount(p));
}

void crossOver(GeneTree *&parent1, GeneTree *&parent2) {
    GeneTree *&subTree1 = getRandNode(parent1);
    GeneTree *&subTree2 = getRandNode(parent2);
    GeneTree *tmp = subTree1;
    subTree1 = subTree2;
    subTree2 = tmp;
}

void mutateTree(GeneTree *&p) {
    GeneTree *&subTree = getRandNode(p); //random select one node

    if(subTree->left == 0) { //if it is term node (because both left and right of term node is 0)
        delete(subTree); //delete this node
        randTerm(subTree, 1); //add new sub tree(may be deep 0 or 1)
    } else { //is inter node
        subTree->opt = getRandOpt(); //change opt
    }
}

void printTree(GeneTree *p) {
    if (p) {
        //output current node
        bool isOpt = true;
        string printOpt;

        switch(p->opt) {
        case '+':
        case '-':
        case '*':
        case '/':
        case '%':
            printOpt = (p->opt);
            break;
        case AZ:
            printOpt = "AZ";
            break;
        case AEZ:
            printOpt = "AEZ";
            break;
        case EZ:
            printOpt = "EZ";
            break;
        case NZ:
            printOpt = "NZ";
            break;
        case EL:
            printOpt = "EL";
            break;

        case 'n':
            isOpt = false;
            cout << (p->data);
            break;
        default:
            isOpt = false;
            cout << (p->opt);
            cout << (p->data);
        }
        if(isOpt) {
            cout << "(";
            printTree(p->left);
            cout << " ";
            cout << printOpt;
            cout << " ";
            printTree(p->right);
            cout << ")";
        }
    }
}
