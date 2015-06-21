#include <stdio.h>
#include <windows.h>
#include <string>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include "myPipe.hpp"
#include "Evol.h"

using namespace std;

bool callJudge(HANDLE &fromJudge, int judgeId, const char* genePath1, const char* genePath2);
bool evalScore(TreeScore *tar, const char *folderName, bool first, const char *standard = "ref.exe");

TreeScore parentTrees[Population];

int generation = 0;

void outputGene(char *folderName, TreeScore *tar) {
    char tmp[1024];
    sprintf(tmp, "mkdir \"genes\\%s\"", folderName);
    system(tmp);

    for(int i = 0; i < Population; i++) {
        sprintf(tmp, "genes\\%s\\i%d.txt", folderName, i);
        ofstream treeOut(tmp);
        exportTree(tar[i].tree, treeOut);
        treeOut.close();
    }
}

void outputScore(char *folderName, TreeScore *tar) {
    char tmp[1024];
    sprintf(tmp, "genes\\%s\\score.txt", folderName);
    ofstream scoreOut(tmp);
    for(int i = 0; i < Population; i++) {
        scoreOut << tar[i].score << endl;
    }
    scoreOut.close();
}


bool evalScore(TreeScore *tar, const char *folderName, bool first, const char *standard) {

    char tmp[1024];
    HANDLE fromJudges[Population];
    for(int j = 0; j < Population / RunLimit; j++) {
        //start judges
        for(int i = j * RunLimit; i < (j + 1) * RunLimit; i++) {
            sprintf(tmp, "genes\\%s\\i%d.txt", folderName, i);
            if(first) {
                if(!callJudge(fromJudges[i], i, tmp, standard)) { //first
                    cout << "Judge" <<  i << " do not respond, exit!!" << endl;
                    return false;
                }
            } else {
                if(!callJudge(fromJudges[i], i, standard, tmp)) {
                    cout << "Judge" <<  i << " do not respond, exit!!" << endl;
                    return false;
                }
            }
        }

        //get scores from judges
        for(int i = j * RunLimit; i < (j + 1) * RunLimit; i++) {
            getMsg(tmp, fromJudges[i]);
            int player1Score;
            sscanf(tmp, "%d", &player1Score);

            if(first) { //I am player1
                tar[i].score += player1Score;
            } else {
                tar[i].score -= player1Score;
            }

            DisconnectNamedPipe(fromJudges[i]); //read finish
            CloseHandle(fromJudges[i]);
        }

    }



    return true;
}


void makeReport(ofstream &report) {
    int best = -1000;
    double average = 0;

    for(int i = 0; i < Population; i++) {
        if(parentTrees[i].score > best) best = parentTrees[i].score;
        average += parentTrees[i].score;
    }
    average /= Population;
    report << generation << ", " << best << ", " << average << endl;
    cout << "Generation " << generation << ": " << best << ", " << average << endl;

    report.flush();
}


int main(int argc, char* argv[]) {
    char tmp[1024];
    srand(time(0));

    ofstream report;
    report.open("report.txt", std::ios_base::app);

    printf("Continue from generation : ");
    scanf("%d", &generation);

    if(generation <= 0) {
        for(int i = 0; i < Population; i++) {
            randBranch(parentTrees[i].tree, InitialDepth, i < Population / 2); //half full, half not
        }
        outputGene("g0", parentTrees);

        for(int i = 0; i < Population; i++) parentTrees[i].score = 0;
        for(int i = 0; i < EvaluationTime; i++) {
            evalScore(parentTrees, "g0", true, "Backgammon.exe");
            evalScore(parentTrees, "g0", false, "Backgammon.exe");
            evalScore(parentTrees, "g0", true);
            evalScore(parentTrees, "g0", false);
        }



        outputScore("g0", parentTrees);
        makeReport(report);
    } else { //load score and tree
        generation--;
        for(int i = 0; i < Population; i++) {
            sprintf(tmp, "genes\\g%d\\i%d.txt", generation, i);
            ifstream treeImport(tmp);
            importTree(parentTrees[i].tree, treeImport);
            treeImport.close();
        }
        /*
                //reevaluate score
                sprintf(tmp, "g%d", generation);
                for(int i = 0; i < Population; i++) parentTrees[i].score = 0;
                for(int i = 0; i < EvaluationTime; i++) {
                    evalScore(parentTrees, tmp, true);
                    evalScore(parentTrees, tmp, false);
                }
                outputScore(tmp, parentTrees);
                makeReport(report);

        */
        //load score
        sprintf(tmp, "genes\\g%d\\score.txt", generation);
        ifstream scoreImport(tmp);
        for(int i = 0; i < Population; i++) {
            scoreImport >> parentTrees[i].score;
        }
        scoreImport.close();
    }

    while(generation < GenerationLimit) {
        TreeScore childrenTree[Population];
        //matting selection
        for(int i = 0; i < Population; i++) {
            int toCompare = rand() % Population;
            while(i == toCompare)toCompare = rand() % Population; //not to compare with self
            TreeScore *toPut = &parentTrees[i];
            if(parentTrees[toCompare] > *toPut) toPut = &parentTrees[toCompare];
            cpTree(childrenTree[i].tree, toPut -> tree);
            //mutate parent copy
            if(rand() % 10 < 1) mutateTree(childrenTree[i].tree); //mutate rate is 10%
        }

        //crossover
        for(int i = 0; i < Population; i+=2) {
            crossOver(childrenTree[i].tree, childrenTree[i + 1].tree);
        }

        char childrenfolder[1024];
        sprintf(childrenfolder, "g%d\\children", generation);

        //out gene for evaluate
        outputGene(childrenfolder, childrenTree);

        //evaluate children trees
        for(int i = 0; i < Population; i++) childrenTree[i].score = 0;
        for(int i = 0; i < EvaluationTime; i++) {
            evalScore(childrenTree, childrenfolder, true, "Backgammon.exe");
            evalScore(childrenTree, childrenfolder, false, "Backgammon.exe");
            evalScore(childrenTree, childrenfolder, true);
            evalScore(childrenTree, childrenfolder, false);
        }
        //output score
        outputScore(childrenfolder, childrenTree);

        nPlusN(parentTrees, childrenTree);

        //remove all child
        for(int i = 0; i < Population; i++) delTree(childrenTree[i].tree);

        generation++;

        char generationFolder[1024];
        sprintf(generationFolder, "g%d", generation);
        outputGene(generationFolder, parentTrees);
        outputScore(generationFolder, parentTrees);

        makeReport(report);

    }


    report.close();
    system("pause");
    return 0;
}


bool callJudge(HANDLE &fromJudge, int judgeId, const char* genePath1, const char* genePath2) {
    char tmp[1024];

    sprintf(tmp, "\\\\.\\pipe\\FromJudge%d", judgeId);
    createPipe(fromJudge, tmp); //create pipe for judge
    sprintf(tmp, "start Judge.exe Judge%d %s %s", judgeId, genePath1, genePath2);

    system(tmp); //execute program

    ConnectNamedPipe(fromJudge, NULL);//wait juge connect

    getMsg(tmp, fromJudge);

    if(tmp[0] != 'r') return false;
    return true;
}
