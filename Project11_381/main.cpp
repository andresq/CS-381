#include <iostream>
#include <string>
#include <fstream>
using namespace std;


// Data stuctures
class imagePP{
    public:
    int numRows;
    int numCols;
    int minVal;
    int maxVal;

    class box{
        public:
        int minR;
        int minC;
        int maxR;
        int maxC;
    };

    class boxNode{
        public:
        int boxType; // 1=doc box, 2=paragraph, 3= textLine
        box BBOX;
        boxNode* next;
    };

    class boxQ{
        public:
        boxNode* Qfront;
        boxNode* Qback;
        void insert(boxNode* newBoxNode){
            Qback->next = newBoxNode;
            Qback = newBoxNode;
        }
    };

    int** imgAry;
    int thresholdVal;

    int* HPP;
    int* VPP;
    
    int* HPPbin;
    int* VPPbin;

    int* HPPMorph;
    int* VPPMorph;

    int HPPruns;
    int VPPruns;

};



int main(int argc, char* argv[]){
    ifstream inFile(argv[1]);
    ofstream outFile1(argv[2]);
    ofstream outFile2(argv[3]);










    inFile.close();
    outFile1.close();
    outFile2.close();
    return 0;
}