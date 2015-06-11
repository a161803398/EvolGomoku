#include "GpTree.h"
using std::cout;
using std::string;
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

void randTree(GeneTree *&p, int maxDeep, bool forceFull) {
    if(maxDeep > 0 && (forceFull || (rand() % 2))) { //if not force full, 50% branch
        randTree2C(p, maxDeep, forceFull);
    } else {
        char opt = TermSet[rand() % TermLength]; //rand select some value from TermSet
        if(opt == 'n') { //normal number
            p = new GeneTree(opt, rand() % MaxScore - MaxScore / 2);
        } else {
            p = new GeneTree(opt, rand() % CountTableLength);
        }
    }
}

void randTree2C(GeneTree *&p, int maxDeep, bool forceFull) {
    char opt;
    if(rand() % 10 < 8){ //80% Arithmetic opt
        opt = FunSet[rand() % FunLength]; //rand select some function from FunSet
    }else{ //20% logic opt
        opt = LogicSet[rand() % LogicLength];
    }

    p = new GeneTree(opt);
    randTree(p->left, maxDeep - 1, forceFull);
    randTree(p->right, maxDeep - 1, forceFull);
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

void evalTree(GeneTree *p) {
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
