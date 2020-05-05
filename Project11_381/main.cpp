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

    // Methods

    void zeroFrame(){
        for(int row = 0; row < numRows+2; row++){
            imgAry[row][0] = 0;
            imgAry[row][numCols+1] = 0;
        }

        for(int col = 0; col < numCols+2; col++){
            imgAry[0][col] = 0;
            imgAry[numRows+1][col] = 0;
        }
    }

    void loadImage(ifstream& inFile){
        int value;
        
        for(int i = 1; i < numRows+1; i++){
            for(int j= 1; j < numCols+1; j++){
                inFile >> value;
                imgAry[i][j] = value;
            }
        }
    }

    void computeHPP(){
        int sum = 0;
        for(int row = 0; row < numRows+2; row++){
            for(int col = 0 ; col < numCols+2; col++){
                if(imgAry[row][col] > 0){
                    sum++;
                }
            }
            HPP[row] = sum;
            sum = 0;
        }
    }

};



int main(int argc, char* argv[]){
    ifstream inFile(argv[1]);
    int inputThreshold = stoi(argv[2]);
    ofstream outFile1(argv[3]);
    ofstream outFile2(argv[4]);


    imagePP image;
    inFile >> image.numRows;
    inFile >> image.numCols;
    inFile >> image.minVal;
    inFile >> image.maxVal;

    image.imgAry = new int*[image.numRows+2];
    for(int i = 0; i < image.numRows+2;i++){
        image.imgAry[i] = new int[image.numCols+2];
    }

    image.zeroFrame();

    image.thresholdVal = inputThreshold;

    // Size to numRows+2, numCols+2
    image.HPP = new int[image.numRows+2];
    image.VPP = new int[image.numCols+2];

    image.HPPbin = new int[image.numRows+2];
    image.VPPbin = new int[image.numCols+2];

    // Init to zero
    for(int i = 0; i < image.numRows+2; i++){
        image.HPP[i] = 0;
        image.HPPbin[i] = 0;
    }
    for(int i = 0; i < image.numCols+2; i++){
        image.VPP[i] = 0;
        image.VPPbin[i] = 0;
    }

    // Load image to imageAry
    image.loadImage(inFile);


    // Compute HPP
    image.computeHPP();


    for(int i = 0; i < image.numRows+2;i++){
        cout << image.HPP[i] << " ";
    }

    cout << endl;
    cout << endl;
    cout << endl;




    
    
    
    
    
    
    
    
    
    
    
    // for(int i = 0; i < image.numCols+2; i++){
    //     cout << image.VPP[i];
    // }
    //     cout << endl;
    // for(int i = 0; i < image.numCols+2; i++){
    //     cout << image.VPPbin[i];
    // }
        // cout << endl;
    // PRINTS IMAGE ARRAY with frame
    for(int i = 0; i < image.numRows+2; i++){
        for( int j = 0; j < image.numCols+2; j++){
            cout << image.imgAry[i][j];
        }
        cout << endl;
    }


    

    









    inFile.close();
    outFile1.close();
    outFile2.close();
    return 0;
}