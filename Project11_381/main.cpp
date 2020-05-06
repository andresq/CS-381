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

    int structElement [3] = {1,1,1};
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

    void computeVPP(){
        int sum = 0;
        for(int col = 0; col < numCols+2; col++){
            for(int row = 0; row < numRows+2; row++){
                if(imgAry[row][col] > 0){
                    sum++;
                }
            }
            VPP[col] = sum;
            sum = 0;
        }
    }

    void threshold(){
        // HPPbin
        for(int i = 0; i < numRows+2; i++){
            if(HPP[i] < thresholdVal){
                HPPbin[i] = 0;
            } else {
                HPPbin[i] = 1;
            }
        }


        // VPPbin
        for(int i = 0; i < numCols+2; i++){
            if(VPP[i] < thresholdVal){
                VPPbin[i] = 0;
            } else {
                VPPbin[i] = 1;
            }
        }
    }


    // Dilation then Erosion
    void closingHPPbin(){
        int* temp = new int[numRows+2];
        // Dilation
        for(int i = 1; i < numRows+1; i++){
            if(HPPbin[i] == 1){
                temp[i-1] = structElement[0];
                temp[i] = structElement[1];
                temp[i+1] = structElement[2];
            }
        }

        // Erosion
        for(int i = 1; i < numRows+1; i++){
            if(temp[i] == structElement[1]){
                if((temp[i-1] == structElement[0]) && (temp[i+1] == structElement[2])){
                    HPPMorph[i] = 1;
                }
            }
        }

        delete[] temp;
    }

    void closingVPPbin(){
        int* temp = new int[numCols+2];
        // Dilation
        for(int i = 1; i < numCols+1; i++){
            if(VPPbin[i] == 1){
                temp[i-1] = structElement[0];
                temp[i] = structElement[1];
                temp[i+1] = structElement[2];
            }
        }

        // Erosion
        for(int i = 1; i < numCols+1; i++){
            if(temp[i] == structElement[1]){
                if((temp[i-1] == structElement[0]) && (temp[i+1] == structElement[2])){
                    VPPMorph[i] = 1;
                }
            }
        }
        delete[] temp;
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

    image.HPPMorph = new int[image.numRows+2];
    image.VPPMorph = new int[image.numCols+2];

    // Init to zero
    for(int i = 0; i < image.numRows+2; i++){
        image.HPP[i] = 0;
        image.HPPbin[i] = 0;
        image.HPPMorph[i] = 0;
    }
    for(int i = 0; i < image.numCols+2; i++){
        image.VPP[i] = 0;
        image.VPPbin[i] = 0;
        image.VPPMorph[i] = 0;
    }

    // Load image to imageAry
    image.loadImage(inFile);


    // Compute HPP
    image.computeHPP();

    // Compute VPP
    image.computeVPP();


    // Output HPP, VPP to outFile2 with captions
    outFile2 << "HPP:" << endl;
    for(int i = 0; i < image.numRows+2;i++){
        outFile2 << image.HPP[i] << " ";
    }
    outFile2 << endl;
    outFile2 << "VPP:" << endl;
    for(int i = 0; i < image.numCols+2;i++){
        outFile2 << image.VPP[i] << " ";
    }
    outFile2 << endl;

    // Thesholding to create HPPbin & VPPbin
    image.threshold();


    // Output HPPbin, VPPbin to outFile2 with captions
    outFile2 << "HPPbin:" << endl;
    for(int i = 0; i < image.numRows+2;i++){
        outFile2 << image.HPPbin[i] << " ";
    }
    outFile2 << endl;
    outFile2 << "VPPbin:" << endl;
    for(int i = 0; i < image.numCols+2;i++){
        outFile2 << image.VPPbin[i] << " ";
    }
    outFile2 << endl;


    // Creating HPPMorph
    image.closingHPPbin();

    // Creating VPPMorph
    image.closingVPPbin();



    
    
    
    
    
    
    
    // for(int i = 0; i < 3; i++){
    //     cout << image.structElement[i] << endl;
    // }
    
    
    
    // for(int i = 0; i < image.numRows+2; i++){
    //     cout << image.HPPMorph[i];
    // }
    //     cout << endl;
    // for(int i = 0; i < image.numCols+2; i++){
    //     cout << image.VPPMorph[i];
    // }
    //     cout << endl;


    // PRINTS IMAGE ARRAY with frame
    // for(int i = 0; i < image.numRows+2; i++){
    //     for( int j = 0; j < image.numCols+2; j++){
    //         cout << image.imgAry[i][j];
    //     }
    //     cout << endl;
    // }


    

    









    inFile.close();
    outFile1.close();
    outFile2.close();
    return 0;
}