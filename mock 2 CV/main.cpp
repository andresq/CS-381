#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class Image{
    public:
    int numRows;
    int numCols;
    int minVal;
    int maxVal;

    int** imageAry;

    void loadImageAry(ifstream& inFile){
        imageAry = new int*[numRows];
        for(int i = 0; i < numRows; i++){
            imageAry[i] = new int[numCols];
        }

        int input;

        for(int i = 0; i < numRows; i++){
            for(int j= 0; j < numCols; j++){
                inFile >> input;
                imageAry[i][j] = input;
                }
        }
    }

    void prettyPrint(ofstream& outFile){
        for(int i = 0; i < numRows; i++){
            for(int j= 0; j < numCols; j++){
                if(imageAry[i][j] > 0){
                    outFile << imageAry[i][j] << " ";
                } else {
                    outFile << "  "; // two spaces
                }
            }
            outFile << endl;
        }
    }
};

class RCAry{
    public:
    int numRows;
    int numCols;
    int minVal;
    int maxVal;
    int thresholdVal;

    int* sumRowsAry;
    int* sumColAry;

    void loadRowAry(ifstream& rowFile){
        sumRowsAry = new int[numRows];

        // image header has been read by now so now 
        // pairs (index, sum)
        int index, sum;
        while(!rowFile.eof()){
            rowFile >> index;
            rowFile >> sum;
            sumRowsAry[index] = sum;
        }
    }


    void loadColAry(ifstream& colFile){
        sumColAry = new int[numCols];


        // pairs (index, sum)
        int index, sum;
        while(!colFile.eof()){
            colFile >> index;
            colFile >> sum;
            sumColAry[index] = sum;
        }
    }

    void printRowAry(ofstream& imgHPP, ofstream& imgHPPbin){
        // Header first
        imgHPP << numRows << " " << numCols << " " << minVal << " " << maxVal << endl;
        for(int i = 0; i < numRows; i++){
            imgHPP << sumRowsAry[i] << endl;
        }

        // 1-D array with threshold 4
        for(int i = 0; i < numRows; i++){
            if(sumRowsAry[i] >= thresholdVal){
                imgHPPbin <<  "1 "; // per-line not specified
            } else {
                imgHPPbin << "0 ";
            }
        }
    }

    void printColAry(ofstream& imgVPP, ofstream& imgVPPbin){
        // Header
        imgVPP << numRows << " " << numCols << " " << minVal << " " << maxVal << endl;

        for(int i = 0; i < numCols; i++){
            imgVPP << sumColAry[i] << endl;
        }

        for(int i = 0; i < numCols; i++){
            if(sumColAry[i] >= thresholdVal){
                imgVPPbin << "1 ";
            } else {
                imgVPPbin << "0 ";
            }
        }
    }



};

int main(int argc, char* argv[]){
    ifstream imageFile(argv[1]);
    ifstream rowFile(argv[2]);
    ifstream colFile(argv[3]);

    ofstream imgPrettyPrint(argv[4]);
    ofstream imgHPP(argv[5]);
    ofstream imgVPP(argv[6]);
    ofstream imgHPPbin(argv[7]);
    ofstream imgVPPbin(argv[8]);

    Image image;
    // Need to read first
    imageFile >> image.numRows;
    imageFile >> image.numCols;
    imageFile >> image.minVal;
    imageFile >> image.maxVal;

    image.loadImageAry(imageFile);
    image.prettyPrint(imgPrettyPrint);

    RCAry Arys;
    Arys.thresholdVal = 4;

    //Row stuff
    rowFile >> Arys.numRows;
    rowFile >> Arys.numCols;
    rowFile >> Arys.minVal;
    rowFile >> Arys.maxVal;

    Arys.loadRowAry(rowFile);
    Arys.printRowAry(imgHPP, imgHPPbin);


    //Col stuff
    colFile >> Arys.numRows; // Still need to read these
    colFile >> Arys.numCols;
    colFile >> Arys.minVal;
    colFile >> Arys.maxVal;

    Arys.loadColAry(colFile);

    Arys.printColAry(imgVPP, imgVPPbin);


    // Closing files
    imageFile.close();
    rowFile.close();
    colFile.close();

    imgPrettyPrint.close();
    imgHPP.close();
    imgVPP.close();
    imgHPPbin.close();
    imgVPPbin.close();

}